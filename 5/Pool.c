#include "Pool.h"

namespace pr
{
    Pool::Pool(int qsize) : queue(qsize), running(false) {}

    void Pool::start(int nbthread)
    {
        if (running.load())
            return;

        running.store(true);
        for (int i = 0; i < nbthread; ++i) // this miss under ?
            threads.emplace_back(&Pool::poolWorker, this, i);
    }

    void Pool::poolWorker(int idWorker)
    {
        while (running.load())
        {
            Job *job = queue.pop();
            // Si nullptr, signifie que queue non-bloquante et donc qu'on
            // souhaite terminé. Sinon, pop() va nous endormir sur la cond not_empty
            if (job == nullptr)
            {
                std::stringstream msg;
                msg << "Worker " << idWorker << " finished" << std::endl;
                break;
            }
            else
            {
                job->run();
                delete job;
            }
        }
    }

    void Pool::submit(Job *job)
    {
        queue.push(job);
    }

    void Pool::stop()
    {
        queue.setBlocking(false);
        running.store(false);
        for (auto &t : threads)
        {
            if (t.joinable())
                t.join();
        }

        Job *remainingJob;
        while ((remainingJob = queue.pop()) != nullptr)
        {
            delete remainingJob;
        }

        threads.clear();
    }

    Pool::~Pool()
    {
        if (running.load())
            stop();
    }
}