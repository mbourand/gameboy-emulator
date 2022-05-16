#include "MBC1.hpp"

namespace gbmu
{
	MBC1::MBC1(const Cartridge& cartridge)
	{
		_ramBanks.resize(4);
		for (auto& ramBank : _ramBanks)
			ramBank.fill(0x00);
		for (int i = 0; i < cartridge.bytes.size(); i += 0x4000)
		{
			_romBanks.push_back({});
			_romBanks.back().fill(0);
			std::copy(cartridge.bytes.begin() + i, cartridge.bytes.begin() + i + 0x4000, _romBanks.back().begin());
		}
		_selectedRomBank = 1;
		_selectedRamBank = 0;
		_ramEnabled = false;
		_mode = false;
	}

	uint8_t MBC1::readByte(uint16_t address) const
	{
		uint8_t romBank = this->_selectedRomBank | (!this->_mode ? ((this->_selectedRamBank & 0b11) << 5) : 0);
		uint8_t ramBank = (!this->_mode ? 0 : this->_selectedRamBank);

		if (address <= 0x3FFF) // ROM BANK 0
			return this->_romBanks[0][address];
		if (address >= 0x4000 && address <= 0x7FFF) // Switchable ROM BANK
			return this->_romBanks[romBank][address - 0x4000];
		if (address >= 0xA000 && address <= 0xBFFF) // Switchable RAM BANK
			return (this->_ramEnabled ? this->_ramBanks[ramBank][address - 0xA000] : 0xFF);

		return 0xFF;
	}

	void MBC1::writeByte(uint16_t address, uint8_t value)
	{
		uint8_t ramBank = (!this->_mode ? 0 : this->_selectedRamBank);

		if (address >= 0x0000 && address <= 0x1FFF) // RAM enable
			this->_ramEnabled = (value & 0x0F) == 0x0A;
		else if (address >= 0x2000 && address <= 0x3FFF)         // ROM bank select
			if (value == 0x20 || value == 0x40 || value == 0x60) // Weird bug to reproduce
				this->_selectedRomBank = value + 1;
			else
				this->_selectedRomBank = std::max(value & 0x1F, 1);
		else if (address >= 0x4000 && address <= 0x5FFF) // RAM bank select
			this->_selectedRamBank = (value & 0b11);
		else if (address >= 0x6000 && address <= 0x7FFF) // ROM/RAM mode select
			this->_mode = value;
		else if (address >= 0xA000 && address <= 0xBFFF) // Switchable RAM BANK
			if (this->_ramEnabled)
				this->_ramBanks[ramBank][address - 0xA000] = value;
	}

	uint16_t MBC1::readWord(uint16_t address) const
	{
		uint8_t romBank = this->_selectedRomBank | (!this->_mode ? ((this->_selectedRamBank & 0b11) << 5) : 0);
		uint8_t ramBank = (!this->_mode ? 0 : this->_selectedRamBank);

		if (address <= 0x3FFF) // ROM BANK 0
			return (this->_romBanks[0][address + 1] << 8) | this->_romBanks[0][address];
		if (address >= 0x4000 && address <= 0x7FFF) // Switchable ROM BANK
			return (this->_romBanks[romBank][address - 0x4000 + 1] << 8) | this->_romBanks[romBank][address - 0x4000];
		if (address >= 0xA000 && address <= 0xBFFF) // Switchable RAM BANK
			return (_ramEnabled ? ((this->_ramBanks[ramBank][address - 0xA000 + 1] << 8) |
								   this->_ramBanks[ramBank][address - 0xA000])
								: 0xFFFF);
		return 0xFF;
	}

	void MBC1::writeWord(uint16_t address, uint16_t value)
	{
		uint8_t ramBank = (!this->_mode ? 0 : this->_selectedRamBank);

		if (address >= 0x0000 && address <= 0x1FFF) // RAM enable
			this->_ramEnabled = (value & 0x0F) == 0x0A;
		else if (address >= 0x2000 && address <= 0x3FFF)         // ROM bank select
			if (value == 0x20 || value == 0x40 || value == 0x60) // Weird bug to reproduce
				this->_selectedRomBank = value + 1;
			else
				this->_selectedRomBank = std::max(value & 0x1F, 1);
		else if (address >= 0x4000 && address <= 0x5FFF) // RAM bank select
			this->_selectedRamBank = (value & 0b11);
		else if (address >= 0x6000 && address <= 0x7FFF) // ROM/RAM mode select
			this->_mode = value;
		else if (address >= 0xA000 && address <= 0xBFFF) // Switchable RAM BANK
		{
			if (this->_ramEnabled)
			{
				this->_ramBanks[ramBank][address - 0xA000] = (value & 0xFF);
				this->_ramBanks[ramBank][address - 0xA000 + 1] = (value >> 8);
			}
		}
	}

	bool MBC1::isConcerned(uint16_t address, bool write) const
	{
		return (address <= 0x3FFF) && (address >= 0x4000 && address <= 0x7FFF) ||
			   (address >= 0xA000 && address <= 0xBFFF) || (address <= 0x1FFF) ||
			   (address >= 0x2000 && address <= 0x3FFF) || (address >= 0x6000 && address <= 0x7FFF) ||
			   (address >= 0x4000 && address <= 0x5FFF);
	}
}
