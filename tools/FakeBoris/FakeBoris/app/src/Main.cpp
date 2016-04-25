#include <inc\FakeBoris.h>

#include <Windows.h>
#include <iostream>
#include <string>
#include <boost\lexical_cast.hpp>

/**
 * @brief A program bel�p�si pontja.
 *
 * @param argc A parancssori argumentumok sz�ma.
 * @param argv A parancssori argumentumok t�mbje.
 * @return Megfelel� lefut�s eset�n nulla.
 */
int main(int argc, char** argv) {
	/* A konzol karakterk�dol�s�nak megv�ltoztat�sa az �kezetes magyar karakterek megfelel� kijelz�se �rdek�ben. */
	SetConsoleOutputCP(1252);

	/* A parancssori argumentumok beolvas�sa �s �rtelmez�se. */
	std::string device, baud;
	if(argc != 3) {
		std::cout << "Haszn�lat: " << argv[0] << " --port=<port neve> --baud=<adat�tviteli sebess�g>" << std::endl;
		exit(0);
	}
	else {
		for(int i = 1; i < argc; i++) {
			std::string arg(argv[i]);
			if(arg.find("--port=") != std::string::npos) {
				device = arg.substr(arg.find('=') + 1);
			}
			else if(arg.find("--baud=") != std::string::npos) {
				baud = arg.substr(arg.find('=') + 1);
			}
			else {
				std::cout << "�rv�nytelen argumentum, pr�b�ld �jra." << std::endl;
				exit(0);
			}
		}
	}

	/* A haszn�latot bemutat� �zenet ism�telt megjelen�t�se, ha a port neve vagy az adat�tviteli sebess�g nem lett megadva. */
	if(device.empty() || baud.empty()) {
		std::cout << "Haszn�lat: " << argv[0] << " --port=<port neve> --baud=<adat�tviteli sebess�g>" << std::endl;
		exit(0);
	}

	/* A szoftver verzi�sz�m�nak �s a k�sz�t� nev�nek megjelen�t�se. */
	std::cout << "Fake BORIS [Verzi� 1.0]" << std::endl;
	std::cout << "Mikl�s �rp�d (c) 2016" << std::endl << std::endl;

	/* A WinFACT szoftvercsomag Block-oriented Simulation (BORIS) szoftver�nek viselked�s�t ut�nz� objektum l�trehoz�sa �s a soros
	porton kereszt�li kommunik�ci� inicializ�l�sa. */
	FakeBoris fakeBoris(device,	boost::lexical_cast<unsigned int>(baud));
	fakeBoris.start();

	/* A s�g� megjelen�t�se �s az �llapotok megv�ltoztat�s�t kiv�lt� param�terek ism�telt bek�r�se a meg�ll�t� karakterig. */
	std::cout << "�j �llapot be�ll�t�sa: Q[00-15] = [0|1] # A \"Q\" �s az egyenl�s�gjel automatikusan megjelennek." << std::endl;
	std::cout << "Kil�p�s: [q|Q]" << std::endl;
	std::cout << "T�rl�s: [Backspace] # Minden bevitelt egyszerre t�r�l." << std::endl;
	fakeBoris.readUntil('q');

	/* A szoftver fut�s�nak befejez�se. */
	return 0;
}