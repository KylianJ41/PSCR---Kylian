#include "Semaphore.h"
#include <thread>
#include <vector>

// TODO : classe à modifier
class Data
{
	std::vector<int> values;

public:
	int read(pr::Semaphore &sem) const
	{
		int tmp;
		sem.acquire(1);
		if (values.empty())
			tmp = 0;
		else
			tmp = values[rand() % values.size()];
		sem.release(1);
		return tmp;
	}
	void write(pr::Semaphore &sem)
	{
		sem.acquire(256);
		values.push_back(rand());
		sem.release(256);
	}
};

// Pas de modifications dans la suite.
void worker(pr::Semaphore &sem, Data &data)
{
	for (int i = 0; i < 20; i++)
	{
		auto r = ::rand() % 1000; // 0 to 1 sec
		std::this_thread::sleep_for(std::chrono::milliseconds(r));
		if (r % 2)
			auto lu = data.read(sem);
		else
			data.write(sem);
	}
}

int main()
{
	// a faire varier
	const int NBTHREAD = 10;

	// le data partagé
	Data d;
	pr::Semaphore sem(256);

	std::vector<std::thread> threads;
	for (int i = 0; i < NBTHREAD; i++)
		threads.emplace_back(worker, std::ref(sem), std::ref(d));

	for (auto &t : threads)
		t.join();
	return 0;
}
