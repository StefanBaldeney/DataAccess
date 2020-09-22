// DataAccess.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.

#include <iostream>
#include <string>
#include <filesystem>

// sqlite3.c und sqlite3.h wurden manuell in den Projektordner kopiert 
#include <fstream>
#include "sqlite3.h"

typedef struct sqlite3 sqlite3;

using namespace std;
namespace fs = filesystem;

struct Bilddatei
{
	wstring Pfad;
	wstring Dateiname;
	wstring Extension;
	unsigned long Size;
	int Hash;
};

static int callback(void* unNotUsed, int argumentCounter, char** arguments, char** spaltenName)
{
	int i;
	for (i = 0; i < argumentCounter; i++) 
	{}
    return 0;
}

std::vector<Bilddatei> getAlleBilddateien(const string& dirPath)
{
	std::vector<Bilddatei> dateiListe;
	try {
		if (fs::exists(dirPath) && fs::is_directory(dirPath))
		{
			fs::recursive_directory_iterator iter(dirPath);
			fs::recursive_directory_iterator end; // (dirPath.cend());
			
			while (iter != end)
			{
				// ignoriere Ordner und Verknüpfungen
				if (iter->is_regular_file())
				{
					filesystem::path pathToShow(iter->path());

					auto extension = pathToShow.extension().string();
					
					transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
					
					if(extension ==".jpg" ||
						extension == ".jpeg" ||
						extension == ".bmp" ||
						extension == ".heic" ||
						extension == ".heiv" ||
						extension == ".png"
						)
					{
						cout << "Bild: " << pathToShow.parent_path().string()
							<< '/' << pathToShow.stem().string()
							<< extension << "\n";
						
						Bilddatei datei
						{
							pathToShow.parent_path().generic_wstring(),
							pathToShow.stem().generic_wstring(),
							pathToShow.extension().generic_wstring(),
							file_size(pathToShow),
							(int)0
						};
						dateiListe.push_back(datei);
					}
					else
					{
						//cout << "kein Bild: " << pathToShow.stem().string() + extension << endl;
					}
				}				
				error_code errorCode;
				iter.increment(errorCode);
			}
		}
	}
	catch (std::system_error& e)
	{
		cerr << "Exception :: " << e.what();
	}
	return dateiListe;
}

int main(int argc, const char *argv[])
{
	cout << "Durchsuche Ordner: " << argv[1] << endl;
	
	// durchsuche den Ordner nach normalen Dateien mit einer Länge von mind. x Bytes

	std::string path = argv[1];
	auto liste = getAlleBilddateien(path);

	// alle Bilddateien in die Datenbank eintragen

	
	
	// create database Bilddateien
	// Filename, Pfad, Size, Hash
	// 
}

