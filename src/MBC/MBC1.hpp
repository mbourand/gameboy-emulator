#pragma once

#include "Cartridge.hpp"
#include "MBC.hpp"
#include <array>
#include <vector>

namespace gbmu
{
	class MBC1 : public MBC
	{
	protected:
		std::vector<std::array<uint8_t, 0x4000>> _romBanks;
		std::vector<std::array<uint8_t, 0x2000>> _ramBanks;
		uint8_t _selectedRomBank;
		uint8_t _selectedRamBank;
		bool _ramEnabled;
		bool _mode;

	public:
		MBC1() = default;
		MBC1(const MBC1& other) = default;
		MBC1(const Cartridge& cartridge);
		MBC1& operator=(const MBC1& other) = default;

		uint8_t readByte(uint16_t address) const override;
		void writeByte(uint16_t address, uint8_t value) override;
		uint16_t readWord(uint16_t address) const override;
		void writeWord(uint16_t address, uint16_t value) override;
		bool isConcerned(uint16_t address, bool write) const override;
	};
}
