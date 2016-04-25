#ifndef INC_ABSTRACTSCHEDULER_H_
#define INC_ABSTRACTSCHEDULER_H_

#include <mutex>
#include <condition_variable>
#include <thread>

/**
 * @brief Absztrakt �soszt�ly az ism�tl�d� m�veletek folytonos v�grehajt�s�ra. Seg�ts�g�vel minden oszt�ly, amelyik ebb�l az �sb�l is
 * sz�rmazik, egyszer�en �s biztons�gosan tud k�l�n sz�lon ism�tl�d� feladatokat ell�tni.
 */
class AbstractScheduler {
	/**
	 * A tagf�ggv�nyek t�bb sz�lr�l val� el�r�s�t k�lcs�n�sen kiz�r� adattag.
	 */
	std::recursive_mutex threadMutex;

	/**
	 * Az ism�tl�d� m�velet sz�l�nak t�bb sz�lr�l val� le�ll�t�s�t k�lcs�n�sen kiz�r� adattag.
	 */
	std::recursive_mutex joinMutex;

	/**
	 * Az ism�tl�d� m�velet �jb�li v�grehajt�sa el�tti v�rakoztat�st biztos�t� adattag.
	 */
	std::condition_variable_any condition;

	/**
	 * Az ism�tl�d� m�velet �jb�li v�grehajt�sa el�tti v�rakoz�si id�.
	 */
	unsigned long long int period;

	/**
	 * Az ism�tl�d� m�velet sz�l�nak fut�s�t jelz� bit.
	 */
	bool isStarted;

	/**
	 * Az ism�tl�d� m�velet sz�la.
	 */
	std::thread timerThread;
	
	/**
	 * @brief K�l�n sz�lon futva megh�vja az ism�tl�d� m�veletet defini�l� f�ggv�nyt, majd v�rakoz�sba kezd �s a v�rakoz�s letel�s�t
	 * k�vet�en ism�tli �nmag�t.
	 */
	void schedule();

protected:
	/**
	 * @brief Az oszt�ly alap�rtelmezett konstruktora.
	 */
	AbstractScheduler();

	/**
	 * @brief Az oszt�ly m�sol� konstruktor�nak tilt�sa.
	 */
	AbstractScheduler(AbstractScheduler&) = delete;
	
	/**
	 * @brief Az �rt�kad� oper�tor tilt�sa az oszt�lyra.
	 */
	AbstractScheduler& operator=(const AbstractScheduler&) = delete;

	/**
	 * @brief Az oszt�ly virtu�lis destruktora. Meg�ll�tja az ism�tl�d� m�velet sz�l�t �s megv�rja a befejez�d�s�t.
	 */
	virtual ~AbstractScheduler();

	/**
	 * @brief Elind�tja az ism�tl�d� m�velet sz�l�t, amennyiben az m�g nem lett elind�tva.
	 */
	void start();

	/**
	 * @brief Le�ll�tja az ism�tl�d� m�velet sz�l�t, amennyiben az el lett ind�tva.
	 */
	void stop();

	/**
	 * @brief Be�ll�tja a v�rakoz�si id�t az ism�tl�d� m�velet �jb�li v�grehajt�sai k�z�tt.
	 *
	 * @param _duration A v�rakoz�si id� hossza milliszekundumokban.
	 */
	void setPeriod(unsigned long long int _duration);

	/**
	 * @brief Defini�lja az ism�tl�d� m�veletet.
	 */
	virtual void performScheduledAction() = 0;
};

#endif /* INC_ABSTRACTSCHEDULER_H_ */
