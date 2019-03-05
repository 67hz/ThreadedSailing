/*
	This could be improved by creating a task queue to avoid overlapping requests (e.g. calling clean while a clean task is processing).
	It might also be cleaner to create a Captain class with a delegator method to issue instructions based on a given int param. There would be a downside
	to that in Captain class needing to reference member functions of the CleaningCrew and EngineCrew task.
*/
#include "pch.h"
#include <iostream>
#include <chrono>
#include <thread>


class CleaningCrew
{
public:
	void clean()
	{
		printf("Cleaning in progress... thread_id: %d\n", std::this_thread::get_id());
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		printf("Cleaning complete.\n");
	}
};

class EngineCrew
{
public:
	int accelerate()
	{
		printf("Increasing Speed... thread_id: %d\n", std::this_thread::get_id());
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		printf("Full Speed Ahead!\n");
		return 0;
	}

	int stopEngine()
	{
		printf("Stopping engine... thread_id: %d\n", std::this_thread::get_id());
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		printf("Engine stopped\n");
		return 0;
	}
};


int main()
{
	int instruction = 0;
	class CleaningCrew cleaningCrew;
	class EngineCrew engineCrew;

	while (instruction != 100)
	{
		std::cout << "Please select a command:\n" << std::endl;
		std::cout << "1) Clean\t2) Full Speed Ahead\t3) Stop\t100) Exit\n" << std::endl;
		std::cin >> instruction;

		switch (instruction)
		{
		case 1:
		{
			std::thread cleaning_thread([&] {
				cleaningCrew.clean();
			});
			cleaning_thread.detach();
			break;
		}
		case 2:
		{
			std::thread accelerate_thread([&] {
				engineCrew.accelerate();
			});
			thread_guard acc_tg(accelerate_thread);
			// @QUESTION: is it necessary to call join() since thread_guard does it during dtor?
			accelerate_thread.join();
			break;
		}
		case 3:
		{
			std::thread stop_thread([&] {
				engineCrew.stopEngine();
			});
			thread_guard stop_tg(stop_thread);
			stop_thread.join();
			break;
		}
		case 100:
			std::cout << "Exiting..." << std::endl;
			break;
		default:
			std::cout << "Invalid Order. Try again.\n" << std::endl;
			break;
		}
	}


	return 0;
	
}
