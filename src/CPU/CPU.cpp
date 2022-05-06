#include "CPU.hpp"

namespace gbmu
{
	CPU::CPU(Memory& memory)
		: _memory(memory), sp(CPU::ENTRY_STACK_POINTER), pc(CPU::ENTRY_POINT), cycleTimer(0), totalCycles(0),
		  enable_interrupts(true)
	{
		this->_registers.fill(0);
	}

	void CPU::tick()
	{
		if (this->cycleTimer > 0)
		{
			--this->cycleTimer;
			this->totalCycles++;
			return;
		}

		uint8_t opcode = _memory.readByte(pc);
		perform_opcode(opcode);

		this->totalCycles++;
	}

	void CPU::writeRegister16(Register high, Register low, uint16_t value)
	{
		uint8_t* high_ptr = &(this->_registers[static_cast<uint8_t>(high)]);
		uint8_t* low_ptr = &(this->_registers[static_cast<uint8_t>(low)]);

		*high_ptr = static_cast<uint8_t>(value >> 8);
		*low_ptr = static_cast<uint8_t>(value & 0xFF);
	}

	uint16_t CPU::readRegister16(Register high, Register low)
	{
		uint8_t high_value = this->_registers[static_cast<uint8_t>(high)];
		uint8_t low_value = this->_registers[static_cast<uint8_t>(low)];

		return (static_cast<uint16_t>(high_value) << 8) | static_cast<uint16_t>(low_value);
	}

	bool CPU::isFlagSet(uint8_t flag) { return (this->_registers[static_cast<uint8_t>(Register::F)] & flag) != 0; }
}
