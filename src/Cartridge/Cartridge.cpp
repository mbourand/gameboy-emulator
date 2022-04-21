#include "Cartridge.hpp"
#include <fstream>
#include <iomanip>
#include <stdexcept>

namespace gbmu
{
	Cartridge::Cartridge(const std::string& filename) { this->loadROM(filename); }

	void Cartridge::loadROM(const std::string& filename)
	{
		this->bytes.clear();

		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		if (!file.is_open())
			throw std::runtime_error("Could not open file: " + filename);

		const auto size = file.tellg();
		file.seekg(0, std::ios::beg);

		this->bytes.resize(size);
		file.read(reinterpret_cast<char*>(this->bytes.data()), size);
	}

	std::ostream& operator<<(std::ostream& os, const Cartridge& cartridge)
	{
		int i = 0;
		for (const auto& byte : cartridge.bytes)
		{
			os << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
			if (++i % 16 == 0)
				os << std::endl;
		}
		os << std::dec << std::setfill(' ') << std::setw(0);

		return os;
	}
}
