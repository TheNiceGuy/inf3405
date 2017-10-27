#pragma once

#include <Windows.h>
#include "Server.h"

class Client {
public:
	Client(Server* server, SOCKET socket);

	Client(DWORD id, HANDLE thread);

	~Client();

	void send(std::string msg);

	void setThread(HANDLE thread);

	Server* getServer() const;

	DWORD& getThreadID() const;

	HANDLE getThread() const;

	SOCKET getSocket() const;

private:
	Server* server_;
	DWORD threadID_;
	HANDLE thread_;
	SOCKET socket_;
};