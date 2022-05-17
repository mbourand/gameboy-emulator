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

	void Memory::update()
	{
		this->_joypad.update();
		this->_handleJoypadInterrupt(this->getJoypad());
	}

	uint8_t Memory::readByte(uint16_t address)
	{
		if (!this->_bytes[0xFF50] && address < BOOT_ROM.size())
			return BOOT_ROM[address];

		if (this->_mbc->isConcerned(address, MBC::READ))
			return this->_mbc->readByte(address);

		if (address == Memory::JOYPAD)
			return this->_joypad.getJoypadState(this->getJoypad());
		return this->_bytes[address];
	}

	void Memory::writeByte(uint16_t address, uint8_t value)
	{
		if (this->_mbc->isConcerned(address, MBC::WRITE))
			this->_mbc->writeByte(address, value);
		else
		{
			if (address == Memory::DIV)
				value = 0;
			else if (address == Memory::JOYPAD)
			{
				value = (this->_bytes[Memory::JOYPAD] & 0b00001111) | (value & 0b00110000);
				this->_handleJoypadInterrupt(value);
			}
			this->_bytes[address] = value;
		}
	}

	uint16_t Memory::readWord(uint16_t address)
	{
		if (!this->_bytes[0xFF50] && address < BOOT_ROM.size())
			return (BOOT_ROM[address + 1] << 8) | BOOT_ROM[address];

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

		if (address == Memory::DIV)
			value = 0;
		else if (address == Memory::JOYPAD)
		{
			value = (this->_bytes[address] & 0b00001111) | (value & 0b00110000);
			this->_handleJoypadInterrupt(value);
		}
		this->_bytes[address + 1] = static_cast<uint8_t>(value >> 8);
		this->_bytes[address] = static_cast<uint8_t>(value & 0xFF);
	}

	void Memory::setDivTimer(uint8_t value) { this->_bytes[Memory::DIV] = value; }
	void Memory::setJoypad(uint8_t value) { this->_bytes[Memory::JOYPAD] = value; }
	uint8_t Memory::getJoypad() const { return this->_bytes[Memory::JOYPAD]; }

	void Memory::_handleJoypadInterrupt(uint8_t value)
	{
		uint8_t current = this->_bytes[Memory::JOYPAD];
		uint8_t newJoyp = this->_joypad.getJoypadState(value);

		for (int bit = 0; bit < 4; bit++)
		{
			if ((current & (1 << bit)) != 0 && (newJoyp & (1 << bit)) == 0)
			{
				this->_bytes[Memory::IF] |= Interrupt::Joypad;
				break;
			}
		}
	}
}
