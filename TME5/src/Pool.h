#pragma once
#include <vector>
#include <thread>
#include <iostream>
#include <atomic>
#include <sstream>
#include "Queue.h"
#include "Job.h"

namespace pr
{
    class Pool
    {
        Queue<Job> queue;
        std::vector<std::thread> threads;
        std::atomic<bool> running;

        void poolWorker(int idWorker)
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

    public:
        Pool(int qsize) : queue(qsize), running(false) {}

        void start(int nbthread)
        {
            if (running.load())
                return;

            running.store(true);
            for (int i = 0; i < nbthread; ++i)
                threads.emplace_back(&Pool::poolWorker, this, i);
        }

        void submit(Job *job)
        {
            queue.push(job);
        }

        void stop()
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

        ~Pool()
        {
            if (running.load())
                stop();
        }
    };
} // namespace pr
