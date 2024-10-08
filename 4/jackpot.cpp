#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

class Compte
{
    int solde;
    // mutable keyword permet de modifier une variable membre const, et getSolde est const
    mutable std::mutex m;

public:
    Compte(int solde = 0) : solde(solde) {}

    // Constructeur par copie
    Compte(const Compte &other) : solde(0)
    {
        std::lock_guard<std::mutex> lock(other.m);
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

    // ________________________________________________
    // le mutex sera déverrouillé à la fin du scope dans lequel il est
    // déclaré, donc ici a la fin de la func. Si déclaré dans un if, à la fin du if

    // unique_lock donne plus de contrôle, notamment la possibilité de le déverrouiller
    // il reste préférable d'utiliser lock_guard si on a pas besoin de déverrouiller
    // car il est plus rapide
    void crediter(size_t val)
    {
        // std::lock_guard<std::mutex> lock(m);
        std::unique_lock<std::mutex> lock(m);
        solde += val;
    }
    int getSolde() const
    {
        // std::lock_guard<std::mutex> lock(m);
        std::unique_lock<std::mutex> lock(m);
        return solde;
    }

    bool debiter(unsigned int val)
    {
        // std::lock_guard<std::mutex> lock(m);
        std::unique_lock<std::mutex> lock(m);
        bool doit = (solde >= val);
        if (doit)
            solde -= val;
        else
            std::cout << "Solde insuffisant" << std::endl;

        return doit;
    }
};

const int JP = 10000;
void jackpot(Compte &c)
{
    for (int i = 0; i < JP; ++i)
        c.crediter(1);
}

void losepot(Compte &c)
{
    for (int i = 0; i < JP / 10; i++)
        c.debiter(10);
}

const int NB_THREAD = 10;
int main()
{
    Compte c(0);
    std::vector<std::thread> threads;
    threads.reserve(NB_THREAD);

    for (int i = 0; i < NB_THREAD; ++i)
        threads.emplace_back(jackpot, std::ref(c));

    for (auto &t : threads)
    { // joinable() permet de savoir si le thread est en cours d'execution, evite d'essayer de join un thread déjà terminé
        if (t.joinable())
            t.join();
    }

    int soldeAttendu = NB_THREAD * JP;
    int soldeFinal = c.getSolde();

    std::cout << "Solde final du compte : " << soldeFinal << std::endl;
    std::cout << "Solde attendu          : " << soldeAttendu << std::endl;

    if (soldeFinal == soldeAttendu)
    {
        std::cout << "Test réussi : Le solde correspond à la valeur attendue." << std::endl;
    }
    else
    {
        std::cout << "Test échoué : Le solde ne correspond pas à la valeur attendue." << std::endl;
    }

    // lancé la func lose_pot sur NB_THREAD threads
    for (int i = 0; i < NB_THREAD; ++i)
        threads.emplace_back(losepot, std::ref(c));

    for (auto &t : threads)
    {
        if (t.joinable())
            t.join();
    }

    // Solde du compte
    std::cout << "Solde final du compte : " << c.getSolde() << std::endl;

    return 0;
}

// Q5 : la var solde étant une variable partagée, on a un problème de concurrence.
// On ne peut donc dire la valeur du solde final. Elle varie d'une exec à l'autre

// Q6 : La concurrence aura mins de chances de se produire car unn thread donné
// se terminera plus rapidement. Donc moins de chances que deux threads modifient
// solde en même temps.

// Q8 : Nous avons sans mutex de la concu. Mais le solde ne peut mathématiquement
// pas tombé en négatif car on fait pour chaque thread 1000 itérations où
// on enlève 10, donc on enlève pour chaque thread au max si on satisfait tjr la
// condi dans débit, 100 000. Or le compte après un appel à jackpot est de 100 000.
// En revanche, il se peut que deux thread essayent de débiter en même temps :
// Ils vont par ex tous les deux voir que le solde est 100 000, vont donc chacun
// faire solde = 100 000 - 10, et le solde final après les 2 deux débits de ces 2
// threads sera 999 990. On aura plus d'argent qu'on ne devrait avoir sur le compte.

// Q11 : avoir atomic + mutex serait redondant, car atomic s'occupe
// déjà d'éviter que deux threads modifient la même variable en même temps.
// Si on utilise pas atomic, il faut utiliser un mutex pour éviter la concurrence.