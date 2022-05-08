#pragma once

#include <chrono>

namespace gbmu
{
	class Clock
	{
	protected:
		std::chrono::high_resolution_clock::time_point _last;

	public:
		double frequency;

		Clock() = default;
		Clock(double frequency);

		bool isTimeout() const;
		void reset();
	};
}
