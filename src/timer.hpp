/********************************
 * Project: Cidr                *
 * File: timer.hpp              *
 * Date: 3.10.2020              *
 ********************************/

#ifndef CIDR_TIMER_HPP
#define CIDR_TIMER_HPP

#include <chrono> // for std::chrono functions

class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1>>;
	
	std::chrono::time_point<clock_t> m_beg;

public:
	Timer() : m_beg(clock_t::now())	{
	}
	
	inline void reset() {
		m_beg = clock_t::now();
	}
	
	inline double elapsed() const {
		return (clock_t::now() - m_beg).count();
	}
};

#endif