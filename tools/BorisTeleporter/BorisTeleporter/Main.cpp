#ifdef _WIN32
#include <Windows.h>
#endif
#include <boost\filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>

namespace fs = boost::filesystem;

/**
 * @class recursive_directory_range
 * @brief Rekurz�van bej�rhat� k�nyvt�rfa oszt�ly.
 *
 * Ennek az oszt�lynak a seg�ts�g�vel rekurz�van bej�rhat� az alkalmaz�s aktu�lis �tvonal�n
 * tal�lhat� �sszes k�nyvt�r, ami megk�nny�ti a BORIS f�jlok felder�t�s�t.
 */
class recursive_directory_range {
	fs::path p_;	
	typedef fs::recursive_directory_iterator iterator;

public:
	recursive_directory_range(fs::path p) : p_(p) {}

	iterator begin() { return fs::recursive_directory_iterator(p_); }
	iterator end() { return fs::recursive_directory_iterator(); }
};

/**
 * @brief Megkeresi az �tadott sztringben a megadott eltol�st�l sz�m�tott els� olyan karakterp�rost,
 * amelyik egy abszol�t Windos �tvonal kezdet�re utal. Tal�lat eset�n megk�s�rli visszaadni az
 * eggyel kisebb index� karakternek - a meghajt� bet�jel�nek - az index�t.
 *
 * @param fullString Egy sztringre mutat� referencia, amelyikben a keres�s t�rt�nik.
 * @param offset Az eltol�s m�rt�ke, ahonnan kezd�dik a keres�s.
 * @return Tal�lat eset�n a meghat� bet�jel�nek indexe, ellenkez� esetben std::wstring::npos.
 */
std::wstring::size_type findPathBeginning(const std::wstring& fullString, std::wstring::size_type offset = 0) {
	std::wstring::size_type position = fullString.find(L":\\", offset);
	if(position != std::wstring::npos)
		position = (position > 0) ? position - 1 : position;
	return position;
}

/**
 * @brief Megkeresi az �tadott sztringben a megadott eltol�st�l sz�m�tott legk�zelebbi
 * f�jlkiterjeszt�st a megadott kiterjeszt�sgy�jtem�ny alapj�n. Tal�lat eset�n visszaadja a
 * f�jlkiterjeszt�s ut�ni els� indexet.
 *
 * @param extensionSet A keresend� f�jlkiterjeszt�sek gy�jtem�nye.
 * @param fullString Egy sztringre mutat� referencia, amelyikben a keres�s t�rt�nik.
 * @param offset Az eltol�s m�rt�ke, ahonnan kezd�dik a keres�s.
 * @return Tal�lat eset�n a f�jlkiterjeszt�s ut�ni els� index, ellenkez� esetben std::wstring::npos.
 */
std::wstring::size_type findPathEnd(const std::set<std::wstring>& extensionSet, const std::wstring& fullString, std::wstring::size_type offset = 0) {
	std::wstring::size_type firstOccurrence = std::wstring::npos;
	for(std::wstring extension : extensionSet) {
		std::wstring::size_type position = fullString.find(extension, offset);
		if(position != std::wstring::npos) {
			position += extension.size();
			if(firstOccurrence == std::wstring::npos || position < firstOccurrence)
				firstOccurrence = position;
		}
	}
	return firstOccurrence;
}

/**
 * @brief A program bel�p�si pontja, visszat�r�se a program fut�s�nak v�g�t jelenti.
 *
 * @return Megfelel� lefut�s eset�n nulla.
 */
int main() {
#ifdef _WIN32
	SetConsoleOutputCP(1252);
#endif
	/* Az ismertet� �s az alkalmaz�s jelenlegi �tvonal�nak megjelen�t�se. */
	std::wcout << "BORIS Teleporter [Verzi� 1.1]" << std::endl;
	std::wcout << "Mikl�s �rp�d (c) 2016" << std::endl << std::endl;

	fs::path applicationPath = fs::current_path();
	std::wcout << "Jelenlegi k�nyvt�r:" << std::endl << "  " << applicationPath << std::endl;

	/* A jelenlegi k�nyvt�rban tal�lhat� BORIS specifikus f�jlok felder�t�se �s a tal�latok
	�sszegy�jt�se egy kulcs-�rt�k t�rol�ba. */
	std::map<std::wstring, std::wstring> filenamesWithPath;
	const std::set<std::wstring> extensions = { L".bmp", L".BMP", L".bsy", L".BSY", L".fab", L".FAB", L".sbl", L".SBL"};
	
	for(fs::directory_entry entry : recursive_directory_range(applicationPath)) {
		if(extensions.find(entry.path().extension().c_str()) != extensions.end())
			filenamesWithPath[entry.path().filename().c_str()] = entry.path().c_str();
	}

	/* Nulla tal�lat eset�n a megjegyz�s megjelen�t�se �s a program fut�s�nak befejez�se. */
	if(filenamesWithPath.empty()) {
		std::wcout << "A k�nyvt�rban nem tal�lhat�k BORIS specifikus f�jlok." << std::endl;
		return 0;
	}

	/* A felder�t�s eredm�ny�nek megjelen�t�se �s meger�s�t�s k�r�se a f�jlok fel�l�r�s�hoz. */
	std::wcout << "A k�nyvt�rban tal�lhat� BORIS specifikus f�jlok sz�ma:" << std::endl;
	std::wcout << "  " << filenamesWithPath.size() << std::endl << std::endl;
	std::wcout << "A k�vetkez� m�velet fel�l fogja �rni ezeket a f�jlokat. Folytatja? (I/n)" << std::endl;
	wchar_t answer = 0; std::wcin.get(answer);
	if(answer != L'I')
		return 0;
	
	/* Minden egyes BORIS specifikus f�jl �tvizsg�l�sa �s a hivatkoz�sok fel�l�r�sa. */
	for(std::map<std::wstring, std::wstring>::iterator fileEntry = filenamesWithPath.begin(); fileEntry != filenamesWithPath.end(); fileEntry++) {
		/* A k�pf�jlok �tugr�sa. */
		if(fs::path(fileEntry->first).extension() == L".bmp" || fs::path(fileEntry->first).extension() == L".BMP")
			continue;

		/* Biztons�gi m�solat k�sz�t�se a jelenlegi f�jlr�l. */
		if(fs::exists(fileEntry->second + L".orig"))
			fs::remove(fileEntry->second + L".orig");
		fs::rename(fs::path(fileEntry->second), fs::path(fileEntry->second + L".orig"));
		
		/* A biztons�gi m�solat megnyit�sa olvas�sra �s �j f�jl nyit�sa �r�sra a jelenlegi f�jl
		eredeti nev�vel. */
		std::wifstream borisOrigFile(fileEntry->second + L".orig");
		std::wofstream borisFile(fileEntry->second);
		
		/* A kiterjeszt�sgy�jtem�nyben meghat�rozott f�jlokra mutat� hivatkoz�sok keres�se a
		jelenlegi f�jl minden egyes sor�ban. */
		std::wstring line;
		std::wcout << std::endl << "Hivatkoz�sok a " << fileEntry->second << " f�jlban:" << std::endl;
		while(std::getline(borisOrigFile, line)) {
			/* Hivatkoz�sok keres�se a jelenlegi sorban. */
			std::wstring::size_type pathBegin = findPathBeginning(line);
			std::wstring::size_type pathEnd = findPathEnd(extensions, line);

			while(pathBegin != std::wstring::npos && pathEnd != std::wstring::npos) {
				/* A tal�lat megjelen�t�se �s fel�l�r�sa, amennyiben a kor�bbi k�nyvt�rfelder�t�s
				tal�lt a hivatkoz�ssal megegyez� nev� f�jlt. */
				std::wstring::size_type previousSize = line.size();
				fs::path reference(line.substr(pathBegin, pathEnd - pathBegin));
				std::wcout << "  " << reference << std::endl;
					
				std::map<std::wstring, std::wstring>::iterator it = filenamesWithPath.find(reference.filename().c_str());
				if(it != filenamesWithPath.end())
					line.replace(pathBegin, pathEnd - pathBegin, it->second);
						
				if(previousSize < line.size())
					pathEnd += line.size() - previousSize;
				else if(previousSize > line.size())
					pathEnd -= previousSize - line.size();
				
				/* �j hivatkoz�s keres�se a jelenlegi sorban a kor�bbi tal�latt�l kezdve. */
				pathBegin = findPathBeginning(line, pathEnd);
				pathEnd = findPathEnd(extensions, line, pathEnd);
			}
			
			/* A jelenlegi sor bele�r�sa az �r�sra megnyitott f�jlba. */
			borisFile << line << std::endl;
		}

		/* A biztons�gi m�solat �s a jelenlegi f�jl lez�r�sa. */
		borisOrigFile.close();
		borisFile.close();
	}

	/* A program fut�s�nak befejez�se. */
	return 0;
}