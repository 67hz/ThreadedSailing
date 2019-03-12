#include "pch.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <queue>

class QueueWorker {
	std::queue<int> taskQueue;
	bool runLoop = true;

public:

	std::thread _t;
	explicit QueueWorker()
	{
		std::cout << "QueueWorker constructor" << std::endl;
		_t = std::thread(&QueueWorker::startWatchLoop, this);
	}

	void startWatchLoop()
	{
		while (runLoop)
		{
			if (!taskQueue.empty())
			{
				popJobFromQueue();
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
			else
			{
				std::cout << std::endl;
				std::cout << "queue is empty on threadId _t.get_id(): " << _t.get_id() << std::endl;
				std::cout << "queue is empty on std::this_thread::get_id: " << std::this_thread::get_id() << std::endl;
				std::cout << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			}
		}

	}

	void stopWatchLoop()
	{
		runLoop = false;
	}


	virtual void jobDelegator(int jobId) { }

	void pushJobToQueue(int jobId)
	{
		std::cout << "pushJobToQueue " << jobId << std::endl;
		taskQueue.push(jobId);
	}

	void popJobFromQueue()
	{
		const auto &jobId = taskQueue.front();
		std::cout << "popJobFromQueue: " << jobId << std::endl;
		taskQueue.pop();
		jobDelegator(jobId);
	}

	~QueueWorker()
	{
		if (_t.joinable())
			_t.join();
	}

	const std::thread & getThread()
	{
		return _t;
	}

};

class CleaningCrew : public QueueWorker
{
public:

	void jobDelegator(int jobId) override
	{
		if (jobId == 1)
		{
			if (_t.joinable())
			{
				std::cout << _t.get_id() << " is joinable" << std::endl;
				std::cout << std::this_thread::get_id() << " is joinable" << std::endl;
				_t = std::thread(&CleaningCrew::clean, this);
				_t.detach();
			}
		}
	}

	CleaningCrew()
	{
		std::cout << "Cleaning Crew ctor: " << _t.get_id() << std::endl;

	}

	void clean()
	{
		printf("Cleaning in progress... thread_id: %d\n", _t.get_id());
		printf("Cleaning in progress... thread_id: %d\n", std::this_thread::get_id());
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		printf("Cleaning complete.\n");
	}
};

class EngineCrew : public QueueWorker
{
public:
	EngineCrew()
	{
		std::cout << "Engine crew ctor: " << _t.get_id() << std::endl;
	}

	void jobDelegator(int jobId) override
	{
		if (jobId == 2)
		{
			if (_t.joinable())
			{
				std::cout << _t.get_id() << " is joinable" << std::endl;
				std::cout << std::this_thread::get_id() << " is joinable" << std::endl;
				_t = std::thread(&EngineCrew::accelerate, this);
				_t.detach();
			}
		}
		if (jobId == 3)
		{
			if (_t.joinable())
			{
				std::cout << _t.get_id() << " is joinable" << std::endl;
				std::cout << std::this_thread::get_id() << " is joinable" << std::endl;
				_t = std::thread(&EngineCrew::stopEngine, this);
				_t.detach();
			}
		}
	}

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
			cleaningCrew.pushJobToQueue(1);
			break;
		}
		case 2:
		{
			engineCrew.pushJobToQueue(2);
			break;
		}
		case 3:
		{
			engineCrew.pushJobToQueue(3);
			break;
		}
		case 100:
			std::cout << "Exiting..." << std::endl;
			cleaningCrew.stopWatchLoop();
			engineCrew.stopWatchLoop();
			break;
		default:
			std::cout << "Invalid Order. Try again.\n" << std::endl;
			break;
		}
	}


	return 0;
	
}
