#include <iostream>

#include "Utils.h"

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

    /* initialise the reading buffer */
    messageBufferInit(&buffer_);

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

struct in_addr* Client::getSocketAddr() const {
    struct sockaddr_in* addr = (sockaddr_in*) &addr_;
    return &addr->sin_addr;
}

uint32_t Client::getSocketPort() const {
    struct sockaddr_in* addr = (sockaddr_in*) &addr_;
    return (uint32_t) addr->sin_port;
}

bool Client::receiveMessage() {
    /* rewind the buffer if possible */
    if(buffer_.pos != 0)
        rewindMessageBuffer(&buffer_);

    /* read data from the buffer */
    ssize_t len = recv(socket_, (char*) buffer_.buffer, MESSAGE_MAX_SIZE, 0);
    if(len < 0) {
        cout << "recv() failed: error " << errno << endl;
        return false;
    }

    /* update the next reading position */
    buffer_.pos += len;

    return true;
}

bool Client::waitAuthentification() {
    /* receive the next message */
    if(!receiveMessage())
        return false;

    /* make sure the message is an authentification */
    uint8_t* type = (uint8_t*) buffer_.buffer;
    if(*type != MSG_AUTH)
        return false;

    /* extract the name and the password */
    struct msg_auth* auth = (struct msg_auth*) buffer_.buffer;
    string name = getStringNullOrLength(auth->name, NAME_MAX_LENGTH);
    string pass = getStringNullOrLength(auth->pass, PASS_MAX_LENGTH);

    /* try the authentification */
    if(!server_->authentificate(name, pass))
        return false;

    /* respond to the client that the authentification worked */
    struct msg_resp response;
    response.type   = MSG_RESP;
    response.status = STATUS_OK;
    if(send(socket_, (char*) &response, sizeof(struct msg_resp), 0) < 0)
        return false;

    /* update the user name */
    name_ = name;

    /* authentification was a success */
    return true;
}

bool Client::waitMessage() {
    /* receive the next message */
    if(!receiveMessage())
        return false;

    /* make sure the message contains text */
    uint8_t* type = (uint8_t*) buffer_.buffer;
    if(*type != MSG_CLIENT_TEXT)
        return false;

    /* extract the text */
    struct msg_client_text* text = (struct msg_client_text*) buffer_.buffer;
    string msg = getStringNullOrLength(text->text, TEXT_MAX_LENGTH);

    /* send the text to the other clients */
    if(!server_->sendText(this, msg))
        return false;

    /* respond to the client that the message was sent */
    struct msg_resp response;
    response.type   = MSG_RESP;
    response.status = STATUS_OK;
    if(send(socket_, (char*) &response, sizeof(struct msg_resp), 0) < 0)
        return false;

    /* message was sent */
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
