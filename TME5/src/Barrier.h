#pragma once

#include <mutex>
#include <condition_variable>

namespace pr
{
    class Barrier
    {
    private:
        std::mutex mtx;
        int count;
        int N;
        std::condition_variable all_here;

    public:
        Barrier(int n) : count(0), N(n) {}

        void done()
        {
            std::unique_lock<std::mutex> lock(mtx);
            count++;
            if (count == N)
                all_here.notify_all();
        }

        void waitFor()
        {
            std::unique_lock<std::mutex> lock(mtx);
            all_here.wait(lock, [this]
                          { return count == N; });
        }
    };
} // namespace pr