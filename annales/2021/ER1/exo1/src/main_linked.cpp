// #include "LinkedQueue.hh"
// #include "NaiveBlockingQueue.hh"
#include "LinkedBlockingQueue.hh"
#include <iostream>
#include <vector>

using namespace pr;

#define N 3

void producer(LinkedBlockingQueue<std::string> &queue, int to_produce)
{
    for (int i = 0; i < to_produce; ++i)
    {
        if (queue.put(new std::string("toto")))
        {
            std::cout << "put" << std::endl;
        }
        else
        {
            std::cout << "fail" << std::endl;
        }
    }
}

void consumer(LinkedBlockingQueue<std::string> &queue, int to_consume)
{
    for (int i = 0; i < to_consume; ++i)
    {
        std::string *s = queue.take();
        if (s)
        {
            std::cout << *s << std::endl;
        }
        else
        {
            std::cout << "null" << std::endl;
        }
        delete s;
    }
}

int main()
{
    LinkedBlockingQueue<std::string> queue(100);
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    for (int i = 0; i < N; ++i)
    {
        producers.emplace_back(producer, std::ref(queue), 2000);
        consumers.emplace_back(consumer, std::ref(queue), 2000);
    }

    for (auto &t : producers)
    {
        if (t.joinable())
            t.join();
    }

    std::cout << "All producers finished" << std::endl;

    for (auto &t : consumers)
    {
        if (t.joinable())
            t.join();
    }

    std::cout << "All consumers finished" << std::endl;
}
