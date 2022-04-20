#pragma once

#include <string>
#include <vector>

namespace gbmu
{
	class Cartridge
	{
	protected:
		std::vector<uint8_t> _bytes;

	public:
		Cartridge() = default;
		Cartridge(const Cartridge& other) = default;
		Cartridge(const std::string& filename);
		Cartridge& operator=(const Cartridge& other) = default;

		void loadROM(const std::string& filename);
	};
}
