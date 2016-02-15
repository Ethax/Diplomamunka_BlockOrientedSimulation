#ifdef _WIN32
#include <Windows.h>
#endif
#include <boost\filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <regex>

/**
 * @class recursive_directory_range
 * @brief Rekurz�van bej�rhat� k�nyvt�rfa oszt�ly.
 *
 * Ennek az oszt�lynak a seg�ts�g�vel rekurz�van bej�rhat� az alkalmaz�s
 * aktu�lis �tvonal�n tal�lhat� �sszes k�nyvt�r, ami megk�nny�ti a
 * BORIS f�jlok felder�t�s�t.
 */
class recursive_directory_range {
	boost::filesystem::path p_;	
	typedef boost::filesystem::recursive_directory_iterator iterator;

public:
	recursive_directory_range(boost::filesystem::path p) : p_(p) {}

	iterator begin() { return boost::filesystem::recursive_directory_iterator(p_); }
	iterator end() { return boost::filesystem::recursive_directory_iterator(); }
};

/**
 * @brief A program bel�p�si pontja, visszat�r�se a program fut�s�nak v�g�t
 * jelenti.
 *
 * @return Megfelel� lefut�s eset�n nulla.
 */
int main() {
#ifdef _WIN32
	SetConsoleOutputCP(1252);
#endif
	/* Az ismertet� �s az alkalmaz�s jelenlegi �tvonal�nak megjelen�t�se. */
	std::wcout << "BORIS Teleport [Verzi� 1.0]" << std::endl;
	std::wcout << "Mikl�s �rp�d (c) 2016" << std::endl << std::endl;

	boost::filesystem::path applicationPath = boost::filesystem::current_path();
	std::wcout << "Jelenlegi k�nyvt�r:" << std::endl << "  " << applicationPath << std::endl;

	/* A jelenlegi k�nyvt�rban tal�lhat� BORIS f�jlok felder�t�se �s a
	tal�latok �sszegy�jt�se egy kulcs-�rt�k t�rol�ba. */
	std::map<std::wstring, std::wstring> filenamesWithPath;
	for(boost::filesystem::directory_entry entry : recursive_directory_range(applicationPath)) {
		if(entry.path().extension() == L".SBL" || entry.path().extension() == L".BSY")
			filenamesWithPath[entry.path().filename().c_str()] = entry.path().c_str();
	}

	/* Nulla tal�lat eset�n a megjegyz�s megjelen�t�se �s a program fut�s�nak
	befejez�se. */
	if(filenamesWithPath.empty()) {
		std::wcout << "A k�nyvt�rban nem tal�lhat�k BORIS Rendszerf�jlok (.BSY) vagy BORIS Szuperblokk f�jlok (.SBL)." << std::endl;
		return 0;
	}

	/* A felder�t�s eredm�ny�nek megjelen�t�se �s meger�s�t�s k�r�se a f�jlok
	fel�l�r�s�hoz. */
	std::wcout << "A k�nyvt�rban tal�lhat� BORIS Rendszerf�jlok (.BSY) �s BORIS Szuperblokk f�jlok (.SBL) sz�ma:" << std::endl;
	std::wcout << "  " << filenamesWithPath.size() << std::endl << std::endl;
	std::wcout << "A k�vetkez� m�velet fel�l fogja �rni ezeket a f�jlokat. Folytatja? (I/n)" << std::endl;
	wchar_t answer = 0; std::wcin.get(answer);
	if(answer != L'I')
		return 0;
	
	/* Minden egyes BORIS f�jl �tvizsg�l�sa �s a hivatkoz�sok fel�l�r�sa. */
	for(std::map<std::wstring, std::wstring>::iterator fileEntry = filenamesWithPath.begin(); fileEntry != filenamesWithPath.end(); fileEntry++) {
		/* Biztons�gi m�solat k�sz�t�se a jelenlegi f�jlr�l. */
		if(boost::filesystem::exists(fileEntry->second + L".orig"))
			boost::filesystem::remove(fileEntry->second + L".orig");
		boost::filesystem::rename(boost::filesystem::path(fileEntry->second), boost::filesystem::path(fileEntry->second + L".orig"));
		
		/* A biztons�gi m�solat megnyit�sa olvas�sra �s �j f�jl nyit�sa
		�r�sra a jelenlegi f�jl eredeti nev�vel. */
		std::wifstream borisOrigFile(fileEntry->second + L".orig");
		std::wofstream borisFile(fileEntry->second);
		
		/* BORIS Rendszerf�jlra vagy BORIS Szuperblokk f�jlra mutat�
		hivatkoz�sok keres�se a jelenlegi f�jl minden egyes sor�ban. */
		std::wstring line;
		std::wcout << std::endl << "Hivatkoz�sok a " << fileEntry->second << " f�jlban:" << std::endl;
		while(std::getline(borisOrigFile, line)) {
			boost::filesystem::path pathFromLine(line);
			
			/* A jelenlegi sor megvizsg�l�sa fel�l�r�si c�lb�l, amennyiben az
			egy �rv�nyes hivatkoz�s �s egy BORIS f�jlra mutat. */
			if(pathFromLine.extension() == L".SBL" || pathFromLine.extension() == L".BSY") {
				std::wcout << "  " << pathFromLine << std::endl;
				
				/* A jelenlegi sor fel�l�r�sa a hivatkozott f�jl �j �tvonal�val,
				amennyiben azt a kor�bbi felder�t�s is megtal�lta. Ellenkez�
				esetben a sor m�dos�t�s n�lk�li bele�r�sa az �r�sra megnyitott
				f�jlba. */
				std::map<std::wstring, std::wstring>::iterator it = filenamesWithPath.find(pathFromLine.filename().c_str());
				if(it != filenamesWithPath.end())
					borisFile << it->second << std::endl;
				else
					borisFile << line << std::endl;
			}
			/* A jelenlegi sor bele�r�sa az �r�sra megnyitott f�jlba. */
			else {
				borisFile << line << std::endl;
			}
		}

		/* A biztons�gi m�solat �s a jelenlegi f�jl lez�r�sa. */
		borisOrigFile.close();
		borisFile.close();
	}

	/* A program fut�s�nak befejez�se. */
	return 0;
}