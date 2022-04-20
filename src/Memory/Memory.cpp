#include "Memory.hpp"

namespace gbmu
{
	void Memory::loadCartridge(const Cartridge& cartridge)
	{
		for (size_t i = 0; i < cartridge.bytes.size(); ++i)
			this->_bytes[Memory::CARTRIDGE_START + i] = cartridge.bytes[i];
	}

	uint8_t Memory::readByte(uint16_t address) { return this->_bytes[address]; }
	void Memory::writeByte(uint16_t address, uint8_t value) { this->_bytes[address] = value; }
}
