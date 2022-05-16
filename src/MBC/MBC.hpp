#pragma once

#include <cstdint>

namespace gbmu
{
	class MBC
	{
	public:
		static constexpr bool WRITE = true;
		static constexpr bool READ = false;

		virtual ~MBC() = default;
		virtual uint8_t readByte(uint16_t address) const = 0;
		virtual void writeByte(uint16_t address, uint8_t value) = 0;
		virtual uint16_t readWord(uint16_t address) const = 0;
		virtual void writeWord(uint16_t address, uint16_t value) = 0;
		virtual bool isConcerned(uint16_t address, bool write) const = 0;
	};
}
