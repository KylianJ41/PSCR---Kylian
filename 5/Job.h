#pragma once

#include "Barrier.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <sstream>

namespace pr
{

    class Job
    {
    public:
        // le = 0 oblige aux types concrets de Job à implémenter run()
        virtual void run() = 0;
        // virtual sur le destructeur pour permettre la destruction polymorphique
        // c'est à dire que si on a un pointeur de type Job qui pointe sur un objet
        // de type SleepJob, on veut que le destructeur de SleepJob soit appelé
        virtual ~Job() {};
    };

    // Job concret : exemple

    class SleepJob : public Job
    {
    private:
        int arg;
        int *ret;
        Barrier &barrier;

        int calcul(int v)
        {
            std::stringstream msg;

            msg << "Computing for arg =" << v << std::endl;
            std::cout << msg.str();
            msg.clear();

            // traiter un gros calcul
            std::this_thread::sleep_for(std::chrono::seconds(1));
            int ret = v % 255;

            msg << "Obtained for arg =" << arg << " result " << ret << std::endl;
            std::cout << msg.str();
            return ret;
        }

    public:
        SleepJob(int arg, int *ret, Barrier &barrier) : arg(arg), ret(ret), barrier(barrier) {}

        void run() override
        {
            *ret = calcul(arg);
            barrier.done();
        }
        ~SleepJob() override = default;
    };

}