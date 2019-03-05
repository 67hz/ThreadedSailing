#pragma once

#include <thread>

class thread_guard
{
	std::thread &t;	// ref to thread object
	
public:
	explicit thread_guard(std::thread & _t) : t(_t) {};

	~thread_guard() {
		if (t.joinable()) {
			t.join();
		}
	}

	thread_guard &operator=(thread_guard &const) = delete;	// not going to assign threads
	thread_guard(thread_guard &const) = delete;	// not going to copy threads
};
