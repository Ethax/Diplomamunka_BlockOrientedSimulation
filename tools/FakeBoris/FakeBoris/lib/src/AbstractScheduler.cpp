#include <inc\AbstractScheduler.h>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>

/*
* K�l�n sz�lon futva megh�vja az ism�tl�d� m�veletet defini�l� f�ggv�nyt, majd v�rakoz�sba kezd �s a v�rakoz�s letel�s�t k�vet�en ism�tli
* �nmag�t.
*/
void AbstractScheduler::schedule() {
	std::unique_lock<std::recursive_mutex> threadLock(threadMutex);
	while(isStarted && period > 0) {
		if(condition.wait_for(threadLock, std::chrono::milliseconds(period)) == std::cv_status::timeout)
			performScheduledAction();
	}
}

/*
 * Az oszt�ly alap�rtelmezett konstruktora.
 */
AbstractScheduler::AbstractScheduler() :
	period(0),
	isStarted(false) {}

/*
 * Az oszt�ly virtu�lis destruktora. Meg�ll�tja az ism�tl�d� m�velet sz�l�t �s megv�rja a befejez�d�s�t.
 */
AbstractScheduler::~AbstractScheduler() {
	/* Az ism�tl�d� m�velet sz�l�nak le�ll�t�sa. */
	stop();

	/* A le�ll�tott sz�l befejez�d�s�nek megv�r�sa. */
	std::unique_lock<std::recursive_mutex> joinLock(joinMutex);
	if(timerThread.joinable())
		timerThread.join();
}

/*
 * Elind�tja az ism�tl�d� m�velet sz�l�t, amennyiben az m�g nem lett elind�tva.
 */
void AbstractScheduler::start() {
	std::unique_lock<std::recursive_mutex> threadLock(threadMutex);

	if(!isStarted && period > 0) {
		isStarted = true;
		timerThread = std::thread(&AbstractScheduler::schedule, this);
	}
}

/*
 * Le�ll�tja az ism�tl�d� m�velet sz�l�t, amennyiben az el lett ind�tva.
 */
void AbstractScheduler::stop() {
	std::unique_lock<std::recursive_mutex> threadLock(threadMutex);
	if(isStarted) {
		isStarted = false;
		condition.notify_all();
	}	
}

/*
 * Be�ll�tja a v�rakoz�si id�t az ism�tl�d� m�velet �jb�li v�grehajt�sai k�z�tt.
 */
void AbstractScheduler::setPeriod(unsigned long long int _period) {
	std::unique_lock<std::recursive_mutex> threadLock(threadMutex);
	period = _period;
	condition.notify_all();
}
