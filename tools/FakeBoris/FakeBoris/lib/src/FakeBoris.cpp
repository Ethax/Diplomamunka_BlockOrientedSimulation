#include <inc\FakeBoris.h>

/*
 * Kirajzolja a konzolk�perny�re az I/O interf�szt �br�zol� karakterk�pet.
 */
inline void FakeBoris::drawIODisplay() {
	/* A bemenetek �llapotait jelz� karaktersor ki�rat�sa v�ltakoz� sz�nekkel. */
	std::cout << color(PIN_COLOR);
	for(int i = 0; i < 16; i++)
		std::cout << (i == 8 ? " " : "") << "(" << color(SIGNAL_COLOR) << "0" << color(PIN_COLOR) << ")" << (i < 15 ? " " : "");
	std::cout << std::endl;
	
	/* A bemenetek neveit tartalmaz� �s a neveket a hozz�juk tartoz� �llapotokkal �sszek�t� karaktersorok ki�rat�sa. */
	std::cout << repeat(" |  ", 8) << " " << repeat(" |  ", 8) << std::endl;
	std::cout << color(IO_NAMES_COLOR) << "I15 I14 I13 I12 I11 I10 I09 I08  I07 I06 I05 I04 I03 I02 I01 I00" << std::endl;

	/* Az I/O interf�sz test�t �br�zol� karaktersorok ki�rat�sa. */
	std::cout << color(BODY_COLOR) << std::string(64, '-') << std::endl << "|" << std::string(24, ' ');
	std::cout << "Fake BORIS 1.0" << std::string(24, ' ') << "|" << std::endl << std::string(64, '-') << std::endl;

	/* A kimenetek neveit tartalmaz� �s a neveket a hozz�juk tartoz� �llapotokkal �sszek�t� karaktersorok ki�rat�sa. */
	std::cout << color(IO_NAMES_COLOR) << "Q15 Q14 Q13 Q12 Q11 Q10 Q09 Q08  Q07 Q06 Q05 Q04 Q03 Q02 Q01 Q00" << std::endl;
	std::cout << color(PIN_COLOR) << repeat(" |  ", 8) << " " << repeat(" |  ", 8) << std::endl;

	/* A kimenetek �llapotait jelz� karaktersor ki�rat�sa v�ltakoz� sz�nekkel. */
	for(int i = 0; i < 16; i++)
		std::cout << (i == 8 ? " " : "") << "(" << color(SIGNAL_COLOR) << "0" << color(PIN_COLOR) << ")" << (i < 15 ? " " : "");
	std::cout << std::endl;
}

/*
 * Beolvassa a kimenet sorsz�m�t �s a be�ll�tani k�v�nt �rt�ket a felhaszn�l�t�l.
 */
std::string FakeBoris::getNewSetting(char stop_char) {
	std::string setting;

	/* A lenyomott billenty�k beolvas�sa, am�g a felt�teleknek megfelel� �rt�kek bevitele meg nem t�rt�nik. */
	while(setting.size() < 3) {
		int c = _getch();

		/* A kimenet bet�jel�nek �s az egyenl�s�gjel automatikus ki�rat�sa, am�g a bevitt sz�m�rt�kek a kimenet sorsz�m�t jelentik. */
		if(std::isdigit(c) && setting.size() < 2) {
			setting.push_back(static_cast<char>(c));
			std::cout << (setting.size() == 1 ? "Q" : "") << static_cast<char>(c) << (setting.size() == 2 ? " = " : "");
		}
		/* A kimenetre ker�l� �j �rt�k ki�rat�sa, ha a kimenet sorsz�ma m�r meg lett adva. */
		else if(setting.size() == 2 && (c == '0' || c == '1')) {
			setting.push_back(static_cast<char>(c));
			std::cout << static_cast<char>(c);
		}
		/* A bevitel megszak�t�sa �s az alkalmaz�s le�ll�t�s�t jelz� eredm�ny visszaad�sa a megadott megszak�t� billenyt� lenyom�s�ra. */
		else if(c == std::tolower(stop_char) || c == std::toupper(stop_char)) {
			return "exit";
		}
		/* A bevitel megszak�t�sa �s �res eredm�ny visszaad�sa a t�rl�s billenty�k valamelyik�nek lenyom�s�ra. */
		else if(c == 127 || c == 8) {
			return "";
		}
	}

	/* A bevitel felf�ggeszt�se a sort�r�s vagy a sz�k�z billeny�k lenyom�s�ig, illetve a t�rl�s billenty�k valamelyik�nek lenyom�s�ra
	�res eredm�ny visszaad�sa. */
	int key_code = _getch();
	while(!std::isspace(key_code)) {
		if(key_code == 127 || key_code == 8)
			return "";
		key_code = _getch();
	} 
	return setting;
}

/*
 * Ki�r egy karaktert a megadott poz�ci�val a konzolk�perny�re.
 */
void FakeBoris::printCharAt(SHORT x, SHORT y, wchar_t character) {
	std::unique_lock<std::recursive_mutex> guard(printLocker);
	COORD pos = { x, y };
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), &character, 1, pos, &dwBytesWritten);
}

/*
 * Ism�tl�d� id�k�z�nk�nt elk�ldi az I/O interf�sznek a be�ll�tott kimenetet �s lek�rdezi a bemenet �llapot�t.
 */
void FakeBoris::performScheduledAction() {
	/* A kimenetet �br�zol� 16-bites bitmez� felbont�sa k�t 8-bites eg�szre. */
	unsigned char outputHighByte, outputLowByte;
	{
		std::unique_lock<std::recursive_mutex> guard(rwLocker);

		unsigned int outputValue = output.to_ulong();
		outputHighByte = outputValue >> 8;
		outputLowByte = outputValue;
	}

	/* Az I/O interf�sz kimenet�t a 16-bites bitmez� �rt�k�re m�dos�t� �s a bemenet�t lek�rdez� utas�t�ssorozat �ssze�ll�t�sa �s kik�ld�se
	a soros kommunik�ci�s porton. */
	try {
		unsigned char command[] = { CMD_WRITE_OUTPUT, outputHighByte, outputLowByte, CMD_READ_INPUT };
		serialPort.write(reinterpret_cast<char*>(command), 4);
	}
	catch(boost::system::system_error& e) {
		std::cout << "Az ism�tl�d� �zenetk�ld�s k�zben hiba l�pett fel." << e.what() << std::endl;
	}
}

/*
 * Fogadja a soros kommunik�ci�s porton �rkez� �zenetet az I/O interf�szt�l �s a feldolgoz�st k�vet�en m�dos�tja a konzolk�perny�n a
 * bemenetek �llapotait.
 */
void FakeBoris::readIODevice(const char* input, size_t data_size) {
	const unsigned char* uchar_ptr = reinterpret_cast<const unsigned char*>(input);

	/* A fogadott v�lasz �talak�t�sa bitmez�v� �s konzolk�perny�n szerepl� �rt�kek m�dos�t�sa a fogadott v�lasznak megfelel�en, amennyiben
	annak hossza pontosan k�t b�jt. */
	if(data_size == 2) {
		std::bitset<16> inputBits((uchar_ptr[0] << 8) + uchar_ptr[1]);

		for(int i = 0; i < 16; i++)
			printCharAt((15 - i) * 4 + ((i < 8) ? 2 : 1), inputLine, inputBits.test(i) ? '1' : '0');
	}
}

/*
 * Az oszt�ly konstruktora.
 */
FakeBoris::FakeBoris(const std::string& device, unsigned int baud_rate) :
	AbstractScheduler(),
	inputLine(0),
	outputLine(0),
	serialPortName(device),
	serialPortBaudRate(baud_rate) {}

/*
 * Az oszt�ly virtu�lis destruktora.
 */
FakeBoris::~FakeBoris() {
	/* A soros ponton kereszt�li ism�tl�d� adatcsere le�ll�t�sa �s a soros kommunik�ci�s port lez�r�sa.*/
	try {
		stop();
	}
	catch(...) {
		std::cout << "A soros kommunik�ci� lez�r�sa k�zben hiba l�pett fel." << std::endl;
	}
}

/*
 * Kirajzolja a konzolk�perny�re az I/O interf�szt �br�zol� karakterk�pet, inicializ�lja a soros kommunik�ci�s portot �s elind�tja az I/O
 * interf�sszel t�rt�n� ism�tl�d� adatcser�t.
 */
void FakeBoris::start() {
	/* A konzolk�perny� aktu�lis be�ll�t�sainak elt�rol�sa a k�s�bbi vissza�ll�t�shoz. */
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

	/* A bemenetek �s a kimenetek soraira mutat� Y koordin�t�k elt�rol�sa �s az I/O interf�szt �br�zol� karakterk�p kirajzol�sa. */
	inputLine = csbi.dwCursorPosition.Y;
	outputLine = csbi.dwCursorPosition.Y + 8;
	drawIODisplay();

	/* A kor�bban elt�rolt konzolk�perny� be�ll�t�sok vissza�ll�t�sa. */
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), csbi.wAttributes);
	std::cout << std::endl;

	/* A soros kommunik�ci�s port megnyit�sa, a fogadott v�laszokat feldolgoz� visszah�v� f�ggv�ny be�ll�t�sa �s az ism�tl�d� adatcsere
	elind�t�sa. */
	try {
		serialPort.open(serialPortName, serialPortBaudRate);
		serialPort.setCallback(boost::bind(&FakeBoris::readIODevice, this, _1, _2));
		
		setPeriod(250);
		AbstractScheduler::start();
	}
	catch(boost::system::system_error& e) {
		std::cout << "A soros kommunik�ci�s port inicializ�l�sa k�zben hiba l�pett fel: " << e.what() << std::endl;
	}
}

/*
 * Le�ll�tja a soros ponton kereszt�li ism�tl�d� adatcser�t �s lez�rja a soros kommunik�ci�s portot.
 */
void FakeBoris::stop() {
	AbstractScheduler::stop();

	serialPort.clearCallback();
	if(serialPort.isOpen())
		serialPort.close();
}

/*
 * Ism�tl�d�en bek�ri a felhaszn�l�t�l a kimenetek �llapotainak megv�ltoztat�s�t el�id�z� param�tereket a megadott meg�ll�t� karakterig.
 */
void FakeBoris::readUntil(char stop_char) {
	std::string setting;
	do {
		/* A kor�bban bevitt adatok t�rl�se a konzolk�perny� beviteli sor�b�l. */
		std::cout << '\r' << std::string(10, ' ') << "\r> ";
		
		/* �j param�terek bek�r�se �s a m�velet megszak�t�sa, ha a felhaszn�l� a megszak�t� billenty�t �t�tte le. */
		setting = getNewSetting(stop_char);
		if(setting == "exit")
			break;
		else if(setting == "")
			continue;

		/* A bek�rt param�terek megjelen�t�se �s elt�rol�sa a kimenet bitmez�j�ben. */
		SHORT x = boost::lexical_cast<SHORT>(setting.substr(0, 2));
		if(x <= 15) {
			printCharAt((15 - x) * 4 + ((x < 8) ? 2 : 1), outputLine, setting.at(2));
			{
				std::unique_lock<std::recursive_mutex> guard(rwLocker);
				output.set(x, setting.at(2) == '1');
			}
		}
	} while(setting != "exit");
	std::cout << std::endl;
}

/*
 * Az I/O interf�sz kimeneteit be�ll�t� utas�t�s inicializ�l�sa.
 */
const unsigned char FakeBoris::CMD_WRITE_OUTPUT = 0xba;

/*
 * Az I/O interf�sz bemeneteit beolvas� utas�t�s inicializ�l�sa.
 */
const unsigned char FakeBoris::CMD_READ_INPUT = 0xb9;

/*
 * Az I/O interf�sz test�t �br�zol� karakterk�p sz�n�nek inicializ�l�sa.
 */
const WORD FakeBoris::BODY_COLOR = 0x07;

/*
 *  Az I/O interf�sz csatlakoz�it �br�zol� karakterk�p sz�n�nek inicializ�l�sa.
 */
const WORD FakeBoris::PIN_COLOR = 0x03;

/*
 * Az I/O interf�sz csatlakoz�inak a neveit �br�zol� karakterk�p sz�n�nek inicializ�l�sa.
 */
const WORD FakeBoris::IO_NAMES_COLOR = 0x06;

/*
 * Az I/O interf�sz kimenet�n �s bemenet�n lev� jeleket �rbr�zol� karakterk�p sz�n�nek inicializ�l�sa.
 */
const WORD FakeBoris::SIGNAL_COLOR = 0x0E;

/*
 * M�dos�tja a konzolk�perny� el�ter�nek a sz�n�t �s m�dos�tatlanul visszaadja az �tadott adatfolyam referenci�t.
 */
std::ostream& FakeBoris::color::operator()(std::ostream& os) const {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_num);
	return os;
}

/*
 * Az ism�tl�sek sz�m�nak megfelel�en ism�tl�d�en r��rja az �tadott adatfolyamra az ism�telni k�v�nt sztringet.
 */
std::ostream& FakeBoris::repeat::operator()(std::ostream& os) const {
	for(unsigned int i = 0; i < repeat_number; i++)
		os << text;
	return os;
}

/*
 * M�dos�tja a konzolk�perny� el�ter�nek a sz�n�t �s m�dos�tatlanul visszaadja az �tadott adatfolyam referenci�t.
 */
std::ostream& operator<<(std::ostream& out, FakeBoris::color number) {
	return number(out);
}

/*
 * Az ism�tl�sek sz�m�nak megfelel�en ism�tl�d�en r��rja az �tadott adatfolyamra az ism�telni k�v�nt sztringet.
 */
std::ostream& operator<<(std::ostream& out, FakeBoris::repeat text) {
	return text(out);
}