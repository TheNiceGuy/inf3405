#include <iostream>

#include "Utils.h"
#include "Message/Types.h"
#include "Message/Auth.h"
#include "Message/Status.h"
#include "Message/ClientText.h"

#ifdef __LINUX__
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
#endif
#ifdef __WIN32__
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
#endif

#include "Client.h"

using namespace std;

#ifdef __LINUX__
void* threadEntryPoint(void* data);
#endif
#ifdef __WIN32__
DWORD WINAPI threadEntryPoint(LPVOID data);
#endif

Client::Client(Server* server, socket_t socket) {
    name_   = string();
    server_ = server;
    socket_ = socket;

    /* get the client's IP address */
    socklen_t len = sizeof(addr_);
    if(getpeername(socket_, &addr_, &len) < 0)
        cout << "getpeername() failed: error" << errno << endl;

    /* create the thread */
#ifdef __LINUX__
    pthread_create(&thread_, NULL, threadEntryPoint, this);
#endif
#ifdef __WIN32__
    thread_ = CreateThread(NULL, 0, threadEntryPoint, this, 0, &threadID_);
#endif
}

void Client::handleClient() {
#ifdef __LINUX__
    /* set the thread ID */
    threadID_ = getpid();
#endif

    /* wait for the authentification of the client */
    if(!waitAuthentification()) {
        cout << "Authentification failed" << endl;
        return;
    }

    /* wait for messages */
    while(waitMessage());
}

bool Client::sendMessage(void* msg, uint_t size) {
    return !(send(socket_, (char*) msg, size, 0) < 0);
}

string Client::getName() const {
    return name_;
}

tid_t Client::getThreadID() const {
    return threadID_;
}

thread_t Client::getThread() const {
    return thread_;
}

uint32_t Client::getSocketAddr() const {
    unsigned long addr = ((sockaddr_in*) &addr_)->sin_addr.s_addr;
    uint32_t address = 0;
    address |= (addr >>  0) & 0xff;
    address |= (addr >>  8) & 0xff;
    address |= (addr >> 16) & 0xff;
    address |= (addr >> 24) & 0xff;
    return address;
}

uint32_t Client::getSocketPort() const {
    struct sockaddr_in* addr = (sockaddr_in*) &addr_;
    return (uint32_t) addr->sin_port;
}

SerializableObject* Client::receiveMessage() {
    /* read data from the buffer */
    ssize_t len = recv(socket_, (char*) buffer_.getBuffer(), buffer_.getSize(), 0);
    if(len < 0) {
        cout << "recv() failed: error " << errno << endl;
        return nullptr;
    }

    /* get the message */
    SerializableObject* msg = buffer_.getMessage(len);
    buffer_.rewind(msg->getSize());

    return msg;
}

bool Client::waitAuthentification() {
    /* receive the next message */
    SerializableObject* msg = receiveMessage();
    if(msg == nullptr) {
        delete msg;
        return false;
    }

    /* make sure the message is an authentification */
    if(msg->getID() != MSG_AUTH) {
        delete msg;
        return false;
    }

    /* convert the message */
    MessageAuth* auth = static_cast<MessageAuth*>(msg);

    /* extract the name and the password */
    string name = auth->getName();
    string pass = auth->getPass();

    /* try the authentification */
    if(!server_->authentificate(name, pass)) {
        delete msg;
        return false;
    }

    /* respond to the client that the authentification worked */
    uint8_t buffer[BUFFER_SIZE];
    MessageStatus status(STATUS_OK);
    int len = status.serialize(buffer, BUFFER_SIZE);
    if(sendMessage(buffer, len)) {
        delete msg;
        return false;
    }

    /* update the user name */
    name_ = name;

    /* authentification was a success */
    delete msg;
    return true;
}

bool Client::waitMessage() {
    /* receive the next message */
    SerializableObject* msg = receiveMessage();
    if(msg == nullptr) {
        delete msg;
        return false;
    }

    /* make sure the message contains text */
    if(msg->getID() != MSG_CLIENT_TEXT) {
        delete msg;
        return false;
    }

    /* extract the text */
    MessageClientText* text = static_cast<MessageClientText*>(msg);
    string message = text->getMessage();

    /* send the text to the other clients */
    if(!server_->sendText(this, message)) {
        delete msg;
        return false;
    }

    /* respond to the client that the message was sent */
    uint8_t buffer[BUFFER_SIZE];
    MessageStatus status(STATUS_OK);
    int len = status.serialize(buffer, BUFFER_SIZE);
    if(sendMessage(buffer, len)) {
        delete msg;
        return false;
    }

    /* message was sent */
    delete msg;
    return true;
}

#ifdef __LINUX__
void* threadEntryPoint(void* data) {
#endif
#ifdef __WIN32__
DWORD WINAPI threadEntryPoint(LPVOID data) {
#endif
    /* get the client */
    Client* client = (Client*) data;

    /* handle the client's requests */
    client->handleClient();

    return 0;
}
