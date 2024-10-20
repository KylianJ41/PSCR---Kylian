#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "Queue.h"
#include "Job.h"
#include "Barrier.h"
#include "Pool.h"

void producer(pr::Queue<int> &q, int id, int num_elems)
{
    for (int i = 0; i < num_elems; ++i)
    {
        int *item = new int(i + id * 1000);
        q.push(item);
        std::cout << "Producer " << id << " pushed " << *item << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(pr::Queue<int> &q, int id)
{
    while (true)
    {
        int *item = q.pop();
        if (item == nullptr)
        {
            std::cout << "Consumer " << id << " finished" << std::endl;
            break;
        }
        std::cout << "Consumer " << id << " popped " << *item << std::endl;
        delete item;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}
/* MAIN JUSQU'A Q 9 INCLU
int main()
{
    pr::Queue<int> q(10);
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    for (int i = 0; i < 3; ++i)
        producers.emplace_back(producer, std::ref(q), i, 5);

    for (int i = 0; i < 2; ++i)
        consumers.emplace_back(consumer, std::ref(q), i);

    // Laisser le temps aux producteurs de finir
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // On passe en mode non bloquant
    std::cout << "Set non-blocking mode" << std::endl;
    q.setBlocking(false);

    for (auto &t : producers)
        t.join();

    for (auto &t : consumers)
        t.join();

    std::cout << "All threads finished" << std::endl;
}
*/
/*
int main()
{
    int res = 0;

    pr::SleepJob *job = new pr::SleepJob(42, &res);

    job->run();

    delete job;

    std::cout << "Deleted job, result is = " << res << std::endl;
}
*/

int main()
{
    const int K = 4;  // nb threads ds pool
    const int N = 10; // nb jobs a soumettre

    pr::Pool pool(20);
    pool.start(K);

    pr::Barrier barrier(N);
    std::vector<int> res(N);

    for (int i = 0; i < N; ++i)
    {
        pr::Job *job = new pr::SleepJob(i, &res[i], barrier);
        pool.submit(job);
    }

    // On attend que tous les jobs soient terminés
    barrier.waitFor();
    // Puis une fois sûr qu'il soit terminés, on peut join les threads
    // et nettoyer le pool.
    pool.stop();

    std::cout << "Results:" << std::endl;
    for (int i = 0; i < N; ++i)
        std::cout << "Job " << i << " result: " << res[i] << std::endl;
}

/*
Q1)
L'utilisateur doit alloc les elems lui-même, cela dit
notre contructeur les detruira lui même s'ils n'ont pas été pop.

Q5)
Car une fois qu'un thread entre en wait, il relache le mutex (puis s'endort).

Q6)
Avec une seule condition, tous les threads en attente sur not_empty seront
reveillés. Alors qu'avec deux conditions, on peut reveiller seulement ceux
qui attendent sur, par ex, not_empty.

Q7)
Une seule condition : si plusieurs threads peuvent agir, un seul sera reveillé.
Et si le thread reveillé est un producteur, alors que la Queue est déjà pleine,
il ne pourra rien faire et se remettra donc en attente.
Le consommateur reste bloqué, c'est problématique.

Q8)
Si on modif en faisant
if (empty())
    not_empty.wait(lock);

On ne gère plus le cas des spurious wakeups (reveil sans notification,
ce qui peut arriver avec les condition_variable). On doit revérifier
la condition après un reveil. Car entre le moment où on on vérifie empty()
et le moment où on entre dans wait(), la condition peut changer.
Voire si la queue devient non vide juste après la vérif mais avant le wait, on
pourrait attendre indéfiniment.
*/