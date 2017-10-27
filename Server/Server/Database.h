#pragma once

#include <vector>
#include "Utilisateur.h"

class Database {
public:
	Database();

	Database(std::string fichier);

	~Database();

	void addMsg(std::string msg);


private:
	std::vector<Utilisateur> utilisateurs_;
	std::vector<std::string> historique;
};
