#include <winsock2.h>
#include <windows.h>
#include "Server.h"

using namespace std;

DWORD WINAPI handleClient(LPVOID lpParam);

Server::Server(std::string adresse, int port) {
	adresse_ = adresse;
	port_ = port;

	/* start the WINSOCK API */
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %ld\n", result);
		return;
	}

	/* create the listening socket of the server */
	socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_ == INVALID_SOCKET) {
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	/* configure the listening address structure */
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(adresse.c_str());
	service.sin_port = htons(port);

	/* bind the listening socket to the address */
	if (bind(socket_, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR) {
		wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
		closesocket(socket_);
		WSACleanup();
		return;
	}

	/* set the socket as listening */
	if (listen(socket_, 1) == SOCKET_ERROR) {
		wprintf(L"listen failed with error: %ld\n", WSAGetLastError());
		closesocket(socket_);
		WSACleanup();
		return;
	}

	/* create the mutex for syncronisation */
	mutex_ = CreateMutex(NULL, FALSE, NULL);
	if (mutex_ == NULL) {
		printf("CreateMutex error: %d\n", GetLastError());
		return;
	}
}

Server::~Server() {

}

void Server::waitConnexion() {
	/* continuously listen for incoming connections */
	while (true) {
		/* wait for a new connection */
		SOCKET socket = accept(socket_, NULL, NULL);
		if (socket == INVALID_SOCKET) {
			wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
			closesocket(socket_);
			WSACleanup();
			return;
		}

		/* create a new thread for the client */
		Client* client = new Client(this, socket);
		HANDLE thread = CreateThread(
			NULL,
			0,
			handleClient,
			client,
			CREATE_SUSPENDED,
			&(client->getThreadID())
		);
		client->setThread(thread);

		/* start the thread */
		ResumeThread(thread);

		/* add the new client into the list */
		clients_.push_back(client);
	}
}

void Server::sendText(Client* client, string msg) {
	/* we only handle one message at a time */
	WaitForSingleObject(mutex_, INFINITE);

	/* we send the message to other client */
	for (Client* c : clients_) {
		/* make sure we're not sending the message back to the same client */
		if (c == client) 
			continue;

		/* send the message to the other clients */
		c->send(msg);
	}

	/* write the new message into the backlog */
	db_.addMsg(msg);

	/* release the mutex */
	ReleaseMutex(mutex_);
}

DWORD WINAPI handleClient(LPVOID param) {
	char buffer[TAILLE_MESSAGE];

	/* cast the parameter */
	Client* client = (Client*) param;

	while (true) {
		/* wait for a message */
		int len = recv(client->getSocket(), buffer, TAILLE_MESSAGE, 0);

		/* TODO: format name, ip, date, time, etc */

		/* send the message to the other threads */
		client->getServer()->sendText(buffer);
	}
}