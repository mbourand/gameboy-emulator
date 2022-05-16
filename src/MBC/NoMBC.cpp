#include "NoMBC.hpp"

namespace gbmu
{
	uint8_t NoMBC::readByte(uint16_t address) const { return 0xFF; }
	void NoMBC::writeByte(uint16_t address, uint8_t value) {}
	uint16_t NoMBC::readWord(uint16_t address) const { return 0xFF; }
	void NoMBC::writeWord(uint16_t address, uint16_t value) {}
	bool NoMBC::isConcerned(uint16_t address) const { return false; }
}
