#include "Memory.hpp"
#include "MBC1.hpp"
#include "NoMBC.hpp"
#include "utils.hpp"
#include <iostream>

namespace gbmu
{
	void Memory::loadCartridge(const Cartridge& cartridge)
	{
		this->_bytes.fill(0x0);

		for (size_t i = 0; i < Memory::CARTRIDGE_END; ++i)
			this->_bytes[Memory::CARTRIDGE_START + i] = cartridge.bytes[i];

		uint8_t mbcType = cartridge.bytes[Memory::CARTRIDGE_MBC_TYPE];
		switch (mbcType)
		{
			case 0x00:
				this->_mbc = std::make_unique<NoMBC>();
				break;
			case 0x01:
			case 0x02:
			case 0x03:
				this->_mbc = std::make_unique<MBC1>(cartridge);
				break;
			default:
				std::cerr << "Unsupported MBC type: " << static_cast<int>(mbcType) << std::endl;
				break;
		}
	}

	uint8_t Memory::readByte(uint16_t address)
	{
		if (this->_mbc->isConcerned(address, MBC::READ))
			return this->_mbc->readByte(address);

		return this->_bytes[address];
	}

	void Memory::writeByte(uint16_t address, uint8_t value)
	{
		if (this->_mbc->isConcerned(address, MBC::WRITE))
			this->_mbc->writeByte(address, value);
		else
			this->_bytes[address] = value;
	}

	uint16_t Memory::readWord(uint16_t address)
	{
		if (this->_mbc->isConcerned(address, MBC::READ))
			return this->_mbc->readWord(address);
		return (this->_bytes[address + 1] << 8) | this->_bytes[address];
	}

	void Memory::writeWord(uint16_t address, uint16_t value)
	{
		if (this->_mbc->isConcerned(address, MBC::WRITE))
		{
			this->_mbc->writeWord(address, value);
			return;
		}

		this->_bytes[address + 1] = static_cast<uint8_t>(value >> 8);
		this->_bytes[address] = static_cast<uint8_t>(value & 0xFF);
	}

	void Memory::setDivTimer(uint8_t value) { this->_bytes[Memory::DIV] = value; }
	void Memory::setJoypad(uint8_t value) { this->_bytes[Memory::JOYPAD] = value; }
	uint8_t Memory::getJoypad() const { return this->_bytes[Memory::JOYPAD]; }
}
