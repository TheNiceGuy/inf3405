#include "Utils.h"
#include "Client.h"
#include "Message/Types.h"
#include "Message/Auth.h"
#include "Message/Status.h"
#include "Message/ClientText.h"

#ifdef __LINUX__
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <poll.h>
#endif
#ifdef __WIN32__
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
#endif
#include <iostream>

using namespace std;

void threadEntryPoint(Client* client);

Client::Client(Server* server, socket_t socket) :
    buffer_(socket) {
    name_   = string();
    server_ = server;
    socket_ = socket;
    auth_   = false;

    /* get the client's IP address */
    socklen_t len = sizeof(addr_);
	if (getpeername(socket_, &addr_, &len) < 0)

	/* create the thread */
	cout << "starting client thread" << endl;
    thread_ = thread(threadEntryPoint, this);
}

void Client::handleClient() {
    /* get the thread ID */
    size_t id = hash<thread::id>{}(this_thread::get_id());

    /* update the logger's prefix */
    logger_.setPrefix("Thread " + to_string(id) + ": ");

    /* wait for the authentification of the client */
    if(!waitAuthentification()) {
        logger_("authentification failed");
        return;
    }

    server_->getBacklog(this);
	sendQueuedMessages();

    /* set the recv() timeout of the socket */
#ifdef __LINUX__
    struct timeval tv;
    tv.tv_sec  =  0;
    tv.tv_usec = 10;
#endif
#ifdef __WIN32__
	DWORD tv = 10;
#endif
	setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof(tv));

    /* configure the polling structure */
    struct pollfd pollsocket;
    pollsocket.fd = socket_;
    pollsocket.events = POLLRDNORM;

    /* wait for messages */
    while(true) {
		/* wait for data */
#ifdef __LINUX__
        int ret = poll(&pollsocket, 1, 2000);
#endif
#ifdef __WIN32__
		int ret = WSAPoll(&pollsocket, 1, 2000);
#endif

        /* send queued messages if possible */
		if(!sendQueuedMessages())
			break;

        /* no data is ready to read */
        if(ret < 0)
            continue;

        waitMessage();

	    /* check if the other end is still connected */
        char temp;
        ssize_t isconnected = recv(socket_, &temp, 1, MSG_PEEK);
        if(isconnected == 0) {
            logger_("socket closed");
            break;
        }
    }

    /* remove the client from the server */
	server_->unregister(this);
    auth_ = false;
}

bool Client::sendMessage(const SerializableObject& obj) {
    uint8_t buffer[BUFFER_SIZE];

    /* serialize the message */
    int len = obj.serialize(buffer, BUFFER_SIZE);
    if(len < 0) {
        logger_("failed to serialize the message");
        return false;
    }

    /* send the message */
    len = send(socket_, (char*) buffer, len, 0);
    if(len < 0) {
        logger_("send() failed with error " + errno);
        return false;
    }

    return true;
}

bool Client::sendQueuedMessages() {
    while(!queue_.empty()) {
        /* get the next messages */
        mutex_.lock();
        MessageServerText* msg = queue_.front();
        queue_.pop_front();
        mutex_.unlock();

        /* that shouldn't happen */
        if(msg == nullptr)
            continue;

        /* send the message */
        if(!sendMessage(*msg))
			return false;
    }

	return true;
}

void Client::queue(MessageServerText* msg) {
    mutex_.lock();
    queue_.push_back(msg);
    mutex_.unlock();
}

string Client::getName() const {
    return name_;
}

const thread& Client::getThread() const {
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

bool Client::isAuth() const {
    return auth_;
}

bool Client::waitAuthentification() {
    /* receive the next message */
    SerializableObject* msg = buffer_.getMessage();
    if(msg == nullptr)
        return false;

    /* make sure the message is an authentification */
    if(msg->getID() != MSG_AUTH) {
        logger_("unexpected message during authentification");
        delete msg;
        return false;
    }

    /* convert the message */
    MessageAuth* auth = static_cast<MessageAuth*>(msg);

    /* extract the name and the password */
    string name = auth->getName();
    string pass = auth->getPass();
    logger_("trying authentification with " + name + " and " + pass);

    /* try the authentification */
    if(!server_->authentificate(name, pass)) {
        logger_("authentification failed");
        MessageStatus status(STATUS_NOT_AUTH);
        sendMessage(status);
        delete msg;
        return false;
    }

    /* respond to the client that the authentification worked */
    MessageStatus status(STATUS_OK);
    if(!sendMessage(status)) {
        logger_("failled to send authentification message");
        delete msg;
        return false;
    }
    logger_("authentification was sucessful");

    /* update the user name */
    name_ = name;
    auth_ = true;

    /* authentification was a success */
    delete msg;
    return true;
}

bool Client::waitMessage() {
    /* receive the next message */
    SerializableObject* msg = buffer_.getMessage();
    if(msg == nullptr)
        return false;

    /* make sure the message contains text */
    if(msg->getID() != MSG_CLIENT_TEXT) {
        logger_("unexpected message");

        MessageStatus error(STATUS_NOT_OK);
        sendMessage(error);

        delete msg;
        return false;
    }

    /* extract the text */
    MessageClientText* text = static_cast<MessageClientText*>(msg);
    string message = text->getMessage();
    logger_("MESSAGE " + message);

    /* send the text to the other clients */
    if(!server_->sendText(this, message)) {
        logger_("failed to send message to the server");

        MessageStatus error(STATUS_NOT_OK);
        sendMessage(error);

        delete msg;
        return false;
    }

    /* message was sent */
    delete msg;
    return true;
}

void threadEntryPoint(Client* client) {
    /* make sure we didn't get a null pointer */
    if(client == nullptr)
        return;

    /* handle the client's requests */
    client->handleClient();
}
