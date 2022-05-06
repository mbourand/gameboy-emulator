#pragma once

#include "Memory.hpp"
#include <iomanip>
#include <iostream>

namespace gbmu
{
	class CPU
	{
	public:
		static constexpr uint8_t FLAG_ZERO = 0b10000000;
		static constexpr uint8_t FLAG_NEGATIVE = 0b01000000;
		static constexpr uint8_t FLAG_HALF_CARRY = 0b00100000;
		static constexpr uint8_t FLAG_CARRY = 0b00010000;

		static constexpr uint16_t ENTRY_POINT = 0x100;
		static constexpr uint16_t ENTRY_STACK_POINTER = 0xFFFE;

		static constexpr uint8_t REGISTER_COUNT = 8;
		enum Register : uint8_t
		{
			B = 0,
			C = 1,
			D = 2,
			E = 3,
			H = 4,
			L = 5,
			F = 6,
			A = 7
		};

	protected:
		Memory& _memory;

	public:
		/* A = Accumulator
		** F = Flags */
		std::array<uint8_t, REGISTER_COUNT> _registers;

		uint16_t sp; // Stack pointer
		uint16_t pc; // Program counter

		uint32_t totalCycles;
		uint16_t cycleTimer;

		bool enable_interrupts;

		CPU() = default;
		CPU(const CPU& other) = default;
		CPU& operator=(const CPU& other) = default;
		CPU(Memory& memory);

		void writeRegister16(Register high, Register low, uint16_t value);
		uint16_t readRegister16(Register high, Register low);

		bool isFlagSet(uint8_t flag);

		void tick();
		void perform_opcode(uint8_t opcode);

	protected:
		void nop();
		void inc_bc();
		void rlca();
		void ld_u16_sp();
		void add_hl_bc();
		void ld_reg_reg(uint8_t opcode);
		void add_a_reg(uint8_t opcode);
		void adc_a_reg(uint8_t opcode);
		void sub_a_reg(uint8_t opcode);
		void sbc_a_reg(uint8_t opcode);
		void and_a_reg(uint8_t opcode);
		void xor_a_reg(uint8_t opcode);
		void or_a_reg(uint8_t opcode);
		void cp_a_reg(uint8_t opcode);
		void jr_cond_i8(bool cond);
		void ld_hl_reg(int opcode);
		void ld_reg_hl(int opcode);
		void ld_reg_u8(int opcode);
		void dec_reg(int opcode);
		void inc_reg(int opcode);
		void dec_reg16(int opcode);
		void jp_u16();
		void ld_dec_hl_a();
		void ld_inc_hl_a();
		void ld_reg16_a(Register high, Register low);
		void ld_reg16_u16(int opcode);
		void ld_sp_u16();
		void di_ei(int opcode);

	public:
		friend std::ostream& operator<<(std::ostream& os, const CPU& cpu);
	};

	std::ostream& operator<<(std::ostream& os, const CPU& cpu);
}
