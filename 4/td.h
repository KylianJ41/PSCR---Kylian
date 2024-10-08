#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <ctime>

void work(int index)
{
    std::cout << "started " << index << std::endl;
    auto r = ::rand() % 1000; // 0 to 1 sec
    std::this_thread::sleep_for(std::chrono::milliseconds(r));
    std::cout << "finished " << index << std::endl;
}

namespace pr
{
    int createAndWait(int N)
    {
        std::vector<std::thread> threads;
        threads.reserve(N);

        // emplace_bak construit l'objet directement dans le vector
        for (int i = 0; i < N; ++i)
        {
            threads.emplace_back(work, i);
            std::cout << "thread" << i << " created." << std::endl;
        }

        for (int i = 0; i < N; ++i)
        {
            if (threads[i].joinable())
                threads[i].join();

            // cout thread x joined
            std::cout << "thread" << i << " joined." << std::endl;
        }

        return 0;
    }
} // namespace pr