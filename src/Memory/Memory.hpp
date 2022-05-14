#pragma once

#include "Cartridge.hpp"
#include <array>

namespace gbmu
{
	class Memory
	{
	public:
		static constexpr size_t MEMORY_SIZE = 0x10000;

		static constexpr size_t CARTRIDGE_START = 0x0000;
		static constexpr size_t CARTRIDGE_END = 0x7FFF;

		static constexpr uint16_t IF = 0xFF0F;
		static constexpr uint16_t IE = 0xFFFF;

		static constexpr uint16_t DIV = 0xFF04;
		static constexpr uint16_t TIMA = 0xFF05;
		static constexpr uint16_t TMA = 0xFF06;

	protected:
		std::array<uint8_t, MEMORY_SIZE> _bytes;

	public:
		Memory() = default;
		Memory(const Memory& other) = default;
		Memory& operator=(const Memory& other) = default;

		void loadCartridge(const Cartridge& cartridge);

		uint8_t readByte(uint16_t address);
		void writeByte(uint16_t address, uint8_t value);
		uint16_t readWord(uint16_t address);
		void writeWord(uint16_t address, uint16_t value);

		void setDivTimer(uint8_t value);
	};
}
