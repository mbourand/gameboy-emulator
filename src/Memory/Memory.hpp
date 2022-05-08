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
	};
}
