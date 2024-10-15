#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <mutex>

class Compte
{
    int solde;
    // mutable keyword permet de modifier une variable membre const, et getSolde est const
    mutable std::recursive_mutex m;

public:
    Compte(int solde = 0) : solde(solde) {}

    // Constructeur par copie
    Compte(const Compte &other) : solde(0)
    {
        std::lock_guard<std::recursive_mutex> lock(other.m);
        solde = other.solde;
    }

    /*
    Lorsque vous avez deux objets Compte, chacun avec son propre mutex,
    et que vous souhaitez copier les données de l'un vers l'autre, il est
    essentiel de verrouiller les deux mutex pour garantir que ni l'un ni
    l'autre ne modifie solde pendant la copie. Cependant, verrouiller deux
    mutex simultanément peut introduire un risque de deadlock si deux threads
    tentent de verrouiller les mutex dans des ordres différents

    On va donc std::scoped_lock qui permet de verrouiller plusieurs mutex
    sans risque de deadlock
    */
    Compte &operator=(const Compte &other)
    {
        if (this == &other)
            return *this;

        std::scoped_lock lock(m, other.m);
        solde = other.solde;
        return *this;
    }

    int getSolde() const
    {
        // std::lock_guard<std::mutex> lock(m);
        std::unique_lock<std::recursive_mutex> lock(m);
        return solde;
    }

    void lock() const { m.lock(); }
    void unlock() const { m.unlock(); }
    bool try_lock() const { return m.try_lock(); }

private:
    // ________________________________________________
    // le mutex sera déverrouillé à la fin du scope dans lequel il est
    // déclaré, donc ici a la fin de la func. Si déclaré dans un if, à la fin du if

    // unique_lock donne plus de contrôle, notamment la possibilité de le déverrouiller
    // il reste préférable d'utiliser lock_guard si on a pas besoin de déverrouiller
    // car il est plus rapide
    void crediter(size_t val)
    {
        // std::lock_guard<std::mutex> lock(m);
        std::unique_lock<std::recursive_mutex> lock(m);
        solde += val;
    }

    bool debiter(unsigned int val)
    {
        // std::lock_guard<std::mutex> lock(m);
        std::unique_lock<std::recursive_mutex> lock(m);
        bool doit = (solde >= val);
        if (doit)
            solde -= val;
        else
            std::cout << "Solde insuffisant" << std::endl;

        return doit;
    }

    friend class Banque;
};

class Banque
{
    typedef std::vector<Compte> comptes_t;
    comptes_t comptes;

public:
    Banque(size_t ncomptes, size_t solde) : comptes(ncomptes, Compte(solde)) {}

    void transfert(size_t deb, size_t cred, unsigned int val)
    {
        if (deb == cred)
            return;

        Compte &debiteur = comptes[deb];
        Compte &crediteur = comptes[cred];

        if (deb < cred)
        {
            std::lock(debiteur, crediteur);
            // adopt_lock permet de dire que les mutex sont déjà verrouillés
            // et qu'on doit simplement adopter la propriété de ces verrous
            // notamment la responsabilité de les déverrouiller
            std::lock_guard<std::recursive_mutex> lock_deb(debiteur.m, std::adopt_lock);
            std::lock_guard<std::recursive_mutex> lock_cred(crediteur.m, std::adopt_lock);

            if (debiteur.solde >= val)
            {
                debiteur.solde -= val;
                crediteur.solde += val;
            }
            else
            {
                std::cout << "Solde insuffisant pour le compte " << deb << std::endl;
            }
        }
        else
        {
            std::lock(crediteur, debiteur);
            std::lock_guard<std::recursive_mutex> lock_deb(crediteur.m, std::adopt_lock);
            std::lock_guard<std::recursive_mutex> lock_cred(debiteur.m, std::adopt_lock);

            if (debiteur.solde >= val)
            {
                debiteur.solde -= val;
                crediteur.solde += val;
            }
            else
            {
                std::cout << "Solde insuffisant pour le compte " << deb << std::endl;
            }
        }
    }

    size_t size() const { return comptes.size(); }

    void afficherSoldes() const
    {
        for (size_t i = 0; i < comptes.size(); ++i)
            std::cout << "Compte " << i << " : " << comptes[i].getSolde() << std::endl;
    }

    bool comptabiliser(int attendu) const
    {
        int bilan = 0;
        int id = 0;
        /* On suit RAII (Resource acquisition is initialisation
         * Plutot que de unlock les mutex manuellement, on utilise
         * des unique_lock qui se déverrouillent automatiquement
         * a la fin du scope (fin de la fonction ici)
         */
        std::vector<std::unique_lock<std::recursive_mutex>> locks;
        locks.reserve(comptes.size());

        for (const auto &compte : comptes)
        {
            locks.emplace_back(compte.m);

            if (compte.getSolde() < 0)
            {
                std::cout << "Compte " << id << " en négatif" << std::endl;
            }
            bilan += compte.getSolde();
            id++;
        }
        if (bilan != attendu)
        {
            std::cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << std::endl;
        }
        else
        {
            std::cout << "Bilan comptable correct" << std::endl;
        }
        return bilan == attendu;
    }
};