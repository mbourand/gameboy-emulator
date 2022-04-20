#pragma once

#include "Memory.hpp"

namespace gbmu
{
	class CPU
	{
	public:
		static constexpr uint8_t FLAG_ZERO = 0b10000000;
		static constexpr uint8_t FLAG_ADD = 0b01000000;
		static constexpr uint8_t FLAG_HALF_CARRY = 0b00100000;
		static constexpr uint8_t FLAG_CARRY = 0b00010000;

	protected:
		Memory& _memory;

		uint16_t _af; // Accumulator and flags
		uint16_t _bc;
		uint16_t _de;
		uint16_t _hl;
		uint16_t _sp; // Stack pointer
		uint16_t _pc; // Program counter

	public:
		CPU() = default;
		CPU(const CPU& other) = default;
		CPU& operator=(const CPU& other) = default;
		CPU(Memory& memory);

		// Used to get the high/low 8 bits of a 16-bit registers
		inline uint8_t* high(uint16_t* value) { return reinterpret_cast<uint8_t*>(value); }
		inline uint8_t* low(uint16_t* value) { return reinterpret_cast<uint8_t*>(value) + 1; }

		void tick();
	};
}
