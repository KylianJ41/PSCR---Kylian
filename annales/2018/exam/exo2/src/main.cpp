#include "RDV.h"

#include <vector>
#include <thread>
#include <iostream>
#include <chrono>
#include <random>

// Fichier à completer

void worker(int id, pr::RendezVous &rdv)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(100, 1000);

	for (int i = 0; i < 3; i++)
	{
		std::cout << "Début " << id << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(dis(gen))));

		rdv.meet();

		std::cout << "Fin " << id << std::endl;
	}
}

int main()
{
	// a faire varier
	const int N = 4;

	// construire un RendezVous pour N participants
	pr::RendezVous rdv(N);

	// instancier des threads
	std::vector<std::thread> threads;
	for (int i = 1; i < N; ++i)
	{
		threads.emplace_back(worker, i, std::ref(rdv));
	}

	// participer au rendez vous trois fois
	worker(0, std::ref(rdv));

	// sortie propre
	for (auto &t : threads)
		t.join();

	return 0;
}
