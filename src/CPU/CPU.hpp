#pragma once

#include "Memory.hpp"

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

		static constexpr uint8_t REGISTER_COUNT = 8;
		enum Register : uint8_t
		{
			A = 0,
			F = 1,
			B = 2,
			C = 3,
			D = 4,
			E = 5,
			H = 6,
			L = 7
		};

	protected:
		Memory& _memory;

	public:
		/* A = Accumulator
		** F = Flags */
		std::array<uint8_t, REGISTER_COUNT> _registers;

		uint16_t sp; // Stack pointer
		uint16_t pc; // Program counter

		CPU() = default;
		CPU(const CPU& other) = default;
		CPU& operator=(const CPU& other) = default;
		CPU(Memory& memory);

		void writeRegister16(Register high, Register low, uint16_t value);
		uint16_t readRegister16(Register high, Register low);

		void tick();
		void perform_opcode(uint8_t opcode);
	};
}
