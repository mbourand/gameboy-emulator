#pragma once

#include "MBC.hpp"

namespace gbmu
{
	class NoMBC : public MBC
	{
	public:
		NoMBC() = default;
		NoMBC(const NoMBC& other) = default;
		NoMBC& operator=(const NoMBC& other) = default;

		uint8_t readByte(uint16_t address) const override;
		void writeByte(uint16_t address, uint8_t value) override;
		uint16_t readWord(uint16_t address) const override;
		void writeWord(uint16_t address, uint16_t value) override;
		bool isConcerned(uint16_t address, bool write) const override;
	};
}
