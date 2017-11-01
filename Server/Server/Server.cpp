#include <iostream>

#ifdef __LINUX__
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <errno.h>
    #include <string.h>
    #include <unistd.h>
#endif
#ifdef __WIN32__
    #include <winsock2.h>
    #include <windows.h>
#endif

#include "Utils.h"
#include "Server.h"

using namespace std;

Server::Server(const string& adresse, uint_t port) {
    addr_ = adresse;
    port_ = port;
}

Server::~Server() {
    /* TODO: cleanup*/
}

bool Server::initialise() {
    sockaddr_in service;
    service.sin_family = AF_INET;

    /* make sure the port is valid */
    if(!convertAddr(addr_, &service)) {
        cout << "Error: the specified port isn't valid" << endl;
        return false;
    }

    /* make sure the address is valid */
    if(!convertPort(port_, &service)) {
        cout << "Error: the specified address isn't valid" << endl;
        return false;
    }

#ifdef __WIN32__
    /* start the WINSOCK API */
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != NO_ERROR) {
        cout << "WSAStartup() failed: error " << result << endl;
        return false;
    }
#endif

    /* create the listening socket of the server */
    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ != INVALID_SOCKET) {
        cout << "socket() failed: error " << errno << endl;
#ifdef __WIN32__
        WSACleanup();
#endif
        return false;
    }

    /* bind the listening socket to the address */
    if (bind(socket_, (sockaddr*)& service, sizeof(service)) == SOCKET_ERROR) {
        cout << "bind() failed: error " << errno << endl;
        goto CLEANUP;
    }

    /* set the socket as listening */
    if (listen(socket_, 1) == SOCKET_ERROR) {
        cout << "listen() failed: error " << errno << endl;
        goto CLEANUP;
    }

    /* create the mutex for syncronisation */
#ifdef __LINUX__
    if(pthread_mutex_init(&mutex_, NULL) < 0) {
        cout << "pthread_mutex_init() failed: error" << errno << endl;
        goto CLEANUP;
    }
#endif
#ifdef __WIN32__
    mutex_ = CreateMutex(NULL, FALSE, NULL);
    if (mutex_ == NULL) {
        cout << "CreateMutex() failed: error" << GetLastError() << endl;
        goto CLEANUP;
    }
#endif

    return true;

CLEANUP:
#ifdef __LINUX__
    close(socket_);
#endif
#ifdef __WIN32__
    closesocket(socket_);
    WSACleanup();
#endif
    return false;
}

void Server::waitConnexion() {
    /* continuously listen for incoming connections */
    while (true) {
        /* wait for a new connection */
        socket_t socket = accept(socket_, NULL, NULL);
        if(socket == INVALID_SOCKET) {
            cout << "accept() failed: error " << errno << endl;
            return;
        }

        /* create a new thread for the client */
        Client* client = new Client(this, socket);

        /* add the new client into the list */
        clients_.push_back(client);
    }
}

bool Server::authentificate(const string& name, const string& pass) {
    /* check if the user exists */
    User* user = db_.getUser(name);

    /* create the user if it doesn't exist */
    if(user == nullptr)
        return (db_.addUser(name, pass) !=  nullptr);

    /* check is the password is valid */
    if(!user->validatePass(pass))
        return false;

    /* check if another client is connected with that account */
    for(Client* c : clients_) {
        /* prevent null pointers */
        if(c == nullptr)
            continue;

        /* check if the username isn't used */
        if(c->getName() == name)
            return false;
    }

    return true;
}

bool Server::sendText(Client* client, const string& msg) {
    /* lock the mutex since we only handle one message at a time */
#ifdef __LINUX__
    int err = pthread_mutex_lock(&mutex_);
    if(err != 0) {
        cout << "pthread_mutex_lock() failed: error " << err << endl;
        return false;
    }
#endif
#ifdef __WIN32__
    if(WaitForSingleObject(mutex_, INFINITE) == WAIT_FAILED) {
        cout << "WaitForSingleObject() failed: error " << GetLastError() << endl;
        return false;
    }
#endif

    /* get the client IP */
    uint8_t* addr = (uint8_t*) client->getSocketAddr();

    /* build the message to send back */
    struct msg_server_text message;
    message.type    = MSG_SERVER_TEXT;
    message.addr[0] = addr[0];
    message.addr[1] = addr[1];
    message.addr[2] = addr[2];
    message.addr[3] = addr[3];
    message.port    = client->getSocketPort();
    msg.copy((char*) &message.text, TEXT_MAX_LENGTH, 0);

    /* we send the message to other client */
    for (Client* c : clients_) {
        /* make sure we're not sending the message back to the same client */
        if (c == client)
            continue;

        /* send the message to the other clients */
        c->sendMessage(&message, sizeof(struct msg_server_text));
    }

    /* write the new message into the backlog */
    db_.addMsg(&message);

    /* release the mutex */
#ifdef __LINUX__
    err = pthread_mutex_unlock(&mutex_);
    if(err != 0)
        cout << "pthread_mutex_unlock() failed: error " << err << endl;
#endif
#ifdef __WIN32__
    if(ReleaseMutex(mutex_) == 0)
        cout << "ReleaseMutex() failed: error " << GetLastError() << endl;
#endif

    return true;
}
