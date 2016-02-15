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
 * @brief Rekurzívan bejárható könyvtárfa osztály.
 *
 * Ennek az osztálynak a segítségével rekurzívan bejárható az alkalmazás
 * aktuális útvonalán található összes könyvtár, ami megkönnyíti a
 * BORIS fájlok felderítését.
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
 * @brief A program belépési pontja, visszatérése a program futásának végét
 * jelenti.
 *
 * @return Megfelelõ lefutás esetén nulla.
 */
int main() {
#ifdef _WIN32
	SetConsoleOutputCP(1252);
#endif
	/* Az ismertetõ és az alkalmazás jelenlegi útvonalának megjelenítése. */
	std::wcout << "BORIS Teleport [Verzió 1.0]" << std::endl;
	std::wcout << "Miklós Árpád (c) 2016" << std::endl << std::endl;

	boost::filesystem::path applicationPath = boost::filesystem::current_path();
	std::wcout << "Jelenlegi könyvtár:" << std::endl << "  " << applicationPath << std::endl;

	/* A jelenlegi könyvtárban található BORIS fájlok felderítése és a
	találatok összegyûjtése egy kulcs-érték tárolóba. */
	std::map<std::wstring, std::wstring> filenamesWithPath;
	for(boost::filesystem::directory_entry entry : recursive_directory_range(applicationPath)) {
		if(entry.path().extension() == L".SBL" || entry.path().extension() == L".BSY")
			filenamesWithPath[entry.path().filename().c_str()] = entry.path().c_str();
	}

	/* Nulla találat esetén a megjegyzés megjelenítése és a program futásának
	befejezése. */
	if(filenamesWithPath.empty()) {
		std::wcout << "A könyvtárban nem találhatók BORIS Rendszerfájlok (.BSY) vagy BORIS Szuperblokk fájlok (.SBL)." << std::endl;
		return 0;
	}

	/* A felderítés eredményének megjelenítése és megerõsítés kérése a fájlok
	felülírásához. */
	std::wcout << "A könyvtárban található BORIS Rendszerfájlok (.BSY) és BORIS Szuperblokk fájlok (.SBL) száma:" << std::endl;
	std::wcout << "  " << filenamesWithPath.size() << std::endl << std::endl;
	std::wcout << "A következõ mûvelet felül fogja írni ezeket a fájlokat. Folytatja? (I/n)" << std::endl;
	wchar_t answer = 0; std::wcin.get(answer);
	if(answer != L'I')
		return 0;
	
	/* Minden egyes BORIS fájl átvizsgálása és a hivatkozások felülírása. */
	for(std::map<std::wstring, std::wstring>::iterator fileEntry = filenamesWithPath.begin(); fileEntry != filenamesWithPath.end(); fileEntry++) {
		/* Biztonsági másolat készítése a jelenlegi fájlról. */
		if(boost::filesystem::exists(fileEntry->second + L".orig"))
			boost::filesystem::remove(fileEntry->second + L".orig");
		boost::filesystem::rename(boost::filesystem::path(fileEntry->second), boost::filesystem::path(fileEntry->second + L".orig"));
		
		/* A biztonsági másolat megnyitása olvasásra és új fájl nyitása
		írásra a jelenlegi fájl eredeti nevével. */
		std::wifstream borisOrigFile(fileEntry->second + L".orig");
		std::wofstream borisFile(fileEntry->second);
		
		/* BORIS Rendszerfájlra vagy BORIS Szuperblokk fájlra mutató
		hivatkozások keresése a jelenlegi fájl minden egyes sorában. */
		std::wstring line;
		std::wcout << std::endl << "Hivatkozások a " << fileEntry->second << " fájlban:" << std::endl;
		while(std::getline(borisOrigFile, line)) {
			boost::filesystem::path pathFromLine(line);
			
			/* A jelenlegi sor megvizsgálása felülírási célból, amennyiben az
			egy érvényes hivatkozás és egy BORIS fájlra mutat. */
			if(pathFromLine.extension() == L".SBL" || pathFromLine.extension() == L".BSY") {
				std::wcout << "  " << pathFromLine << std::endl;
				
				/* A jelenlegi sor felülírása a hivatkozott fájl új útvonalával,
				amennyiben azt a korábbi felderítés is megtalálta. Ellenkezõ
				esetben a sor módosítás nélküli beleírása az írásra megnyitott
				fájlba. */
				std::map<std::wstring, std::wstring>::iterator it = filenamesWithPath.find(pathFromLine.filename().c_str());
				if(it != filenamesWithPath.end())
					borisFile << it->second << std::endl;
				else
					borisFile << line << std::endl;
			}
			/* A jelenlegi sor beleírása az írásra megnyitott fájlba. */
			else {
				borisFile << line << std::endl;
			}
		}

		/* A biztonsági másolat és a jelenlegi fájl lezárása. */
		borisOrigFile.close();
		borisFile.close();
	}

	/* A program futásának befejezése. */
	return 0;
}