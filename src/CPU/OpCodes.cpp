#include "CPU.hpp"

namespace gbmu
{
	void CPU::perform_opcode(uint8_t opcode)
	{
		switch (opcode)
		{
			case 0x00: // NOP
				this->pc++;
				break;
			case 0x01: // LD BC, u16
				this->writeRegister16(Register::B, Register::C, this->_memory.readWord(this->pc + 1));
				this->pc += 3;
				break;
			case 0x02: // LD (BC), A
				this->_memory.writeByte(this->readRegister16(Register::B, Register::C), this->_registers[Register::A]);
				this->pc++;
				break;
			case 0x03: // INC BC
				this->writeRegister16(Register::B, Register::C, this->readRegister16(Register::B, Register::C) + 1);
				this->pc++;
				break;
			case 0x04: // INC B
				this->_registers[Register::B]++;
				this->_registers[Register::F] ^= (~FLAG_NEGATIVE);
				this->_registers[Register::F] |= ((this->_registers[Register::B] == 0) ? FLAG_ZERO : 0) |
												 ((this->_registers[Register::B] & 0x10) == 0x10 ? FLAG_HALF_CARRY : 0);
				this->pc++;
				break;
			case 0x05: // DEC B
				this->_registers[Register::B]--;
				this->_registers[Register::F] |= FLAG_NEGATIVE |
												 ((this->_registers[Register::B] == 0) ? FLAG_ZERO : 0) |
												 ((this->_registers[Register::B] & 0x10) ? FLAG_HALF_CARRY : 0);
				this->pc++;
				break;
			case 0x06: // LD B, u8
				this->_registers[Register::B] = this->_memory.readByte(this->pc + 1);
				this->pc += 2;
				break;
			case 0x07: // RLCA (Rotate left through carry accumulator)
				this->_registers[Register::F] = (this->_registers[Register::A] & 0x80) ? FLAG_CARRY : 0;
				this->_registers[Register::A] =
					(this->_registers[Register::A] << 1) | (this->_registers[Register::A] >> 7);
				this->pc++;
				break;
			case 0x08: // LD (u16), SP
				this->_memory.writeWord(this->_memory.readWord(this->pc + 1), this->sp);
				this->pc += 3;
				break;
			case 0x09: // ADD HL, BC
				uint16_t hl = this->readRegister16(Register::H, Register::L),
						 bc = this->readRegister16(Register::B, Register::C);
				uint8_t h = this->_registers[Register::H], b = this->_registers[Register::B];
				this->_registers[Register::F] |=
					((h & 0xF) + (b & 0xF) & 0x10 ? FLAG_HALF_CARRY : 0) | (h + b > 0xFF ? FLAG_CARRY : 0);
				this->writeRegister16(Register::H, Register::L, hl + bc);
				this->pc++;
				break;
		}
	}
}
