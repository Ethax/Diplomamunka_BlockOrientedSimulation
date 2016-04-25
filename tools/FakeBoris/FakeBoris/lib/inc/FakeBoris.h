#ifndef INC_FAKEBORIS_H_
#define INC_FAKEBORIS_H_

#include <inc\AbstractScheduler.h>
#include <AsyncSerial\AsyncSerial.h>

#include <string>
#include <map>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <cctype>
#include <bitset>
#include <mutex>
#include <boost\lexical_cast.hpp>

/**
 * @brief A WinFACT szoftvercsomag Block-oriented Simulation (BORIS) szoftver�nek soros porton kereszt�li kommunik�ci�s jeleit ut�noz�
 * oszt�ly. K�pes elhitetni a CO3715-1H t�pus� I/O interf�sszel, hogy az eml�tett szoftverrel kommunik�l.
 */
class FakeBoris: public AbstractScheduler {
	/**
	 * A fogadott jelek kijelz�s�nek sora a konzol koordin�tarendszer�ben.
	 */
	SHORT inputLine;

	/**
	 * A kik�ld�tt jelek kijelz�s�nek sora a konzol koordin�tarendszer�ben.
	 */
	SHORT outputLine;

	/**
	 * A soros port neve a Windows rendszerben.
	 */
	const std::string serialPortName;

	/**
	 * A soros port adat�tviteli sebess�ge.
	 */
	const unsigned int serialPortBaudRate;

	/**
	 * A soros ponton kereszt�li aszinkron kommunik�ci�t megval�s�t� objektum.
	 */
	CallbackAsyncSerial serialPort;

	/**
	 * A 16-bites kimenetet t�rol� bitmez�.
	 */
	std::bitset<16> output;
	
	/**
	 * A kimenet �r�s�t �s olvas�s�t k�lcs�n�sen kiz�r� adattag.
	 */
	std::recursive_mutex rwLocker;

	/**
	 * A konzolk�perny�re t�bb sz�lr�l t�rt�n� �r�si m�veleteket k�lcs�n�sen kiz�r� adattag.
	 */
	std::recursive_mutex printLocker;

	/**
	 * @brief Kirajzolja a konzolk�perny�re az I/O interf�szt �br�zol� karakterk�pet.
	 */
	inline void drawIODisplay();

	/**
	 * @brief Beolvassa a kimenet sorsz�m�t �s a be�ll�tani k�v�nt �rt�ket a felhaszn�l�t�l.
	 *
	 * @param stop_char A beolvas�st megszak�t�, azonnal kil�ptet� karakter.
	 * @return A meghat�rozott kimenet sorsz�ma �s a be�ll�tani k�v�nt �rt�k karakteresen t�m�r�tve egy sztringben.
	 */
	std::string getNewSetting(char stop_char);

	/**
	 * @brief Ki�r egy karaktert a megadott poz�ci�val a konzolk�perny�re.
	 *
	 * @param x A ki�rat�s X ir�ny� helyzete a konzol koordin�tarendszer�ben.
	 * @param y A ki�rat�s Y ir�ny� helyzete a konzol koordin�tarendszer�ben.
	 * @param character A ki�ratni k�v�nt karakter.
	 */
	void printCharAt(SHORT x, SHORT y, wchar_t character);

	/**
	 * @brief Ism�tl�d� id�k�z�nk�nt elk�ldi az I/O interf�sznek a be�ll�tott kimenetet �s lek�rdezi a bemenet �llapot�t.
	 */
	virtual void performScheduledAction();

	/**
	 * @brief Fogadja a soros kommunik�ci�s porton �rkez� �zenetet az I/O interf�szt�l �s a feldolgoz�st k�vet�en m�dos�tja a
	 * konzolk�perny�n a bemenetek �llapotait.
	 *
	 * @param read_data A soros kommunik�ci�s portra �rkezett �zenet els� elem�re mutat� pointer.
	 * @param data_size A soros kommunik�ci�s portra �rkezett �zenet hossza.
	 */
	void readIODevice(const char* input, size_t data_size);

	/**
	 * @brief Az oszt�ly alap�rtelmezett konstruktor�nak tilt�sa.
	 */
	FakeBoris() = delete;

	/**
	 * @brief Az oszt�ly m�sol� konstruktor�nak tilt�sa.
	 */
	FakeBoris(FakeBoris&) = delete;

	/**
	 * @brief Az �rt�kad� oper�tor tilt�sa az oszt�lyra.
	 */
	FakeBoris& operator=(const FakeBoris&) = delete;

public:
	/**
	 * @brief Az oszt�ly konstruktora.
	 *
	 * @param device A soros port neve a Windows rendszerben.
	 * @param baud_rate A soros port adat�tviteli sebess�ge.
	 */
	FakeBoris(const std::string& device, unsigned int baud_rate);

	/**
	 * @brief Az oszt�ly virtu�lis destruktora.
	 */
	virtual ~FakeBoris();

	/**
	 * @brief Kirajzolja a konzolk�perny�re az I/O interf�szt �br�zol� karakterk�pet, inicializ�lja a soros kommunik�ci�s portot �s
	 * elind�tja az I/O interf�sszel t�rt�n� ism�tl�d� adatcser�t.
	 */
	void start();

	/**
	 * @brief Le�ll�tja a soros ponton kereszt�li ism�tl�d� adatcser�t �s lez�rja a soros kommunik�ci�s portot.
	 */
	void stop();

	/**
	 * @brief Ism�tl�d�en bek�ri a felhaszn�l�t�l a kimenetek �llapotainak megv�ltoztat�s�t el�id�z� param�tereket a megadott meg�ll�t�
	 * karakterig.
	 */
	void readUntil(char stop_char);

private:
	/**
	 * Az I/O interf�sz kimeneteit be�ll�t� utas�t�s.
	 */
	static const unsigned char CMD_WRITE_OUTPUT;

	/**
	 * Az I/O interf�sz bemeneteit beolvas� utas�t�s.
	 */
	static const unsigned char CMD_READ_INPUT;

	/**
	 * Az I/O interf�sz test�t �br�zol� karakterk�p sz�ne.
	 */
	static const WORD BODY_COLOR;

	/**
	 * Az I/O interf�sz csatlakoz�it �br�zol� karakterk�p sz�ne.
	 */
	static const WORD PIN_COLOR;

	/**
	 * Az I/O interf�sz csatlakoz�inak a neveit �br�zol� karakterk�p sz�ne.
	 */
	static const WORD IO_NAMES_COLOR;

	/**
	 * Az I/O interf�sz kimenet�n �s bemenet�n lev� jeleket �rbr�zol� karakterk�p sz�ne.
	 */
	static const WORD SIGNAL_COLOR;

	/**
	 * @brief Adatfolyam effektor, amelyik m�dos�tja a konzolk�perny� el�ter�nek a sz�n�t a kurzor aktu�lis poz�ci�j�t�l kezd�d�en.
	 */
	class color;

	/**
	 * @brief Adatfolyam effektor, amelyik ism�tli az �tadott sztring ki�rat�s�t a megadott �rt�knek megfelel� alkalommal.
	 */
	class repeat;

	/**
	 * @brief Hozz�f�r�st biztos�t a priv�t adattagokhoz az adatfolyam oper�tor sz�m�ra.
	 *
	 * @param out A m�dos�tani k�v�nt adatfolyamra hivatkoz� referencia.
	 * @param number A konzolk�perny� el�ter�nek a sz�n�t m�dos�t� effektor.
	 * @return A m�dos�tott adatfolyamra hivatkoz� referencia.
	 */
	friend std::ostream& operator<<(std::ostream& out, FakeBoris::color number);

	/**
	 * @brief Hozz�f�r�st biztos�t a priv�t adattagokhoz az adatfolyam oper�tor sz�m�ra.
	 *
	 * @param out A m�dos�tani k�v�nt adatfolyamra hivatkoz� referencia.
	 * @param number Az �tadott sztring ism�tl�d� ki�rat�s�t el�id�z� effektor.
	 * @return A m�dos�tott adatfolyamra hivatkoz� referencia.
	 */
	friend std::ostream& operator<<(std::ostream& out, FakeBoris::repeat text);
};

/**
 * @brief Adatfolyam effektor, amelyik m�dos�tja a konzolk�perny� el�ter�nek a sz�n�t a kurzor aktu�lis poz�ci�j�t�l kezd�d�en.
 */
class FakeBoris::color {
	/**
	 * Az el�t�r be�ll�tani k�v�nt sz�nk�dja.
	 */
	WORD color_num;

public:
	/**
	 * @brief Az oszt�ly konstruktora.
	 *
	 * @param color_number Az el�t�r be�ll�tani k�v�nt sz�nk�dja.
	 */
	color(WORD color_number) : color_num(color_number) {}

	/**
	 * @brief M�dos�tja a konzolk�perny� el�ter�nek a sz�n�t �s m�dos�tatlanul visszaadja az �tadott adatfolyam referenci�t.
	 *
	 * @param os A m�dos�tani k�v�nt adatfolyamra hivatkoz� referencia.
	 * @return A m�dos�tott adatfolyamra hivatkoz� referencia.
	 */
	std::ostream& operator()(std::ostream& os) const;
};

/**
 * @brief Adatfolyam effektor, amelyik ism�tli az �tadott sztring ki�rat�s�t a megadott �rt�knek megfelel� alkalommal.
 */
class FakeBoris::repeat {
	/**
	 * Az ism�tl�sek sz�ma.
	 */
	unsigned int repeat_number;

	/**
	 * Az ism�telni k�v�nt sztringre hivatkoz� referencia.
	 */
	const std::string& text;

public:
	/**
	 * @brief Az oszt�ly konstruktora.
	 *
	 * @param text_to_repeat Az ism�telni k�v�nt sztringre hivatkoz� referencia.
	 * @param repeat_n_times Az ism�tl�sek sz�ma.
	 */
	repeat(const std::string& text_to_repeat, unsigned int repeat_n_times) : repeat_number(repeat_n_times), text(text_to_repeat) {}

	/**
	 * @brief Az ism�tl�sek sz�m�nak megfelel�en ism�tl�d�en r��rja az �tadott adatfolyamra az ism�telni k�v�nt sztringet.
	 *
	 * @param os A m�dos�tani k�v�nt adatfolyamra hivatkoz� referencia.
	 * @return A m�dos�tott adatfolyamra hivatkoz� referencia.
	 */
	std::ostream& operator()(std::ostream& os) const;
};

#endif /* INC_FAKEBORIS_H_ */