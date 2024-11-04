#include "Semaphore.h"
#include <iostream>
#include <thread>
#include <vector>

void pinger(int n, pr::Semaphore &sema, pr::Semaphore &semb)
{
	for (int i = 0; i < n; i++)
	{
		sema.acquire(1);
		std::cout << "ping ";
		semb.release(1);
	}
}

void ponger(int n, pr::Semaphore &sema, pr::Semaphore &semb)
{
	for (int i = 0; i < n; i++)
	{
		semb.acquire(1);
		std::cout << "pong ";
		sema.release(1);
	}
}

int main1()
{
	// a faire varier si on le souhaite
	const int NBITER = 20;
	pr::Semaphore sema(1);
	pr::Semaphore semb(0);
	std::vector<std::thread> threads;

	// TODO : instancier un thread pinger et un thread ponger avec n=NBITER
	threads.emplace_back(pinger, NBITER, std::ref(sema), std::ref(semb));
	threads.emplace_back(ponger, NBITER, std::ref(sema), std::ref(semb));

	// TODO : terminaison et sortie propre
	for (auto &t : threads)
		t.join();

	return 0;
}
