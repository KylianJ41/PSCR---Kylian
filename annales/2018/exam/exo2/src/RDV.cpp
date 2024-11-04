#include "RDV.h"

namespace pr
{

    RendezVous::RendezVous(int n) : N(n), count(0) {}

    void RendezVous::meet()
    {
        std::unique_lock<std::mutex> lock(mtx);
        count++;

        if (count == N)
        {
            count = 0;
            cv.notify_all();
        }
        else
        {
            cv.wait(lock, [this]()
                    { return count == 0; });
        }
    }

}
