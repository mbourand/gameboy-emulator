#include "CPU.hpp"

namespace gbmu
{
	void CPU::perform_opcode(uint8_t opcode)
	{
		switch (opcode)
		{
			/* clang-format off */
			case 0x00: // NOP
				this->nop();
				break;
			case 0x01: case 0x11: case 0x21: // LD reg16, u16
				this->ld_reg16_u16(opcode);
				break;
			case 0x31: // LD SP, u16
				this->ld_sp_u16();
				break;
			case 0x03: // INC BC
				this->inc_bc();
				break;
			case 0x07: // RLCA (Rotate left through carry accumulator)
				this->rlca();
				break;
			case 0x08: // LD (u16), SP
				this->ld_u16_sp();
				break;
			case 0x09: // ADD HL, BC
				this->add_hl_bc();
				break;
			case 0x20: // JR NZ, i8
				this->jr_cond_i8(!this->isFlagSet(FLAG_ZERO));
				break;
			case 0x30: // JR NC, i8
				this->jr_cond_i8(!this->isFlagSet(FLAG_CARRY));
				break;
			case 0xC3: // JP u16
				this->jp_u16();
				break;
			case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x47: // LD B, reg
			case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4F: // LD C, reg
			case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x57: // LD D, reg
			case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5F: // LD E, reg
			case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x67: // LD H, reg
			case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6F: // LD L, reg
			case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D: case 0x7F: // LD A, reg
				this->ld_reg_reg(opcode);
				break;
			case 0x02: // LD (BC), A
				this->ld_reg16_a(Register::B, Register::C);
				break;
			case 0x12: // LD (DE), A
				this->ld_reg16_a(Register::D, Register::E);
				break;
			case 0x22: // LD (HL+), A
				this->ld_inc_hl_a();
				break;
			case 0x32: // LD (HL-), A
				this->ld_dec_hl_a();
				break;
			case 0x0B: case 0x1B: case 0x2B: case 0x3B: // DEC reg16
				this->dec_reg16(opcode);
				break;
			case 0x0C: case 0x1C: case 0x2C: case 0x3C: case 0x04: case 0x14: case 0x24: // INC reg
				this->inc_reg(opcode);
				break;
			case 0x0D: case 0x1D: case 0x2D: case 0x3D: case 0x05: case 0x15: case 0x25: // DEC reg
				this->dec_reg(opcode);
				break;
			case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x77: // LD (HL), reg
				this->ld_hl_reg(opcode);
				break;
			case 0x4E: case 0x5E: case 0x6E: case 0x7E: // LD reg, (HL)
				this->ld_reg_hl(opcode);
				break;
			case 0x06: case 0x16: case 0x26: case 0x0E: case 0x1E: case 0x2E: case 0x3E: // LD reg, (u8)
				this->ld_reg_u8(opcode);
				break;
			case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x87: // ADD A, reg
				this->add_a_reg(opcode);
				break;
			case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8F: // ADC A, reg
				this->adc_a_reg(opcode);
				break;
			case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x97: // SUB A, reg
				this->sub_a_reg(opcode);
				break;
			case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9F: // SBC A, reg
				this->sbc_a_reg(opcode);
				break;
			case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA7: // AND A, reg
				this->and_a_reg(opcode);
				break;
			case 0xA8: case 0xA9: case 0xAA: case 0xAB: case 0xAC: case 0xAD: case 0xAF: // XOR A, reg
				this->xor_a_reg(opcode);
				break;
			case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB7: // OR A, reg
				this->or_a_reg(opcode);
				break;
			case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBF: // CP A, reg
				this->cp_a_reg(opcode);
				break;
			case 0xF3: case 0xFB: // DI
				this->di_ei(opcode);
				break;
				/* clang-format on */
			default: // Unknown opcode
				this->pc++;
				break;
		}
	}

	void CPU::nop() { this->pc++; }

	void CPU::ld_reg16_u16(int opcode)
	{
		Register high = static_cast<Register>((opcode >> 4 & 0b11) * 2);
		Register low = static_cast<Register>(high + 1);
		uint16_t value = this->_memory.readWord(this->pc + 1);
		this->writeRegister16(high, low, value);
		this->pc += 3;
	}

	void CPU::ld_sp_u16()
	{
		this->sp = this->_memory.readWord(this->pc + 1);
		this->pc += 3;
	}

	void CPU::inc_bc()
	{
		this->writeRegister16(Register::B, Register::C, this->readRegister16(Register::B, Register::C) + 1);
		this->pc++;
	}

	void CPU::rlca()
	{
		this->_registers[Register::F] = (this->_registers[Register::A] & 0x80) ? FLAG_CARRY : 0;
		this->_registers[Register::A] = (this->_registers[Register::A] << 1) | (this->_registers[Register::A] >> 7);
		this->pc++;
	}

	void CPU::ld_u16_sp()
	{
		this->_memory.writeWord(this->_memory.readWord(this->pc + 1), this->sp);
		this->pc += 3;
	}

	void CPU::add_hl_bc()
	{
		uint16_t hl = this->readRegister16(Register::H, Register::L),
				 bc = this->readRegister16(Register::B, Register::C);
		uint8_t h = this->_registers[Register::H], b = this->_registers[Register::B];
		this->_registers[Register::F] |=
			((h & 0xF) + (b & 0xF) & 0x10 ? FLAG_HALF_CARRY : 0) | (h + b > 0xFF ? FLAG_CARRY : 0);
		this->writeRegister16(Register::H, Register::L, hl + bc);
		this->pc++;
	}

	void CPU::ld_reg_reg(uint8_t opcode)
	{
		this->_registers[opcode >> 3 & 0b111] = this->_registers[opcode & 0b111];
		this->pc++;
	}

	void CPU::add_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->_registers[Register::A], src = this->_registers[opcode & 0b111];
		this->_registers[Register::A] += src;
		this->_registers[Register::F] = ((dest & 0xF) + (src & 0xF) & 0x10 ? FLAG_HALF_CARRY : 0) |
										(dest + src > 0xFF ? FLAG_CARRY : 0) | (dest + src == 0 ? FLAG_ZERO : 0);
		this->pc++;
	}

	void CPU::adc_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->_registers[Register::A], src = this->_registers[opcode & 0b111];
		this->_registers[Register::A] += src + this->isFlagSet(FLAG_CARRY);
		this->_registers[Register::F] = ((dest & 0xF) + (src & 0xF) & 0x10 ? FLAG_HALF_CARRY : 0) |
										(dest + src + this->isFlagSet(FLAG_CARRY) > 0xFF ? FLAG_CARRY : 0) |
										(this->_registers[Register::A] == 0 ? FLAG_ZERO : 0);
		this->pc++;
	}

	void CPU::sub_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->_registers[Register::A], src = this->_registers[opcode & 0b111];
		this->_registers[Register::A] -= src;
		this->_registers[Register::F] = ((dest & 0xF) - (src & 0xF) & 0x10 ? FLAG_HALF_CARRY : 0) |
										(dest < src ? FLAG_CARRY : 0) | (dest == src ? FLAG_ZERO : 0) | FLAG_NEGATIVE;
		this->pc++;
	}

	void CPU::sbc_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->_registers[Register::A], src = this->_registers[opcode & 0b111];
		this->_registers[Register::F] =
			FLAG_NEGATIVE | (dest == src + this->isFlagSet(FLAG_CARRY) ? FLAG_ZERO : 0) |
			((dest & 0xF) - (src + this->isFlagSet(FLAG_CARRY) & 0xF) & 0x10 ? FLAG_HALF_CARRY : 0) |
			(dest < src + this->isFlagSet(FLAG_CARRY) ? FLAG_CARRY : 0);
		this->_registers[Register::A] -= src + ((this->_registers[Register::F] & FLAG_CARRY) > 0);
		this->pc++;
	}

	void CPU::and_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->_registers[Register::A], src = this->_registers[opcode & 0b111];
		this->_registers[Register::F] = ((dest & src) == 0 ? FLAG_ZERO : 0) | FLAG_HALF_CARRY;
		this->_registers[Register::A] &= this->_registers[opcode & 0b111];
		this->pc++;
	}

	void CPU::xor_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->_registers[Register::A], src = this->_registers[opcode & 0b111];
		this->_registers[Register::F] = ((dest ^ src) == 0 ? FLAG_ZERO : 0);
		this->_registers[Register::A] ^= this->_registers[opcode & 0b111];
		this->pc++;
	}

	void CPU::or_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->_registers[Register::A], src = this->_registers[opcode & 0b111];
		this->_registers[Register::F] = ((dest ^ src) == 0 ? FLAG_ZERO : 0);
		this->_registers[Register::A] |= this->_registers[opcode & 0b111];
		this->pc++;
	}

	void CPU::cp_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->_registers[Register::A], src = this->_registers[opcode & 0b111];
		this->_registers[Register::F] = ((dest & 0xF) - (src & 0xF) & 0x10 ? FLAG_HALF_CARRY : 0) |
										(dest < src ? FLAG_CARRY : 0) | (dest == src ? FLAG_ZERO : 0) | FLAG_NEGATIVE;
		this->pc++;
	}

	void CPU::jr_cond_i8(bool cond)
	{
		if (cond)
			this->pc += static_cast<int8_t>(this->_memory.readByte(this->pc + 1));
		this->pc += 2;
	}

	void CPU::ld_hl_reg(int opcode)
	{
		this->_memory.writeByte(this->_registers[Register::H] << 8 | this->_registers[Register::L],
								this->_registers[opcode & 0b111]);
		this->pc++;
	}

	void CPU::ld_reg_hl(int opcode)
	{
		this->_registers[opcode >> 3 & 0b111] =
			this->_memory.readByte(this->_registers[Register::H] << 8 | this->_registers[Register::L]);
		this->pc++;
	}

	void CPU::ld_reg_u8(int opcode)
	{
		this->_registers[opcode + 0x40 >> 3 & 0b111] = this->_memory.readByte(this->pc + 1);
		this->pc += 2;
	}

	void CPU::dec_reg(int opcode)
	{
		int fourth_bit = this->_registers[opcode + 0x40 >> 3 & 0b111] & (1 << 4);
		this->_registers[opcode + 0x40 >> 3 & 0b111]--;
		int fourth_bit_after = this->_registers[opcode + 0x40 >> 3 & 0b111] & (1 << 4);

		this->_registers[Register::F] = (this->_registers[opcode + 0x40 >> 3 & 0b111] == 0 ? FLAG_ZERO : 0) |
										FLAG_NEGATIVE | this->isFlagSet(FLAG_CARRY) |
										(fourth_bit != fourth_bit_after ? FLAG_HALF_CARRY : 0);
		;
		this->pc++;
	}

	void CPU::inc_reg(int opcode)
	{
		int fourth_bit = this->_registers[opcode + 0x40 >> 3 & 0b111] & (1 << 4);
		this->_registers[opcode + 0x40 >> 3 & 0b111]++;
		int fourth_bit_after = this->_registers[opcode + 0x40 >> 3 & 0b111] & (1 << 4);

		this->_registers[Register::F] = (this->_registers[opcode + 0x40 >> 3 & 0b111] == 0 ? FLAG_ZERO : 0) |
										this->isFlagSet(FLAG_CARRY) |
										(fourth_bit != fourth_bit_after ? FLAG_HALF_CARRY : 0);
		;
		this->pc++;
	}

	void CPU::dec_reg16(int opcode)
	{
		uint16_t val = this->readRegister16(static_cast<Register>((opcode + 0x40 >> 3 & 0b111) - 1),
											static_cast<Register>(opcode + 0x40 >> 3 & 0b111));
		this->writeRegister16(static_cast<Register>((opcode + 0x40 >> 3 & 0b111) - 1),
							  static_cast<Register>(opcode + 0x40 >> 3 & 0b111), --val);
		this->pc++;
	}

	void CPU::jp_u16()
	{
		this->pc = this->_memory.readWord(this->pc + 1);
		// this->cycleTimer += 4;
	}

	void CPU::ld_dec_hl_a()
	{
		uint16_t addr = this->readRegister16(Register::H, Register::L);
		this->_memory.writeByte(addr, this->_registers[Register::A]);
		this->writeRegister16(Register::H, Register::L, addr - 1);
		this->pc++;
	}

	void CPU::ld_inc_hl_a()
	{
		uint16_t addr = this->readRegister16(Register::H, Register::L);
		this->_memory.writeByte(addr, this->_registers[Register::A]);
		this->writeRegister16(Register::H, Register::L, addr + 1);
		this->pc++;
	}

	void CPU::ld_reg16_a(Register high, Register low)
	{
		uint16_t addr = this->readRegister16(high, low);
		this->_memory.writeByte(addr, this->_registers[Register::A]);
		this->pc++;
	}

	void CPU::di_ei(int opcode)
	{
		this->enable_interrupts = (opcode & 0xF) == 0xB;
		this->pc++;
	}

	std::ostream& operator<<(std::ostream& os, const CPU& cpu)
	{
		os << std::hex << std::setw(4) << std::setfill('0') << cpu.pc << ": ";
		os << "A:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu._registers[CPU::Register::A])
		   << " "
		   << "F:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu._registers[CPU::Register::F])
		   << " "
		   << "B:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu._registers[CPU::Register::B])
		   << " "
		   << "C:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu._registers[CPU::Register::C])
		   << " "
		   << "D:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu._registers[CPU::Register::D])
		   << " "
		   << "E:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu._registers[CPU::Register::E])
		   << " "
		   << "H:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu._registers[CPU::Register::H])
		   << " "
		   << "L:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu._registers[CPU::Register::L])
		   << " "
		   << "SP:" << std::setw(4) << std::setfill('0') << static_cast<uint16_t>(cpu.sp);
		return os;
	}
}
