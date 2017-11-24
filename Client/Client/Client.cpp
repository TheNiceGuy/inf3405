#include "Client.h"
#include "Window.h"
#include "Message/Types.h"
#include "Message/Status.h"
#include "Message/Auth.h"
#include "Message/ClientText.h"
#include "Message/ServerText.h"
#include "Message/Quit.h"

#ifdef __LINUX__
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <poll.h>
    #include <time.h>
#endif
#ifdef __WIN32__
	#include <winsock2.h>
	#include <ws2tcpip.h>
#endif
#include <csignal>
#include <iostream>
#include <stdexcept>
#include <chrono>

using namespace std;

Client::Client(const string& addr, uint_t port) {
    /* make sure the port is valid */
    if(!isPortValid(port))
        throw invalid_argument("the specified port isn't valid");

    /* configure the IPv4 port for the socket */
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_port = htons(port);
    
    /* convert the string address into the structure */
#ifdef __LINUX__
    int result = inet_pton(AF_INET, addr.c_str(), &service.sin_addr);
    if(result == 0)
        throw invalid_argument("the specified address isn't valid");
#endif
#ifdef __WIN32__
	int result = InetPton(AF_INET, (PCTSTR) addr.c_str(), (PVOID) &service.sin_addr);
	if (result == 0)
		throw invalid_argument("the specified address isn't valid");
#endif

    /* create the socket that connects to the server */
    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#ifdef __LINUX__
    if(socket_ < 0)
        throw runtime_error("socket() failed: error " + to_string(errno));
#endif
#ifdef __WIN32__
    if(socket_ == INVALID_SOCKET)
        throw runtime_error("socket() failed: error " + to_string(WSAGetLastError()));
#endif

    /* connect to the server */
    result = connect(socket_, (struct sockaddr*) &service, sizeof(service));
    if(result < 0) {
#ifdef __LINUX__
        close(socket_);
#endif
#ifdef __WIN32__
		closesocket(socket_);
#endif
        throw runtime_error("connect() failed: error " + to_string(errno));
    }

    /* initialise the buffer */
    buffer_.setSocket(socket_);
}

Client::~Client() {
    /* close the socket */
#ifdef __LINUX__
    close(socket_);
#endif
#ifdef __WIN32__
	closesocket(socket_);
#endif
}

void Client::checkInput() {
    bool locked = false;

    /* mutex stuff */
    mutex mut;
    unique_lock<mutex> lock(mut);

#ifdef __LINUX__
    /* configure the polling structure */
    struct pollfd pollstdin;
    pollstdin.fd     = STDIN_FILENO;
    pollstdin.events = POLLIN;
#endif

    while(running_) {
        /* wait for the main thread to process the input */
        eventInput_.wait(lock);

        /* don't lock the mutex while we wait for poll */
        if(locked)
            mutexType_.unlock();
        else
            locked = true;

        /* wait for a keystroke */
#ifdef __LINUX__
        while(true) {
            int result = poll(&pollstdin, 1, 1);
            if(result > 0) break;
            if(!running_) return;
        }
#endif
#ifdef __WIN32__
		while(true) {
			if(exKbHit())
				break;
			this_thread::sleep_for(chrono::milliseconds(100));
		}
#endif

        /* lock the mutex for the type variable */
        mutexType_.lock();
        type_ = 0;

        /* notify the main thread */
        event_.notify_one();
    }

    /* unlock the mutex */
    if(locked) mutexType_.unlock();
}

void Client::checkSocket() {
    bool locked = false;

    /* mutex stuff */
    mutex mut;
    unique_lock<mutex> lock(mut);

    /* configure the polling structure */
    struct pollfd pollsocket;
    pollsocket.fd     = socket_;
    pollsocket.events = POLLPRI;

    while(running_) {
        /* wait for the main thread to process the data */
        eventSocket_.wait(lock);

        /* don't lock the mutex while we wait for poll */
        if(locked)
            mutexType_.unlock();
        else
            locked = true;

        /* wait for data into the socket */
        while(true) {
#ifdef __LINUX__
            int result = poll(&pollsocket, 1, 1);
#endif
#ifdef __WIN32__
			int result = WSAPoll(&pollsocket, 1, 1);
#endif
            if(result > 0) break;
            if(!running_) return;
        }

        /* lock the mutex for the type variable */
        mutexType_.lock();
        type_ = 1;

        /* notify the main thread */
        event_.notify_one();
    }

    /* unlock the mutex */
    if(locked) mutexType_.unlock();
}

bool Client::getInput() {
    /* get input from the user */
    string msg = Window::getInstance().getLine(200);

    /* check which message to send */
    if(msg == "/quit") {
        return false;
    } else if(!(msg == "")) {
        /* send the message */
        MessageClientText text(msg);
        if(!this->sendMessage(text)) {
            WINDOW_DEBUG("failed to send message");
        }
    }

    return true;
}

bool Client::getSocket() {
    /* get a message */
    SerializableObject* obj = buffer_.getMessage();

    /* make sure there was an object */
    if(obj == nullptr)
        return false;;

    /* it's supposed to be a server message */
    if(obj->getID() != MSG_SERVER_TEXT) {
        WINDOW_DEBUG("message is not server text: " + to_string(obj->getID()));
        delete obj;
        return false;
    }

    /* print the message */
    MessageServerText* text = static_cast<MessageServerText*>(obj);
    printMessage(text);

    delete obj;
    return true;
}

void Client::mainLoop(const string& user, const string& pass) {
    /* send the authentification message */
    MessageAuth auth(user, pass);
    authentificate(auth);

    /* set the recv() timeout of the socket */
#ifdef __LINUX__
    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 10;
    
#endif
#ifdef __WIN32__
	DWORD tv = 10;
#endif
	setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof(tv));

    /* start the threads */
    running_ = true;
    thread input(&Client::checkInput, this);
    thread socket(&Client::checkSocket, this);

    /* wake them up */
    this_thread::sleep_for(chrono::seconds(1));
    eventInput_.notify_one();
    eventSocket_.notify_one();

    unique_lock<mutex> lock(mutex_);
    while(true) {
        event_.wait(lock);


        WINDOW_DEBUG("test");

        bool status;
        if(type_ == 0) {
            status = getInput();
            eventInput_.notify_one();
        } else if(type_ == 1) {
            status = getSocket();
            eventSocket_.notify_one();
        } else {

        }

        if(!status)
            break;
    }

    /* stop the other threads */
    running_ = false;
    eventInput_.notify_one();
    eventSocket_.notify_one();

    /* wait for the threads */
    input.join();
    socket.join();
}

bool Client::authentificate(const MessageAuth& auth) {
    uint8_t buffer[BUFFER_SIZE];

    /* serialize the message */
    int len = auth.serialize(buffer, BUFFER_SIZE);
    if(len < 0)
        return false;

    /* send the message */
    len = send(socket_, (char*) buffer, len, 0);
    if(len < 0) {
        WINDOW_DEBUG("send() failed: error " + to_string(errno));
        return false;
    }

    /* get the response */
    SerializableObject* resp = buffer_.getMessage();
    if(resp == nullptr)
        return false;

    /* make sure the server responded with a status */
    bool ret = true;
    if(resp->getID() != MSG_RESP) {
        WINDOW_DEBUG("unexpected response after authentification");
        ret = false;
    }

    /* make sure the authentification was sucessful */
    MessageStatus* status = (MessageStatus*) resp;
    if(status->getStatus() != STATUS_OK) {
        Window::getInstance().printLine("authentification failed");
        ret = false;
    }

    delete resp;
    return ret;
}

bool Client::sendMessage(const MessageClientText& msg) {
    uint8_t buffer[BUFFER_SIZE];

    /* serialize the message */
    int len = msg.serialize(buffer, BUFFER_SIZE);
    if(len < 0)
        return false;

    /* send the message */
    len = send(socket_, (char*) buffer, len, 0);
    if(len < 0) {
        WINDOW_DEBUG("send() failed: error " + to_string(errno));
        return false;
    }

    return true;
}

void Client::printMessage(MessageServerText* msg) const {
    /* make sure this isn't a null pointer */
    if(msg == nullptr)
        return;

    /* get the address */
    sockaddr_in service;
    service.sin_addr.s_addr = msg->getAddress();
    char* addr = inet_ntoa(service.sin_addr);

    /* get the port */
    uint_t port = ntohs(msg->getPort());

    /* get the timestamp */
    char time[100];
    time_t unix = msg->getTime();
    strftime(time, 100, "%Y-%m-%d@%H:%M:%S", localtime(&unix));

    /* build the string */
    string text = "[" + msg->getName() + " - "
                + string(addr) + ":" + to_string(port) + " - "
                + string(time) + "]: "
                + msg->getMessage();

    /* print the string */
    Window::getInstance().printLine(text);
}
