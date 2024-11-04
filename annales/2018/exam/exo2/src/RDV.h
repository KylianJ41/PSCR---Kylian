#ifndef EXO2_RDV_H_
#define EXO2_RDV_H_

#include <condition_variable>
#include <mutex>

namespace pr
{

    // classe RendezVous
    class RendezVous
    {
    private:
        int N;
        int count;
        std::mutex mtx;
        std::condition_variable cv;

    public:
        RendezVous(int n);
        void meet();
    };

}

#endif /* EXO2_RDV_H_ */
