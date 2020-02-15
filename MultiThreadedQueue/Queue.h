#include <queue>
#include <mutex>

class QueueItem
{
public:
	QueueItem(int value) : value(value) {}

	int getValue() const { return this->value; }
	void setValue(int value) { this->value = value; }
private:
	int value;
};

class Queue
{
public:
	Queue() : m_Queue(){}

	void PushItem(const std::shared_ptr<QueueItem>& item) { this->m_Queue.push(item); }
	void PushItem(QueueItem* item) { this->m_Queue.push(std::make_shared<QueueItem>(*item)); }
	void PopItem() { this->m_Queue.pop(); }

	const std::shared_ptr<QueueItem>& GetFront() const { return this->m_Queue.front(); }
	const std::shared_ptr<QueueItem>& GetBack() const { return this->m_Queue.back(); }
	uint64_t GetLength() const { return this->m_Queue.size(); }
	bool IsEmpty() const { return this->m_Queue.empty(); }

public:
	std::mutex m_QueueMutex;

private:
	std::queue<std::shared_ptr<QueueItem>> m_Queue;
};