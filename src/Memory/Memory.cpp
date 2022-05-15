#include "Memory.hpp"
#include <iostream>

namespace gbmu
{
	void Memory::loadCartridge(const Cartridge& cartridge)
	{
		this->_bytes.fill(0);

		for (size_t i = 0; i < cartridge.bytes.size(); ++i)
			this->_bytes[Memory::CARTRIDGE_START + i] = cartridge.bytes[i];
	}

	uint8_t Memory::readByte(uint16_t address)
	{
		if (!this->_bytes[0xFF50] && address < BOOT_ROM.size())
			return BOOT_ROM[address];
		return this->_bytes[address];
	}

	void Memory::writeByte(uint16_t address, uint8_t value) { this->_bytes[address] = value; }

	uint16_t Memory::readWord(uint16_t address)
	{
		if (!this->_bytes[0xFF50] && address < BOOT_ROM.size())
			return (BOOT_ROM[address + 1] << 8) | BOOT_ROM[address];
		return (this->_bytes[address + 1] << 8) | this->_bytes[address];
	}

	void Memory::writeWord(uint16_t address, uint16_t value)
	{
		if (address == Memory::DIV)
			value = 0;
		this->_bytes[address + 1] = static_cast<uint8_t>(value >> 8);
		this->_bytes[address] = static_cast<uint8_t>(value & 0xFF);
	}

	void Memory::setDivTimer(uint8_t value) { this->_bytes[Memory::DIV] = value; }
}
