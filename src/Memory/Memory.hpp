#pragma once

#include "Cartridge.hpp"
#include "MBC.hpp"
#include <array>
#include <memory>

namespace gbmu
{
	class Memory
	{
	public:
		static constexpr size_t MEMORY_SIZE = 0x10000;

		static constexpr size_t CARTRIDGE_START = 0x0000;
		static constexpr size_t CARTRIDGE_END = 0x7FFF;
		static constexpr size_t CARTRIDGE_MBC_TYPE = 0x147;

		static constexpr uint16_t IF = 0xFF0F;
		static constexpr uint16_t IE = 0xFFFF;

		static constexpr uint16_t DIV = 0xFF04;
		static constexpr uint16_t TIMA = 0xFF05;
		static constexpr uint16_t TMA = 0xFF06;
		static constexpr uint16_t TAC = 0xFF07;
		static constexpr uint16_t JOYPAD = 0xFF00;
		static constexpr uint16_t DMA = 0xFF46;

	protected:
		std::array<uint8_t, MEMORY_SIZE> _bytes;
		std::unique_ptr<MBC> _mbc;

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
		void setJoypad(uint8_t value);
		uint8_t getJoypad() const;

	protected:
		void _handleJoypadInterrupt(uint8_t value);
	};
}
