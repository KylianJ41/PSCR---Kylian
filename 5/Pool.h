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

        void poolWorker(int idWorker);

    public:
        Pool(int qsize);
        void start(int nbthread);
        void submit(Job *job);
        void stop();
        ~Pool();
    };
} // namespace pr