#include <stdio.h>
#include <thread>
#include <atomic>

#include "Queue.h"

int main(int argc, char const *argv[])
{
	std::shared_ptr<Queue> queue = std::make_shared<Queue>();

	std::atomic_int state(1);

	auto foo = [](std::shared_ptr<Queue> queue, std::atomic_int& state){
		printf("Queue ptr: %lld\n", queue.get());
		for(int i=0; i<10; i++) {
			char name[20];
			std::scanf("Enter name: %s", name);
			std::shared_ptr<QueueItem> item = std::make_shared<QueueItem>(i);
			printf("Added: %d\n", i);
			queue->PushItem(item);
		}
		state = 0;
		printf("\nQuitting Thread #1\n");
	};

	auto bar = [](std::shared_ptr<Queue> queue, std::atomic_int& state){
		printf("Queue ptr: %lld\n", queue.get());
		while(queue->GetLength() > 0 || state == 1) {
			printf("\t\tValue: %d\tLength: %lld\n", queue->GetFront()->getValue(), queue->GetLength());
			queue->PopItem();
		}
		printf("\nQuitting Thread #2\n");
	};

	std::thread data_adder_thread(foo, queue, std::ref(state));
	std::thread data_user_thread(bar, queue, std::ref(state));

	data_adder_thread.join();
	data_user_thread.join();

	if(!data_user_thread.joinable())
		printf("\nThread #2 not joinable\n");

	

	return 0;
}
