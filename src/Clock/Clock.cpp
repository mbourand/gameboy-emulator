#include "Clock.hpp"
#include <chrono>

namespace gbmu
{
	Clock::Clock(double frequency) : frequency(frequency) { this->_last = std::chrono::high_resolution_clock::now(); }

	bool Clock::isTimeout() const
	{
		using namespace std::chrono;
		auto now = high_resolution_clock::now();
		auto duration = duration_cast<nanoseconds>(now - this->_last);
		return duration.count() >= this->frequency;
	}

	long long Clock::getElapsedTime() const
	{
		using namespace std::chrono;
		auto now = high_resolution_clock::now();
		auto duration = duration_cast<nanoseconds>(now - this->_last);
		return duration.count();
	}

	void Clock::reset() { this->_last = std::chrono::high_resolution_clock::now(); }
}
