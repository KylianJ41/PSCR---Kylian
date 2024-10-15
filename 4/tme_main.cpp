#include "compte.h"

void effectuerTransfers(Banque &banque, size_t N_COMPTES, size_t N_TRANSFERS, int thread_id)
{
    // random_device est une source d'entropie pour les générateurs de nombres pseudo-aléatoires
    std::random_device rd;
    std::mt19937 gen(rd() + thread_id);
    std::uniform_int_distribution<> dist_comptes(0, N_COMPTES - 1);
    std::uniform_int_distribution<> dist_montant(1, 100);
    std::uniform_int_distribution<> dist_sleep(0, 20);

    for (size_t i = 0; i < N_TRANSFERS; ++i)
    {
        size_t deb = dist_comptes(gen);
        size_t cred = deb;

        while (cred == deb)
            cred = dist_comptes(gen);

        unsigned int m = dist_montant(gen);

        banque.transfert(deb, cred, m);

        std::this_thread::sleep_for(std::chrono::milliseconds(dist_sleep(gen)));
    }
}

int main()
{

    const size_t N_COMPTES = 10;
    const size_t SOLDE_INITIAL = 1000;
    const size_t N_THREADS = 5;
    const size_t N_TRANSFERS = 1000;

    Banque banque(N_COMPTES, SOLDE_INITIAL);

    std::vector<std::thread> threads;
    threads.reserve(N_THREADS);

    // for (size_t i = 0; i < N_THREADS; ++i)
    //     threads.emplace_back(effectuerTransfers, std::ref(banque), N_COMPTES, N_TRANSFERS, static_cast<int>(i));

    /*for (auto &th : threads)
    {
        if (th.joinable())
            th.join();
    }*/

    banque.comptabiliser(N_COMPTES * SOLDE_INITIAL);

    banque.afficherSoldes();
}

// Q7 : non car il pourrait y avoir des transferts/debits/credits qui se font en meme temps
