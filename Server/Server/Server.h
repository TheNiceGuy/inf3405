#pragma once

#include <winsock2.h>
#include <vector>
#include "Database.h"
#include "Client.h"

const int TAILLE_MESSAGE = 400;

class Server {
public:
	Server(std::string adresse, int port);

	~Server();

	void waitConnexion();

	void sendText(Client* client, std::string msg);

private:
	std::string adresse_;
	int port_;
	SOCKET socket_;

	Database db_;

	std::vector<Client*> clients_;

	HANDLE mutex_;
};