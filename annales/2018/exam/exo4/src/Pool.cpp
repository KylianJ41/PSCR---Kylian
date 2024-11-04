#include "Pool.h"

namespace pr
{

	// fonction passee a ctor de  thread
	void poolWorker(Queue<Job> *queue, bool *finish)
	{
		while (!*finish)
		{
			Job *j = queue->pop();

			if (j == nullptr)
			{
				// on est non bloquant = il faut sortir
				queue->decrementeTraiteTache();
				return;
			}

			j->run();
			delete j;
			queue->decrementeTraiteTache();
		}
	}

	void Pool::start(int nbthread)
	{
		threads.reserve(nbthread);
		for (int i = 0; i < nbthread; i++)
		{
			threads.emplace_back(poolWorker, &queue, &stopping);
			queue.incrementeTraiteTache();
		}
	}
	// Ajout a la fin du TD, pour la terminaison
	void Pool::stop()
	{
		stopping = true;
		queue.setBlocking(false);
		for (auto &t : threads)
		{
			t.join();
		}
		threads.clear();
	}
	Pool::~Pool()
	{
		stop();
	}
	void Pool::submit(Job *job)
	{
		queue.push(job);
	}

	void Pool::waitUntilDone()
	{
		// A COMPLETER
		queue.waitUntilDone();
	}

}
