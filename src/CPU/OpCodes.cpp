#include "CPU.hpp"
#include "Gameboy.hpp"

namespace gbmu
{
	void CPU::perform_opcode(uint8_t opcode)
	{
		/* clang-format off */
		switch (opcode)
		{
		case 0x00: // NOP
			this->nop(); break;
		case 0x01: case 0x11: case 0x21: // LD reg16, u16
			this->ld_reg16_u16(opcode); break;
		case 0x31: // LD SP, u16
			this->ld_sp_u16(); break;
		case 0x07: // RLCA
			this->rlca(); break;
		case 0x17: // RLA
			this->rla(); break;
		case 0x0F: // RRCA
			this->rrca(); break;
		case 0x1F: // RRA
			this->rra(); break;
		case 0x08: // LD (u16), SP
			this->ld_u16_sp(); break;
		case 0x09: case 0x19: case 0x29: // ADD HL, reg16
			this->add_hl_reg16(opcode); break;
		case 0x39: // ADD HL, SP
			this->add_hl_sp(opcode); break;
		case 0x20: // JR NZ, i8
			this->jr_cond_i8(!this->isFlagSet(FLAG_ZERO)); break;
		case 0x30: // JR NC, i8
			this->jr_cond_i8(!this->isFlagSet(FLAG_CARRY)); break;
		case 0x18: // JR Z, i8
			this->jr_cond_i8(true); break;
		case 0x28: // JR Z, i8
			this->jr_cond_i8(this->isFlagSet(FLAG_ZERO)); break;
		case 0x38: // JR C, i8
			this->jr_cond_i8(this->isFlagSet(FLAG_CARRY)); break;
		case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x47: // LD B, reg
		case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4F: // LD C, reg
		case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x57: // LD D, reg
		case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5F: // LD E, reg
		case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x67: // LD H, reg
		case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6F: // LD L, reg
		case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D: case 0x7F: // LD A, reg
			this->ld_reg_reg(opcode); break;
		case 0x02: // LD (BC), A
			this->ld_reg16_a(Reg::B, Reg::C); break;
		case 0x12: // LD (DE), A
			this->ld_reg16_a(Reg::D, Reg::E); break;
		case 0x22: // LD (HL+), A
			this->ld_inc_hl_a(); break;
		case 0x32: // LD (HL-), A
			this->ld_dec_hl_a(); break;
		case 0x0A: // LD A, (BC)
			this->ld_a_reg16(Reg::B, Reg::C); break;
		case 0x1A: // LD A, (DE)
			this->ld_a_reg16(Reg::D, Reg::E); break;
		case 0x2A: // LD A, (HL+)
			this->ld_a_inc_hl(); break;
		case 0x3A: // LD A, (HL-)
			this->ld_a_dec_hl(); break;
		case 0x0B: case 0x1B: case 0x2B: // DEC reg16
			this->dec_reg16(opcode); break;
		case 0x3B: // DEC SP
			this->dec_sp(); break;
		case 0x0C: case 0x1C: case 0x2C: case 0x3C: case 0x04: case 0x14: case 0x24: // INC reg
			this->inc_reg(opcode); break;
		case 0x34: // INC (HL)
			this->inc_hl(); break;
		case 0x35: // DEC (HL)
			this->dec_hl(); break;
		case 0x0D: case 0x1D: case 0x2D: case 0x3D: case 0x05: case 0x15: case 0x25: // DEC reg
			this->dec_reg(opcode); break;
		case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x77: // LD (HL), reg
			this->ld_hl_reg(opcode); break;
		case 0x46: case 0x56: case 0x66: case 0x4E: case 0x5E: case 0x6E: case 0x7E: // LD reg, (HL)
			this->ld_reg_hl(opcode); break;
		case 0x06: case 0x16: case 0x26: case 0x0E: case 0x1E: case 0x2E: case 0x3E: // LD reg, (u8)
			this->ld_reg_u8(opcode); break;
		case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x87: // ADD A, reg
			this->add_a_reg(opcode); break;
		case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8F: // ADC A, reg
			this->adc_a_reg(opcode); break;
		case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x97: // SUB A, reg
			this->sub_a_reg(opcode); break;
		case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9F: // SBC A, reg
			this->sbc_a_reg(opcode); break;
		case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA7: // AND A, reg
			this->and_a_reg(opcode); break;
		case 0xA8: case 0xA9: case 0xAA: case 0xAB: case 0xAC: case 0xAD: case 0xAF: // XOR A, reg
			this->xor_a_reg(opcode); break;
		case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB7: // OR A, reg
			this->or_a_reg(opcode); break;
		case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBF: // CP A, reg
			this->cp_a_reg(opcode); break;
		case 0x86: // ADD A, (HL)
			this->add_a_hl(); break;
		case 0x8E: // ADC A, (HL)
			this->adc_a_hl(); break;
		case 0x96: // SUB A, (HL)
			this->sub_a_hl(); break;
		case 0x9E: // SBC A, (HL)
			this->sbc_a_hl(); break;
		case 0xA6: // AND A, (HL)
			this->and_a_hl(); break;
		case 0xAE: // XOR A, (HL)
			this->xor_a_hl(); break;
		case 0xB6: // OR A, (HL)
			this->or_a_hl(); break;
		case 0xBE: // CP A, (HL)
			this->cp_a_hl(); break;
		case 0xF3: case 0xFB: // DI EI
			this->di_ei(opcode); break;
		case 0xE0: // LDH (u8), A
			this->ldh_u8_a(); break;
		case 0xE2: // LDH (C), A
			this->ldh_c_a(); break;
		case 0xF0: // LDH A, (u8)
			this->ldh_a_u8(); break;
		case 0xF2: // LDH A, (C)
			this->ldh_a_c(); break;
		case 0xC6: // ADD A, u8
			this->add_a_u8(); break;
		case 0xD6: // SUB A, u8
			this->sub_a_u8(); break;
		case 0xE6: // AND A, u8
			this->and_a_u8(); break;
		case 0xF6: // OR A, u8
			this->or_a_u8(); break;
		case 0xCE: // ADC A, (u8)
			this->adc_a_u8(); break;
		case 0xDE: // SBC A, (u8)
			this->sbc_a_u8(); break;
		case 0xEE: // XOR A, (u8)
			this->xor_a_u8(); break;
		case 0xFE: // CP A, (u8)
			this->cp_a_u8(); break;
		case 0xC2: // JP NZ, u16
			this->jp_cond_u16(!this->isFlagSet(FLAG_ZERO)); break;
		case 0xE9: // JP (HL)
			this->jp_hl(); break;
		case 0xC3: // JP u16
			this->jp_cond_u16(true); break;
		case 0xD2: // JP NC, u16
			this->jp_cond_u16(!this->isFlagSet(FLAG_CARRY)); break;
		case 0xCA: // JP Z, u16
			this->jp_cond_u16(this->isFlagSet(FLAG_ZERO)); break;
		case 0xDA: // JP C, u16
			this->jp_cond_u16(this->isFlagSet(FLAG_CARRY)); break;
		case 0xC4: // CALL NZ, u16
			this->call_cond_u16(!this->isFlagSet(FLAG_ZERO)); break;
		case 0xD4: // CALL NC, u16
			this->call_cond_u16(!this->isFlagSet(FLAG_CARRY)); break;
		case 0xCC: // CALL Z, u16
			this->call_cond_u16(this->isFlagSet(FLAG_ZERO)); break;
		case 0xDC: // CALL C, u16
			this->call_cond_u16(this->isFlagSet(FLAG_CARRY)); break;
		case 0xCD: // CALL u16
			this->call_cond_u16(true); break;
		case 0xC9: // RET
			this->ret(); break;
		case 0xC0: // RET NZ
			this->ret_cond(!this->isFlagSet(FLAG_ZERO)); break;
		case 0xD0: // RET NC
			this->ret_cond(!this->isFlagSet(FLAG_CARRY)); break;
		case 0xC8: // RET Z
			this->ret_cond(this->isFlagSet(FLAG_ZERO)); break;
		case 0xD8: // RET C
			this->ret_cond(this->isFlagSet(FLAG_CARRY)); break;
		case 0xD9: // RETI
			this->reti(); break;
		case 0xC7: case 0xCF: case 0xD7: case 0xDF: case 0xE7: case 0xEF: case 0xF7: case 0xFF: // RST u8
			this->rst(opcode); break;
		case 0xC5: case 0xD5: case 0xE5: case 0xF5: // PUSH reg16
			this->push_reg16(opcode); break;
		case 0xC1: case 0xD1: case 0xE1: case 0xF1: // POP reg16
			this->pop_reg16(opcode); break;
		case 0x03: case 0x13: case 0x23: // INC reg16
			this->inc_reg16(opcode); break;
		case 0x33: // INC SP
			this->inc_sp(); break;
		case 0xE8: // ADD SP, i8
			this->add_sp_i8(); break;
		case 0xF8: // LD HL, SP+i8
			this->ld_hl_sp_i8(); break;
		case 0xF9: // LD SP, HL
			this->ld_sp_hl(); break;
		case 0xEA: // LD (u16), A
			this->ld_u16_a(); break;
		case 0xFA: // LD A, (u16)
			this->ld_a_u16(); break;
		case 0x36: // LD (HL), u8
			this->ld_hl_u8(); break;
		case 0x2F: // CPL
			this->cpl(); break;
		case 0x3F: // CCF
			this->ccf(); break;
		case 0x37: // SCF
			this->scf(); break;
		case 0x27: // DAA
			this->daa(); break;
		case 0x76: // HALT
			this->halt(); break;
		case 0x10: // STOP
			this->stop(); break;
		default: // Unknown opcode
			this->pc++;
			break;
		}
		/* clang-format on */
	}

	void CPU::perform_opcode_cb(uint8_t opcode)
	{
		/* clang-format off */
		switch (opcode)
		{
			// http://jgmalcolm.com/z80/advanced/shif#sla
			case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x07: // RLC reg8
				this->rlc_reg8(static_cast<Reg>(opcode & 0b111)); break;
			case 0x06: // RLC (HL)
				this->rlc_hl(); break;
			case 0x08: case 0x09: case 0x0A: case 0x0B: case 0x0C: case 0x0D: case 0x0F: // RRC reg8
				this->rrc_reg8(static_cast<Reg>(opcode & 0b111)); break;
			case 0x0E: // RRC (HL)
				this->rrc_hl(); break;
			case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x17: // RL reg8
				this->rl_reg8(static_cast<Reg>(opcode & 0b111)); break;
			case 0x16: // RL (HL)
				this->rl_hl(); break;
			case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1F: // RR reg8
				this->rr_reg8(static_cast<Reg>(opcode & 0b111)); break;
			case 0x1E: // RR (HL)
				this->rr_hl(); break;
			case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x27: // SLA reg8
				this->sla_reg8(static_cast<Reg>(opcode & 0b111)); break;
			case 0x26: // SLA (HL)
				this->sla_hl(); break;
			case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D: case 0x2F: // SRA reg8
				this->sra_reg8(static_cast<Reg>(opcode & 0b111)); break;
			case 0x2E: // SRA (HL)
				this->sra_hl(); break;
			case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x37: // SEAP reg8
				this->swap_reg8(static_cast<Reg>(opcode & 0b111)); break;
			case 0x36: // SWAP (HL)
				this->swap_hl(); break;
			case 0x38: case 0x39: case 0x3A: case 0x3B: case 0x3C: case 0x3D: case 0x3F: // SRL reg8
				this->srl_reg8(static_cast<Reg>(opcode & 0b111)); break;
			case 0x3E: // SRL (HL)
				this->srl_hl(); break;
			case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x47: // BIT 0, reg8
			case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4F: // BIT 1, reg8
			case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x57: // BIT 2, reg8
			case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5F: // BIT 3, reg8
			case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x67: // BIT 4, reg8
			case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6F: // BIT 5, reg8
			case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x77: // BIT 6, reg8
			case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D: case 0x7F: // BIT 7, reg8
				this->bit_reg8((opcode - 0x40) / 8, static_cast<Reg>(opcode & 0b111)); break;
			case 0x46: case 0x4E: case 0x56: case 0x5E: case 0x66: case 0x6E: case 0x76: case 0x7E: // BIT n, (HL)
				this->bit_hl((opcode - 0x46) / 8); break;
			case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x87: // RES 0, reg8
			case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8F: // RES 1, reg8
			case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x97: // RES 2, reg8
			case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9F: // RES 3, reg8
			case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA7: // RES 4, reg8
			case 0xA8: case 0xA9: case 0xAA: case 0xAB: case 0xAC: case 0xAD: case 0xAF: // RES 5, reg8
			case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB7: // RES 6, reg8
			case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBF: // RES 7, reg8
				this->res_reg8((opcode - 0x80) / 8, static_cast<Reg>(opcode & 0b111)); break;
			case 0x86: case 0x8E: case 0x96: case 0x9E: case 0xA6: case 0xAE: case 0xB6: case 0xBE: // RES n, (HL)
				this->res_hl((opcode - 0x86) / 8); break;
			case 0xC0: case 0xC1: case 0xC2: case 0xC3: case 0xC4: case 0xC5: case 0xC7: // SET 0, reg8
			case 0xC8: case 0xC9: case 0xCA: case 0xCB: case 0xCC: case 0xCD: case 0xCF: // SET 1, reg8
			case 0xD0: case 0xD1: case 0xD2: case 0xD3: case 0xD4: case 0xD5: case 0xD7: // SET 2, reg8
			case 0xD8: case 0xD9: case 0xDA: case 0xDB: case 0xDC: case 0xDD: case 0xDF: // SET 3, reg8
			case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE4: case 0xE5: case 0xE7: // SET 4, reg8
			case 0xE8: case 0xE9: case 0xEA: case 0xEB: case 0xEC: case 0xED: case 0xEF: // SET 5, reg8
			case 0xF0: case 0xF1: case 0xF2: case 0xF3: case 0xF4: case 0xF5: case 0xF7: // SET 6, reg8
			case 0xF8: case 0xF9: case 0xFA: case 0xFB: case 0xFC: case 0xFD: case 0xFF: // SET 7, reg8
				this->set_reg8((opcode - 0xC0) / 8, static_cast<Reg>(opcode & 0b111)); break;
			case 0xC6: case 0xCE: case 0xD6: case 0xDE: case 0xE6: case 0xEE: case 0xF6: case 0xFE: // SET n, (HL)
				this->set_hl((opcode - 0xC6) / 8); break;
		}
		/* clang-format on */
	}

	void CPU::swap_reg8(Reg reg)
	{
		this->registers[reg] = (this->registers[reg] >> 4) | (this->registers[reg] << 4);
		this->registers[Reg::F] = (this->registers[reg] == 0 ? FLAG_ZERO : 0);
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::bit_reg8(uint8_t bit, Reg reg)
	{
		this->registers[Reg::F] =
			((this->registers[reg] & (1 << bit)) == 0 ? FLAG_ZERO : 0) | FLAG_HALF_CARRY | this->getFlag(FLAG_CARRY);
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::set_reg8(uint8_t bit, Reg reg)
	{
		this->registers[reg] |= (1 << bit);
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::res_reg8(uint8_t bit, Reg reg)
	{
		this->registers[reg] &= ~(1 << bit);
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::rlc_reg8(Reg reg)
	{
		this->registers[Reg::F] = (this->registers[reg] & 0x80) ? FLAG_CARRY : 0;
		this->registers[reg] = (this->registers[reg] << 1) | (this->registers[reg] >> 7);
		this->registers[Reg::F] |= (this->registers[reg] == 0) ? FLAG_ZERO : 0;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::rrc_reg8(Reg reg)
	{
		this->registers[Reg::F] = (this->registers[reg] & 0x01) ? FLAG_CARRY : 0;
		this->registers[reg] = (this->registers[reg] >> 1) | (this->registers[reg] << 7);
		this->registers[Reg::F] |= (this->registers[reg] == 0) ? FLAG_ZERO : 0;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::rl_reg8(Reg reg)
	{
		bool carry = this->isFlagSet(FLAG_CARRY);
		this->registers[Reg::F] = (this->registers[reg] & 0x80) ? FLAG_CARRY : 0;
		this->registers[reg] = (this->registers[reg] << 1) | carry;
		this->registers[Reg::F] |= (this->registers[reg] == 0) ? FLAG_ZERO : 0;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::rr_reg8(Reg reg)
	{
		bool carry = this->isFlagSet(FLAG_CARRY);
		this->registers[Reg::F] = (this->registers[reg] & 0x1) ? FLAG_CARRY : 0;
		this->registers[reg] = (this->registers[reg] >> 1) | (carry << 7);
		this->registers[Reg::F] |= (this->registers[reg] == 0) ? FLAG_ZERO : 0;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::sla_reg8(Reg reg)
	{
		this->registers[Reg::F] = (this->registers[reg] & 0x80) ? FLAG_CARRY : 0;
		this->registers[reg] <<= 1;
		this->registers[Reg::F] |= (this->registers[reg] == 0) ? FLAG_ZERO : 0;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::sra_reg8(Reg reg)
	{
		this->registers[Reg::F] = (this->registers[reg] & 0x01) ? FLAG_CARRY : 0;
		this->registers[reg] = (this->registers[reg] >> 1) | (this->registers[reg] & 0x80);
		this->registers[Reg::F] |= (this->registers[reg] == 0) ? FLAG_ZERO : 0;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::srl_reg8(Reg reg)
	{
		this->registers[Reg::F] = (this->registers[reg] & 0x01) ? FLAG_CARRY : 0;
		this->registers[reg] >>= 1;
		this->registers[Reg::F] |= (this->registers[reg] == 0) ? FLAG_ZERO : 0;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::nop()
	{
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::ld_reg16_u16(int opcode)
	{
		Reg high = static_cast<Reg>((opcode >> 4 & 0b11) * 2);
		Reg low = static_cast<Reg>(high + 1);
		uint16_t value = this->_gb.readWord(this->pc + 1);
		this->writeRegister16(high, low, value);
		this->pc += 3;
		this->_cycleTimer += 12;
	}

	void CPU::ld_sp_u16()
	{
		this->sp = this->_gb.readWord(this->pc + 1);
		this->pc += 3;
		this->_cycleTimer += 12;
	}

	void CPU::inc_reg16(int opcode)
	{
		Reg high = static_cast<Reg>((opcode - 0x03) / 16 * 2);
		Reg low = static_cast<Reg>(high + 1);
		this->writeRegister16(high, low, this->readRegister16(high, low) + 1);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::inc_sp()
	{
		this->sp++;
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::rlca()
	{
		this->registers[Reg::F] = (this->registers[Reg::A] & 0x80) ? FLAG_CARRY : 0;
		this->registers[Reg::A] = (this->registers[Reg::A] << 1) | (this->registers[Reg::A] >> 7);
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::rrca()
	{
		this->registers[Reg::F] = (this->registers[Reg::A] & 0x01) ? FLAG_CARRY : 0;
		this->registers[Reg::A] = (this->registers[Reg::A] >> 1) | (this->registers[Reg::A] << 7);
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::rla()
	{
		bool carry = this->isFlagSet(FLAG_CARRY);
		this->registers[Reg::F] = (this->registers[Reg::A] & 0x80) ? FLAG_CARRY : 0;
		this->registers[Reg::A] <<= 1;
		this->registers[Reg::A] |= carry;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::rra()
	{
		bool carry = this->isFlagSet(FLAG_CARRY);
		this->registers[Reg::F] = (this->registers[Reg::A] & 0x01) ? FLAG_CARRY : 0;
		this->registers[Reg::A] >>= 1;
		this->registers[Reg::A] |= (carry << 7);
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::ld_u16_sp()
	{
		this->_gb.writeWord(this->_gb.readWord(this->pc + 1), this->sp);
		this->pc += 3;
		this->_cycleTimer += 20;
	}

	void CPU::ld_reg_reg(uint8_t opcode)
	{
		this->registers[opcode >> 3 & 0b111] = this->registers[opcode & 0b111];
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::add_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->registers[Reg::A], src = this->registers[opcode & 0b111];
		this->registers[Reg::A] += src;
		this->registers[Reg::F] = ((dest & 0xF) + (src & 0xF) > 0xF ? FLAG_HALF_CARRY : 0) |
								  (static_cast<uint16_t>(dest) + src > 0xFF ? FLAG_CARRY : 0) |
								  (this->registers[Reg::A] == 0 ? FLAG_ZERO : 0);
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::add_a_hl()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->readRegister16(Reg::H, Reg::L));
		this->registers[Reg::A] += src;
		this->registers[Reg::F] = ((dest & 0xF) + (src & 0xF) > 0xF ? FLAG_HALF_CARRY : 0) |
								  (static_cast<uint16_t>(dest) + src > 0xFF ? FLAG_CARRY : 0) |
								  (this->registers[Reg::A] == 0 ? FLAG_ZERO : 0);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::sub_a_hl()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->readRegister16(Reg::H, Reg::L));
		this->registers[Reg::A] -= src;
		this->registers[Reg::F] = ((dest & 0xF) - (src & 0xF) & 0x10 ? FLAG_HALF_CARRY : 0) |
								  (dest < src ? FLAG_CARRY : 0) | (dest == src ? FLAG_ZERO : 0) | FLAG_NEGATIVE;
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::and_a_hl()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->readRegister16(Reg::H, Reg::L));
		this->registers[Reg::F] = ((dest & src) == 0 ? FLAG_ZERO : 0) | FLAG_HALF_CARRY;
		this->registers[Reg::A] &= src;
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::or_a_hl()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->readRegister16(Reg::H, Reg::L));
		this->registers[Reg::F] = ((dest | src) == 0 ? FLAG_ZERO : 0);
		this->registers[Reg::A] |= src;
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::adc_a_hl()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->readRegister16(Reg::H, Reg::L));
		this->registers[Reg::A] = this->registers[Reg::A] + src + this->isFlagSet(FLAG_CARRY);
		int res = dest + src + this->isFlagSet(FLAG_CARRY);
		int halfRes = (dest & 0xF) + (src & 0xF) + this->isFlagSet(FLAG_CARRY);

		this->registers[Reg::F] = (halfRes > 0xF ? FLAG_HALF_CARRY : 0) | (res > 0xFF ? FLAG_CARRY : 0) |
								  (this->registers[Reg::A] == 0 ? FLAG_ZERO : 0);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::sbc_a_hl()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->readRegister16(Reg::H, Reg::L));
		this->registers[Reg::A] = this->registers[Reg::A] - src - this->isFlagSet(FLAG_CARRY);

		int signedRes = static_cast<int>(dest) - src - this->isFlagSet(FLAG_CARRY);
		int halfSignedRes = static_cast<int>(dest & 0xF) - (src & 0xF) - (this->isFlagSet(FLAG_CARRY));

		this->registers[Reg::F] = FLAG_NEGATIVE | (this->registers[Reg::A] == 0 ? FLAG_ZERO : 0) |
								  (halfSignedRes < 0 ? FLAG_HALF_CARRY : 0) | (signedRes < 0 ? FLAG_CARRY : 0);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::xor_a_hl()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->readRegister16(Reg::H, Reg::L));
		this->registers[Reg::F] = ((dest ^ src) == 0 ? FLAG_ZERO : 0);
		this->registers[Reg::A] ^= src;
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::cp_a_hl()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->readRegister16(Reg::H, Reg::L));
		this->registers[Reg::F] = ((dest & 0xF) - (src & 0xF) & 0x10 ? FLAG_HALF_CARRY : 0) |
								  (dest < src ? FLAG_CARRY : 0) | (dest == src ? FLAG_ZERO : 0) | FLAG_NEGATIVE;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::adc_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->registers[Reg::A], src = this->registers[opcode & 0b111];
		this->registers[Reg::A] += src + this->isFlagSet(FLAG_CARRY);
		int res = dest + src + this->isFlagSet(FLAG_CARRY);
		int halfRes = (dest & 0xF) + (src & 0xF) + this->isFlagSet(FLAG_CARRY);

		this->registers[Reg::F] = (halfRes > 0xF ? FLAG_HALF_CARRY : 0) | (res > 0xFF ? FLAG_CARRY : 0) |
								  (this->registers[Reg::A] == 0 ? FLAG_ZERO : 0);
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::sub_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->registers[Reg::A], src = this->registers[opcode & 0b111];
		this->registers[Reg::A] -= src;
		this->registers[Reg::F] = ((dest & 0xF) - (src & 0xF) & 0x10 ? FLAG_HALF_CARRY : 0) |
								  (dest < src ? FLAG_CARRY : 0) | (dest == src ? FLAG_ZERO : 0) | FLAG_NEGATIVE;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::sbc_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->registers[Reg::A], src = this->registers[opcode & 0b111];
		this->registers[Reg::A] = this->registers[Reg::A] - src - this->isFlagSet(FLAG_CARRY);

		int signedRes = static_cast<int>(dest) - src - this->isFlagSet(FLAG_CARRY);
		int halfSignedRes = static_cast<int>(dest & 0xF) - (src & 0xF) - (this->isFlagSet(FLAG_CARRY));

		this->registers[Reg::F] = FLAG_NEGATIVE | (this->registers[Reg::A] == 0 ? FLAG_ZERO : 0) |
								  (halfSignedRes < 0 ? FLAG_HALF_CARRY : 0) | (signedRes < 0 ? FLAG_CARRY : 0);
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::and_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->registers[Reg::A], src = this->registers[opcode & 0b111];
		this->registers[Reg::F] = ((dest & src) == 0 ? FLAG_ZERO : 0) | FLAG_HALF_CARRY;
		this->registers[Reg::A] &= this->registers[opcode & 0b111];
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::xor_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->registers[Reg::A], src = this->registers[opcode & 0b111];
		this->registers[Reg::F] = ((dest ^ src) == 0 ? FLAG_ZERO : 0);
		this->registers[Reg::A] ^= src;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::or_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->registers[Reg::A], src = this->registers[opcode & 0b111];
		this->registers[Reg::F] = ((dest | src) == 0 ? FLAG_ZERO : 0);
		this->registers[Reg::A] |= src;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::cp_a_reg(uint8_t opcode)
	{
		uint8_t dest = this->registers[Reg::A], src = this->registers[opcode & 0b111];
		this->registers[Reg::F] = ((dest & 0xF) - (src & 0xF) & 0x10 ? FLAG_HALF_CARRY : 0) |
								  (dest < src ? FLAG_CARRY : 0) | (dest == src ? FLAG_ZERO : 0) | FLAG_NEGATIVE;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::jr_cond_i8(bool cond)
	{
		if (cond)
		{
			this->pc += static_cast<int8_t>(this->_gb.readByte(this->pc + 1));
			this->_cycleTimer += 12;
		}
		else
			this->_cycleTimer += 8;
		this->pc += 2;
	}

	void CPU::ld_hl_reg(int opcode)
	{
		this->_gb.writeByte(this->registers[Reg::H] << 8 | this->registers[Reg::L], this->registers[opcode & 0b111]);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::ld_reg_hl(int opcode)
	{
		this->registers[opcode >> 3 & 0b111] =
			this->_gb.readByte(this->registers[Reg::H] << 8 | this->registers[Reg::L]);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::ld_reg_u8(int opcode)
	{
		this->registers[opcode + 0x40 >> 3 & 0b111] = this->_gb.readByte(this->pc + 1);
		this->pc += 2;
		this->_cycleTimer += 8;
	}

	void CPU::dec_reg(int opcode)
	{
		Reg reg = static_cast<Reg>((opcode - 0x5) / 8);
		uint8_t upperNibbleBefore = (this->registers[reg] >> 4);
		this->registers[reg]--;
		uint8_t upperNibbleAfter = (this->registers[reg] >> 4);
		this->registers[Reg::F] = (this->registers[reg] == 0 ? FLAG_ZERO : 0) | FLAG_NEGATIVE |
								  this->getFlag(FLAG_CARRY) |
								  (upperNibbleBefore != upperNibbleAfter ? FLAG_HALF_CARRY : 0);
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::inc_reg(int opcode)
	{
		Reg reg = static_cast<Reg>((opcode - 0x4) / 8);
		uint8_t upperNibbleBefore = (this->registers[reg] >> 4);
		this->registers[reg]++;
		uint8_t upperNibbleAfter = (this->registers[reg] >> 4);

		this->registers[Reg::F] = (this->registers[reg] == 0 ? FLAG_ZERO : 0) | this->getFlag(FLAG_CARRY) |
								  (upperNibbleBefore != upperNibbleAfter ? FLAG_HALF_CARRY : 0);
		;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::dec_reg16(int opcode)
	{
		Reg high = static_cast<Reg>((opcode - 0x0B) / 16 * 2);
		Reg low = static_cast<Reg>(high + 1);
		uint16_t val = this->readRegister16(high, low);
		this->writeRegister16(high, low, --val);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::dec_sp()
	{
		this->sp--;
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::jp_cond_u16(bool cond)
	{
		if (cond)
		{
			this->pc = this->_gb.readWord(this->pc + 1);
			this->_cycleTimer += 16;
		}
		else
		{
			this->_cycleTimer += 12;
			this->pc += 3;
		}
	}

	void CPU::ld_dec_hl_a()
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		this->_gb.writeByte(addr, this->registers[Reg::A]);
		this->writeRegister16(Reg::H, Reg::L, addr - 1);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::ld_inc_hl_a()
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		this->_gb.writeByte(addr, this->registers[Reg::A]);
		this->writeRegister16(Reg::H, Reg::L, addr + 1);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::ld_reg16_a(Register high, Register low)
	{
		uint16_t addr = this->readRegister16(high, low);
		this->_gb.writeByte(addr, this->registers[Reg::A]);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::di_ei(int opcode)
	{
		if (opcode == 0xFB)
			this->_ei_next_instruction = true;
		else
			this->ime = false;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::ldh_u8_a()
	{
		this->_gb.writeByte(0xFF00 + this->_gb.readByte(this->pc + 1), this->registers[Reg::A]);
		this->pc += 2;
		this->_cycleTimer += 12;
	}

	void CPU::ldh_c_a()
	{
		this->_gb.writeByte(0xFF00 + this->registers[Reg::C], this->registers[Reg::A]);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::ldh_a_u8()
	{
		this->registers[Reg::A] = this->_gb.readByte(0xFF00 + this->_gb.readByte(this->pc + 1));
		this->pc += 2;
		this->_cycleTimer += 12;
	}

	void CPU::ldh_a_c()
	{
		this->registers[Reg::A] = this->_gb.readByte(0xFF00 + this->registers[Reg::C]);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::adc_a_u8()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->pc + 1);
		this->registers[Reg::A] += src + this->isFlagSet(FLAG_CARRY);

		int res = dest + src + this->isFlagSet(FLAG_CARRY);
		int halfRes = (dest & 0xF) + (src & 0xF) + this->isFlagSet(FLAG_CARRY);

		this->registers[Reg::F] = (halfRes > 0xF ? FLAG_HALF_CARRY : 0) | (res > 0xFF ? FLAG_CARRY : 0) |
								  (this->registers[Reg::A] == 0 ? FLAG_ZERO : 0);
		this->pc += 2;
		this->_cycleTimer += 8;
	}

	void CPU::sbc_a_u8()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->pc + 1);
		this->registers[Reg::A] = this->registers[Reg::A] - src - this->isFlagSet(FLAG_CARRY);

		int signedRes = static_cast<int>(dest) - src - this->isFlagSet(FLAG_CARRY);
		int halfSignedRes = static_cast<int>(dest & 0xF) - (src & 0xF) - (this->isFlagSet(FLAG_CARRY));

		this->registers[Reg::F] = FLAG_NEGATIVE | (this->registers[Reg::A] == 0 ? FLAG_ZERO : 0) |
								  (halfSignedRes < 0 ? FLAG_HALF_CARRY : 0) | (signedRes < 0 ? FLAG_CARRY : 0);
		this->pc += 2;
		this->_cycleTimer += 8;
	}

	void CPU::xor_a_u8()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->pc + 1);
		this->registers[Reg::A] ^= src;
		this->registers[Reg::F] = (this->registers[Reg::A] == 0 ? FLAG_ZERO : 0);
		this->pc += 2;
		this->_cycleTimer += 8;
	}

	void CPU::cp_a_u8()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->pc + 1);
		this->registers[Reg::F] = ((dest & 0xF) < (src & 0xF) ? FLAG_HALF_CARRY : 0) | (dest < src ? FLAG_CARRY : 0) |
								  (dest == src ? FLAG_ZERO : 0) | FLAG_NEGATIVE;
		this->pc += 2;
		this->_cycleTimer += 8;
	}

	void CPU::ld_a_reg16(Register high, Register low)
	{
		uint16_t addr = this->readRegister16(high, low);
		this->registers[Reg::A] = this->_gb.readByte(addr);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::ld_a_inc_hl()
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		this->registers[Reg::A] = this->_gb.readByte(addr);
		this->writeRegister16(Reg::H, Reg::L, addr + 1);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::ld_a_dec_hl()
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		this->registers[Reg::A] = this->_gb.readByte(addr);
		this->writeRegister16(Reg::H, Reg::L, addr - 1);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::call_cond_u16(bool cond)
	{
		if (cond)
		{
			this->sp -= 2;
			this->_gb.writeWord(this->sp, this->pc + 3);
			this->pc = this->_gb.readWord(this->pc + 1);
			this->_cycleTimer += 24;
		}
		else
		{
			this->pc += 3;
			this->_cycleTimer += 12;
		}
	}

	void CPU::ret_cond(bool cond)
	{
		if (cond)
		{
			this->pc = this->_gb.readWord(this->sp);
			this->sp += 2;
			this->_cycleTimer += 20;
		}
		else
		{
			this->pc++;
			this->_cycleTimer += 8;
		}
	}

	void CPU::jp_hl()
	{
		this->pc = this->readRegister16(Reg::H, Reg::L);
		this->_cycleTimer += 4;
	}

	void CPU::ret() // Only 16 cycles
	{
		this->pc = this->_gb.readWord(this->sp);
		this->sp += 2;
		this->_cycleTimer += 16;
	}

	void CPU::reti()
	{
		this->pc = this->_gb.readWord(this->sp);
		this->sp += 2;
		this->ime = true;
		this->_cycleTimer += 16;
	}

	void CPU::rst(int opcode)
	{
		uint16_t addr = opcode - 0xC7;
		this->sp -= 2;
		this->_gb.writeWord(this->sp, this->pc + 1);
		this->pc = addr;
		this->_cycleTimer += 16;
	}

	void CPU::add_a_u8()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->pc + 1);
		this->registers[Reg::A] += src;
		this->registers[Reg::F] = ((dest & 0xF) + (src & 0xF) > 0xF ? FLAG_HALF_CARRY : 0) |
								  (static_cast<uint16_t>(dest) + src > 0xFF ? FLAG_CARRY : 0) |
								  (this->registers[Reg::A] == 0 ? FLAG_ZERO : 0);
		this->pc += 2;
		this->_cycleTimer += 8;
	}

	void CPU::sub_a_u8()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->pc + 1);
		this->registers[Reg::A] -= src;
		this->registers[Reg::F] = ((dest & 0xF) - (src & 0xF) & 0x10 ? FLAG_HALF_CARRY : 0) |
								  (dest < src ? FLAG_CARRY : 0) | (dest == src ? FLAG_ZERO : 0) | FLAG_NEGATIVE;
		this->pc += 2;
		this->_cycleTimer += 8;
	}

	void CPU::and_a_u8()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->pc + 1);
		this->registers[Reg::F] = ((dest & src) == 0 ? FLAG_ZERO : 0) | FLAG_HALF_CARRY;
		this->registers[Reg::A] &= src;
		this->pc += 2;
		this->_cycleTimer += 8;
	}

	void CPU::or_a_u8()
	{
		uint8_t dest = this->registers[Reg::A], src = this->_gb.readByte(this->pc + 1);
		this->registers[Reg::F] = ((dest | src) == 0 ? FLAG_ZERO : 0);
		this->registers[Reg::A] |= src;
		this->pc += 2;
		this->_cycleTimer += 8;
	}

	void CPU::push_reg16(int opcode)
	{
		Reg high = static_cast<Reg>((opcode - 0xC5) / 16 * 2);
		Reg low = static_cast<Reg>(high + 1);
		// Because reg enum is matched with ld opcode order (bcdehlfa) instead of reg16 order (bcdehlaf)
		if (high == Reg::F)
		{
			high = Reg::A;
			low = Reg::F;
		}
		uint16_t val = this->readRegister16(high, low);
		this->sp -= 2;
		this->_gb.writeWord(this->sp, val);
		this->pc++;
		this->_cycleTimer += 16;
	}

	void CPU::pop_reg16(int opcode)
	{
		Reg high = static_cast<Reg>((opcode - 0xC1) / 16 * 2);
		Reg low = static_cast<Reg>(high + 1);
		// Because reg enum is matched with ld opcode order (bcdehlfa) instead of reg16 order (bcdehlaf)
		if (high == Reg::F)
		{
			high = Reg::A;
			low = Reg::F;
		}
		uint16_t val = this->_gb.readWord(this->sp);
		this->sp += 2;
		this->writeRegister16(high, low, val);
		if (low == Reg::F)
			this->registers[Reg::F] &= 0xF0;
		this->pc++;
		this->_cycleTimer += 12;
	}

	void CPU::inc_hl()
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		uint8_t val = this->_gb.readByte(addr);
		uint8_t upperNibbleBefore = (val >> 4);
		this->_gb.writeByte(addr, val + 1);
		uint8_t upperNibbleAfter = (this->_gb.readByte(addr) >> 4);
		this->registers[Reg::F] = (val == 0xFF ? FLAG_ZERO : 0) |
								  (upperNibbleBefore != upperNibbleAfter ? FLAG_HALF_CARRY : 0) |
								  this->getFlag(FLAG_CARRY);
		this->pc += 1;
		this->_cycleTimer += 12;
	}

	void CPU::dec_hl()
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		uint8_t val = this->_gb.readByte(addr);
		uint8_t upperNibbleBefore = (val >> 4);
		this->_gb.writeByte(addr, val - 1);
		uint8_t upperNibbleAfter = (this->_gb.readByte(addr) >> 4);
		this->registers[Reg::F] = (val == 1 ? FLAG_ZERO : 0) |
								  (upperNibbleBefore != upperNibbleAfter ? FLAG_HALF_CARRY : 0) |
								  this->getFlag(FLAG_CARRY) | FLAG_NEGATIVE;
		this->pc += 1;
		this->_cycleTimer += 12;
	}

	void CPU::add_sp_i8()
	{
		int8_t val = this->_gb.readByte(this->pc + 1);
		if (val >= 0)
		{
			this->registers[Reg::F] = ((this->sp & 0xFF) + val > 0xFF ? FLAG_CARRY : 0) |
									  ((this->sp & 0xF) + (val & 0xF) > 0xF ? FLAG_HALF_CARRY : 0);
		}
		else
		{
			this->registers[Reg::F] = ((this->sp + val & 0xFF) <= (this->sp & 0xFF) ? FLAG_CARRY : 0) |
									  ((this->sp + val & 0xF) <= (this->sp & 0xF) ? FLAG_HALF_CARRY : 0);
		}
		this->sp += val;
		this->pc += 2;
		this->_cycleTimer += 16;
	}

	void CPU::ld_hl_sp_i8()
	{
		int8_t val = this->_gb.readByte(this->pc + 1);
		this->writeRegister16(Reg::H, Reg::L, this->sp + val);
		if (val >= 0)
		{
			this->registers[Reg::F] = ((this->sp & 0xFF) + val > 0xFF ? FLAG_CARRY : 0) |
									  ((this->sp & 0xF) + (val & 0xF) > 0xF ? FLAG_HALF_CARRY : 0);
		}
		else
		{
			this->registers[Reg::F] = ((this->sp + val & 0xFF) <= (this->sp & 0xFF) ? FLAG_CARRY : 0) |
									  ((this->sp + val & 0xF) <= (this->sp & 0xF) ? FLAG_HALF_CARRY : 0);
		}
		this->pc += 2;
		this->_cycleTimer += 12;
	}

	void CPU::ld_sp_hl()
	{
		this->sp = this->readRegister16(Reg::H, Reg::L);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::ld_u16_a()
	{
		uint16_t addr = this->_gb.readWord(this->pc + 1);
		this->_gb.writeByte(addr, this->registers[Reg::A]);
		this->pc += 3;
		this->_cycleTimer += 16;
	}

	void CPU::ld_a_u16()
	{
		uint16_t addr = this->_gb.readWord(this->pc + 1);
		this->registers[Reg::A] = this->_gb.readByte(addr);
		this->pc += 3;
		this->_cycleTimer += 16;
	}

	void CPU::ld_hl_u8()
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		this->_gb.writeByte(addr, this->_gb.readByte(this->pc + 1));
		this->pc += 2;
		this->_cycleTimer += 12;
	}

	void CPU::add_hl_reg16(int opcode)
	{
		Reg high = static_cast<Reg>((opcode - 0x09) / 16 * 2);
		Reg low = static_cast<Reg>(high + 1);
		uint16_t hl = this->readRegister16(Reg::H, Reg::L);
		uint16_t val = this->readRegister16(high, low);
		this->writeRegister16(Reg::H, Reg::L, hl + val);

		uint32_t res = hl + val;
		uint32_t halfRes = (hl & 0xFFF) + (val & 0xFFF);

		this->registers[Reg::F] =
			((res & 0x10000) ? FLAG_CARRY : 0) | (halfRes > 0xFFF ? FLAG_HALF_CARRY : 0) | this->getFlag(FLAG_ZERO);

		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::add_hl_sp(int opcode)
	{
		uint16_t hl = this->readRegister16(Reg::H, Reg::L);
		this->writeRegister16(Reg::H, Reg::L, hl + this->sp);

		uint32_t res = hl + this->sp;
		uint32_t halfRes = (hl & 0xFFF) + (this->sp & 0xFFF);

		this->registers[Reg::F] =
			((res & 0x10000) ? FLAG_CARRY : 0) | (halfRes > 0xFFF ? FLAG_HALF_CARRY : 0) | this->getFlag(FLAG_ZERO);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::ccf()
	{
		this->registers[Reg::F] = this->getFlag(FLAG_ZERO) | (!this->isFlagSet(FLAG_CARRY) ? FLAG_CARRY : 0);
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::cpl()
	{
		this->registers[Reg::A] = ~this->registers[Reg::A];
		this->registers[Reg::F] =
			FLAG_NEGATIVE | FLAG_HALF_CARRY | this->getFlag(FLAG_ZERO) | this->getFlag(FLAG_CARRY);
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::scf()
	{
		this->registers[Reg::F] = FLAG_CARRY | this->getFlag(FLAG_ZERO);
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::daa()
	{
		bool carry = this->isFlagSet(FLAG_CARRY);
		bool halfCarry = this->isFlagSet(FLAG_HALF_CARRY);
		bool negative = this->isFlagSet(FLAG_NEGATIVE);

		int correction = 0;
		uint8_t setCarry = 0;

		if (halfCarry || (!negative && (this->registers[Reg::A] & 0xF) > 9))
			correction |= 0x06;
		if (carry || (!negative && this->registers[Reg::A] > 0x99))
		{
			correction |= 0x60;
			setCarry = FLAG_CARRY;
		}

		this->registers[Reg::A] = this->registers[Reg::A] + (negative ? -correction : correction);

		this->registers[Reg::F] =
			this->getFlag(FLAG_NEGATIVE) | (this->registers[Reg::A] == 0 ? FLAG_ZERO : 0) | setCarry;

		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::halt()
	{
		this->halted = true;
		this->pc++;
		this->_cycleTimer += 4;
	}

	void CPU::stop()
	{
		this->stopped = true;
		this->pc++;
		this->_cycleTimer += 4;
	}

	std::ostream& operator<<(std::ostream& os, const CPU& cpu)
	{
		os << std::hex << std::setw(4) << std::setfill('0') << cpu.pc << ":  " << std::setw(2) << std::setfill('0')
		   << (int)cpu._gb.readByte(cpu.pc) << "     ";
		os << "A:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu.registers[CPU::Reg::A]) << " "
		   << "F:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu.registers[CPU::Reg::F]) << " "
		   << "B:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu.registers[CPU::Reg::B]) << " "
		   << "C:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu.registers[CPU::Reg::C]) << " "
		   << "D:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu.registers[CPU::Reg::D]) << " "
		   << "E:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu.registers[CPU::Reg::E]) << " "
		   << "H:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu.registers[CPU::Reg::H]) << " "
		   << "L:" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(cpu.registers[CPU::Reg::L]) << " "
		   << "SP:" << std::setw(4) << std::setfill('0') << static_cast<uint16_t>(cpu.sp) << "  CY:" << std::dec
		   << cpu._ticks / 4;
		return os;
	}

	void CPU::rlc_hl()
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		uint8_t val = this->_gb.readByte(addr);
		this->registers[Reg::F] = (val & 0x80) ? FLAG_CARRY : 0;
		this->_gb.writeByte(addr, (val << 1) | (val >> 7));
		this->registers[Reg::F] |= (this->_gb.readByte(addr) == 0) ? FLAG_ZERO : 0;
		this->pc++;
		this->_cycleTimer += 12;
	}

	void CPU::rrc_hl()
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		uint8_t val = this->_gb.readByte(addr);
		this->registers[Reg::F] = (val & 0x01) ? FLAG_CARRY : 0;
		this->_gb.writeByte(addr, (val >> 1) | (val << 7));
		this->registers[Reg::F] |= (this->_gb.readByte(addr) == 0) ? FLAG_ZERO : 0;
		this->pc++;
		this->_cycleTimer += 12;
	}

	void CPU::rl_hl()
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		uint8_t val = this->_gb.readByte(addr);
		bool carry = this->isFlagSet(FLAG_CARRY);
		this->registers[Reg::F] = (val & 0x80) ? FLAG_CARRY : 0;
		this->_gb.writeByte(addr, (val << 1) | carry);
		this->registers[Reg::F] |= (this->_gb.readByte(addr) == 0) ? FLAG_ZERO : 0;
		this->pc++;
		this->_cycleTimer += 12;
	}

	void CPU::rr_hl()
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		uint8_t val = this->_gb.readByte(addr);
		bool carry = this->isFlagSet(FLAG_CARRY);
		this->registers[Reg::F] = (val & 0x1) ? FLAG_CARRY : 0;
		this->_gb.writeByte(addr, (val >> 1) | (carry << 7));
		this->registers[Reg::F] |= (this->_gb.readByte(addr) == 0) ? FLAG_ZERO : 0;
		this->pc++;
		this->_cycleTimer += 12;
	}

	void CPU::sla_hl()
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		uint8_t val = this->_gb.readByte(addr);
		this->registers[Reg::F] = (val & 0x80) ? FLAG_CARRY : 0;
		this->_gb.writeByte(addr, val << 1);
		this->registers[Reg::F] |= (this->_gb.readByte(addr) == 0) ? FLAG_ZERO : 0;
		this->pc++;
		this->_cycleTimer += 12;
	}

	void CPU::sra_hl()
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		uint8_t val = this->_gb.readByte(addr);
		this->registers[Reg::F] = (val & 0x01) ? FLAG_CARRY : 0;
		this->_gb.writeByte(addr, (val >> 1) | (val & 0x80));
		this->registers[Reg::F] |= (this->_gb.readByte(addr) == 0) ? FLAG_ZERO : 0;
		this->pc++;
		this->_cycleTimer += 12;
	}

	void CPU::srl_hl()
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		uint8_t val = this->_gb.readByte(addr);
		this->registers[Reg::F] = (val & 0x01) ? FLAG_CARRY : 0;
		this->_gb.writeByte(addr, val >> 1);
		this->registers[Reg::F] |= (this->_gb.readByte(addr) == 0) ? FLAG_ZERO : 0;
		this->pc++;
		this->_cycleTimer += 12;
	}

	void CPU::swap_hl()
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		uint8_t val = this->_gb.readByte(addr);
		this->_gb.writeByte(addr, (val >> 4) | (val << 4));
		this->registers[Reg::F] = (this->_gb.readByte(addr) == 0 ? FLAG_ZERO : 0);
		this->pc++;
		this->_cycleTimer += 12;
	}

	void CPU::bit_hl(uint8_t bit)
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		uint8_t val = this->_gb.readByte(addr);
		this->registers[Reg::F] =
			((val & (1 << bit)) == 0 ? FLAG_ZERO : 0) | FLAG_HALF_CARRY | this->getFlag(FLAG_CARRY);
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::res_hl(uint8_t bit)
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		uint8_t val = this->_gb.readByte(addr);
		this->_gb.writeByte(addr, val & ~(1 << bit));
		this->pc++;
		this->_cycleTimer += 8;
	}

	void CPU::set_hl(uint8_t bit)
	{
		uint16_t addr = this->readRegister16(Reg::H, Reg::L);
		uint8_t val = this->_gb.readByte(addr);
		this->_gb.writeByte(addr, val | (1 << bit));
		this->pc++;
		this->_cycleTimer += 8;
	}

}
