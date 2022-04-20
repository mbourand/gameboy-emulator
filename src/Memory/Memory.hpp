#pragma once

#include "Cartridge.hpp"
#include <array>

namespace gbmu
{
	class Memory
	{
	protected:
		std::array<uint8_t, 0xFFFF> _bytes;

	public:
		Memory() = default;
		Memory(const Memory& other) = default;
		Memory& operator=(const Memory& other) = default;

		void loadCartridge(const Cartridge& cartridge);
	};
}
