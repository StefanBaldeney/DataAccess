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
	string Pfad;
	string Dateiname;
	string Extension;
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
			fs::recursive_directory_iterator iter(dirPath,
				fs::directory_options::skip_permission_denied 
				| fs::directory_options::follow_directory_symlink);
			
			fs::recursive_directory_iterator end; // (dirPath.cend());
			
			while (iter != end)
			{
				if (true)
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
							<< extension << endl;
						
						Bilddatei datei
						{
							pathToShow.parent_path().generic_string(),
							pathToShow.stem().generic_string(),
							pathToShow.extension().generic_string(),
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
		//cerr << "Exception :: " << e.what();
		cout << "Exception :: " << e.what();
	}
	return dateiListe;
}

int main(int argc, const char *argv[])
{
	string path = argv[1];
	path = "C:/Users";
	
	cout << "Durchsuche Ordner: " << path << endl;
	
	// durchsuche den Ordner nach normalen Dateien mit einer Länge von mind. x Bytes
	
	auto liste = getAlleBilddateien(path);

	// alle Bilddateien in die Datenbank eintragen
	// erstelle eine neue Datenbank
	sqlite3* db;
	sqlite3_open("c:/sqlite3/BilderC3.sqlite", &db);

	string createSql = "create table Bild(pfad nvarchar(8000), name nvarchar(1000), extension nvarchar(16), size long, hash int);";

	char* err;
	int rc;
    rc = sqlite3_exec(db, createSql.c_str(), callback, 0, &err);

	for (vector<Bilddatei>::iterator it = liste.begin(); it != liste.end(); ++it) {

		string insertSQL{ "insert into Bild VALUES(" };

		insertSQL.append("\"");
		insertSQL.append((*it).Pfad);
		insertSQL.append("\",\"");
		insertSQL.append((*it).Dateiname);
		insertSQL.append("\",\"");
		insertSQL.append((*it).Extension);
		insertSQL.append("\",");
		insertSQL.append(to_string((*it).Size));
		insertSQL.append(",");
		insertSQL.append(to_string((*it).Hash));
		insertSQL.append(");");
		
		//cout << insertSQL << endl;

		rc = sqlite3_exec(db, insertSQL.c_str(), callback, 0, &err);
	}
	
	sqlite3_free(err);
	sqlite3_close(db);
}

