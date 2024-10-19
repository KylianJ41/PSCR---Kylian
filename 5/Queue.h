#pragma once

#include <cstring>
#include <cstddef>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace pr
{
    template <typename T>
    class Queue
    {
        T **tab;
        const size_t allocsize;
        size_t begin;
        size_t sz;
        std::mutex mtx;

        /*
            not_full est utilisé pour bloquer les threads qui veulent push
            alors que la queue est pleine. Ces threads devront attendre sur
            cette condition dans push() si la queue est pleine. Une notification
            est envoyée dans pop() quand un elt est enlevé et qu'il y a à nouveau
            de la place.
        */
        std::condition_variable not_full;
        /*
            not_empty est utilisé pour bloquer les threads qui veulent pop
            alors que la queue est vide. Ces threads devront attendre sur
            cette condition dans pop() si la queue est vide. Une notification
            est envoyée dans push() quand un elt est ajouté et qu'il y a nouveau
            des éléments.
        */
        std::condition_variable not_empty;

        std::atomic<bool> is_blocking;

    public:
        Queue(size_t maxsize) : allocsize(maxsize), begin(0), sz(0), is_blocking(true)
        {
            tab = new T *[maxsize];
            memset(tab, 0, maxsize * sizeof(T *));
        }

        size_t size() const
        {
            std::lock_guard<std::mutex> lock(mtx);
            return sz;
        }

        void push(T *elt)
        {
            std::unique_lock<std::mutex> lock(mtx);

            not_full.wait(lock, [this]
                          { return !full() || !is_blocking.load(); });

            if (!full())
            {
                tab[(begin + sz) % allocsize] = elt;
                sz++;
                not_empty.notify_one();
            }
            // Si mode non bloquant, on ne fait rien
        }

        T *pop()
        {
            std::unique_lock<std::mutex> lock(mtx);

            not_empty.wait(lock, [this]
                           { return !empty() || !is_blocking.load(); });

            if (empty())
                return nullptr;

            T *ret = tab[begin];
            tab[begin] = nullptr;
            sz--;
            begin = (begin + 1) % allocsize;
            not_full.notify_one();
            return ret;
        }

        void setBlocking(bool isBlocking)
        {
            is_blocking.store(isBlocking);
            not_empty.notify_all();
            not_full.notify_all();
        }

        ~Queue()
        {

            for (size_t i = 0; i < sz; ++i)
            {
                size_t ind = (begin + i) % allocsize;
                delete tab[ind];
            }
            delete[] tab;
        }

        bool full() const
        {
            return sz == allocsize;
        }

        bool empty() const
        {
            return sz == 0;
        }
    };
} // namespace pr