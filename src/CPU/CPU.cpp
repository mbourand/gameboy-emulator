#include "CPU.hpp"

namespace gbmu
{
	CPU::CPU(Memory& memory)
		: _memory(memory), sp(ENTRY_STACK_POINTER), pc(ENTRY_POINT), _cycleTimer(0),
		  _divClock(gbmu::Clock(DIV_FREQUENCY)), _timaClock(gbmu::Clock(0.0)), halted(false), ime(false), _ticks(0),
		  _divLastInc(0), _timaLastInc(0), _ei_next_instruction(false)
	{
		this->registers[Register::A] = 0x01;
		this->registers[Register::F] = 0xB0;
		this->registers[Register::B] = 0x00;
		this->registers[Register::C] = 0x13;
		this->registers[Register::D] = 0x00;
		this->registers[Register::E] = 0xD8;
		this->registers[Register::H] = 0x01;
		this->registers[Register::L] = 0x4D;

		memory.writeByte(0xFF05, 0x00); // TIMA
		memory.writeByte(0xFF06, 0x00); // TMA
		memory.writeByte(0xFF07, 0x00); // TAC
		memory.writeByte(0xFF10, 0x80); // NR10
		memory.writeByte(0xFF11, 0xBF); // NR11
		memory.writeByte(0xFF12, 0xF3); // NR12
		memory.writeByte(0xFF14, 0xBF); // NR14
		memory.writeByte(0xFF16, 0x3F); // NR21
		memory.writeByte(0xFF17, 0x00); // NR22
		memory.writeByte(0xFF19, 0xBF); // NR24
		memory.writeByte(0xFF1A, 0x7F); // NR30
		memory.writeByte(0xFF1B, 0xFF); // NR31
		memory.writeByte(0xFF1C, 0x9F); // NR32
		memory.writeByte(0xFF1E, 0xBF); // NR33
		memory.writeByte(0xFF20, 0xFF); // NR41
		memory.writeByte(0xFF21, 0x00); // NR42
		memory.writeByte(0xFF22, 0x00); // NR43
		memory.writeByte(0xFF23, 0xBF); // NR30
		memory.writeByte(0xFF24, 0x77); // NR50
		memory.writeByte(0xFF25, 0xF3); // NR51
		memory.writeByte(0xFF26, 0xF1); // NR52 (0xF0 = SGB)
		memory.writeByte(0xFF40, 0x91); // LCDC
		memory.writeByte(0xFF42, 0x00); // SCY
		memory.writeByte(0xFF43, 0x00); // SCX
		memory.writeByte(0xFF45, 0x00); // LYC
		memory.writeByte(0xFF47, 0xFC); // BGP
		memory.writeByte(0xFF48, 0xFF); // OBP0
		memory.writeByte(0xFF49, 0xFF); // OBP1
		memory.writeByte(0xFF4A, 0x00); // WY
		memory.writeByte(0xFF4B, 0x00); // WX
		memory.writeByte(0xFFFF, 0x00); // IE
	}

	void CPU::tick()
	{
		if (this->_cycleTimer > 0)
		{
			this->_ticks++;
			this->_cycleTimer -= 4;
			return;
		}

		if (this->ime)
		{
			for (uint8_t bit = 0; bit < 5; ++bit)
			{
				if ((this->_memory.readByte(0xFF0F) & (1 << bit)) && (this->_memory.readByte(0xFFFF) & (1 << bit)))
				{
					this->_memory.writeByte(0xFF0F, this->_memory.readByte(0xFF0F) & ~(1 << bit));
					this->ime = false;
					this->_cycleTimer = 20;
					this->sp -= 2;
					this->_memory.writeWord(this->sp, this->pc);
					this->pc = 0x0040 + bit * 8;
					return;
				}
			}
		}

		if (this->_ei_next_instruction)
		{
			this->_ei_next_instruction = false;
			this->ime = true;
		}

		if (!this->halted)
		{
			uint8_t opcode = this->_memory.readByte(this->pc);
			if (opcode == 0xCB) // PREFIX CB
			{
				this->_cycleTimer += 4;
				opcode = this->_memory.readByte(++this->pc);
				perform_opcode_cb(opcode);
			}
			else
				perform_opcode(opcode);
		}
	}

	void CPU::writeRegister16(Register high, Register low, uint16_t value)
	{
		uint8_t* high_ptr = &(this->registers[static_cast<uint8_t>(high)]);
		uint8_t* low_ptr = &(this->registers[static_cast<uint8_t>(low)]);

		*high_ptr = static_cast<uint8_t>(value >> 8);
		*low_ptr = static_cast<uint8_t>(value & 0xFF);
	}

	uint16_t CPU::readRegister16(Register high, Register low)
	{
		uint8_t high_value = this->registers[static_cast<uint8_t>(high)];
		uint8_t low_value = this->registers[static_cast<uint8_t>(low)];

		return (static_cast<uint16_t>(high_value) << 8) | static_cast<uint16_t>(low_value);
	}

	void CPU::update_timers()
	{
		if (this->_ticks - this->_divLastInc >= CPU::CPU_FREQUENCY / CPU::DIV_FREQUENCY)
		{
			this->_divLastInc = this->_ticks;
			this->_memory.writeByte(0xFF04, this->_memory.readByte(0xFF04) + 1);
			this->_request_interrupt(Interrupt::Timer);
		}

		uint8_t timer_control = this->_memory.readByte(0xFF07);
		if (timer_control & 0b100)
		{
			auto timer_frequency = CPU::CPU_FREQUENCY / (4 << (timer_control & 0b11) * 2);
			if ((timer_control & 0b11) == 0)
				timer_frequency = CPU::CPU_FREQUENCY / 1024;

			if (this->_ticks - this->_timaLastInc >= timer_frequency)
			{
				this->_timaLastInc = this->_ticks;
				uint8_t tima = this->_memory.readByte(0xFF05);
				if (tima == 0xFF)
				{
					this->_memory.writeByte(0xFF05, this->_memory.readByte(0xFF06));
					this->_request_interrupt(Interrupt::Timer);
				}
				else
					this->_memory.writeByte(0xFF05, tima + 1);
			}
		}
	}

	void CPU::_request_interrupt(Interrupt interrupt)
	{
		this->_memory.writeByte(0xFF0F, this->_memory.readByte(0xFF0F) | interrupt);
	}

	bool CPU::isFlagSet(uint8_t flag) { return (this->registers[static_cast<uint8_t>(Register::F)] & flag) != 0; }
}
