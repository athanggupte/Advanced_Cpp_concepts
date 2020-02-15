#include <stdio.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "Queue.h"

void ThreadLoop(const std::shared_ptr<Queue>& , int );
void AddValue(const std::shared_ptr<Queue>& , int );
void Terminate(const std::shared_ptr<Queue>& );

std::condition_variable cv;
std::vector<std::thread> threadPool;

int main(int argc, char const *argv[])
{
	std::shared_ptr<Queue> queue = std::make_shared<Queue>();

	int numThreads = std::thread::hardware_concurrency();
	printf("Number of Threads: %d\n", numThreads);

	for(int i=0; i<numThreads-1; i++) {
		threadPool.push_back(std::thread(ThreadLoop, queue, i));
	}

	AddValue(queue, 10);
	AddValue(queue, 20);
	AddValue(queue, 30);
	AddValue(queue, 40);

	Terminate(queue);

	for(int i=0; i<numThreads-1; i++) {
		threadPool.at(i).join();
	}

	return 0;
}

void ThreadLoop(const std::shared_ptr<Queue>& queue, int id) {
	while (true)
	{
		int val;
		{
			std::unique_lock<std::mutex> lock(queue->m_QueueMutex);
			cv.wait(lock, [queue]{ return !queue->IsEmpty(); });

			val = queue->GetFront()->getValue();
			queue->PopItem();
		}
		if(val < 0) return;
		printf("<%d> : %d\n", id, val);
	}
	
}

void AddValue(const std::shared_ptr<Queue>& queue, int value) {
	{
		std::unique_lock<std::mutex> lock(queue->m_QueueMutex);
		queue->PushItem(std::make_shared<QueueItem>(value));
	}
	cv.notify_one();
}

void Terminate(const std::shared_ptr<Queue>& queue) {
	{
		std::unique_lock<std::mutex> lock(queue->m_QueueMutex);
		for(auto& thread : threadPool) {
			queue->PushItem(std::make_shared<QueueItem>(-1));
		}
	}
	cv.notify_all();
}