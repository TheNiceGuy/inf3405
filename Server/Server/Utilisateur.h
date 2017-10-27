#pragma once

class Utilisateur {
public:
	Utilisateur();

	Utilisateur(std::string nom, std::string pass);

	~Utilisateur();

	std::string getNom();

	std::string getPass();

private:
	std::string nom_;
	std::string pass_;
};