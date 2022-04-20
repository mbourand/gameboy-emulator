#pragma once

#include <string>
#include <vector>

namespace gbmu
{
	class Cartridge
	{
	public:
		std::vector<uint8_t> bytes;

	public:
		Cartridge() = default;
		Cartridge(const Cartridge& other) = default;
		Cartridge(const std::string& filename);
		Cartridge& operator=(const Cartridge& other) = default;

		void loadROM(const std::string& filename);

		friend std::ostream& operator<<(std::ostream& os, const Cartridge& cartridge);
	};

	std::ostream& operator<<(std::ostream& os, const Cartridge& cartridge);
}
