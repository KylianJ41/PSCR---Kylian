#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>

namespace pr
{

	class Semaphore
	{
		int compteur;

		std::mutex mtx;
		std::condition_variable cv;

	public:
		Semaphore(int initial) : compteur(initial) {}
		void acquire(int qte)
		{
			std::unique_lock<std::mutex> lock(mtx);
			cv.wait(lock, [&]
					{ return compteur >= qte; });
			compteur -= qte;
		}
		void release(int qte)
		{
			std::unique_lock<std::mutex> lock(mtx);
			compteur += qte;
			cv.notify_all();
		}
	};

}
