#ifndef CLIENT_H
#define CLIENT_H

#include "Utils.h"
#include "Message/Auth.h"
#include "Message/ClientText.h"
#include "Message/ServerText.h"
#include "Message/Buffer.h"

#ifdef __LINUX__
#endif
#ifdef __WIN32__
#endif
#include <thread>
#include <mutex>
#include <condition_variable>

#define BUFFER_SIZE 1024
#define BUFFER_ZERO -1

class Client {
public:
    Client(const std::string& addr, uint_t port);
    ~Client();

    void mainLoop(const std::string& user, const std::string& pass);

private:
    socket_t socket_;

    std::mutex mutex_;

    std::mutex mutexType_;

    std::condition_variable event_;

    std::condition_variable eventInput_;

    std::condition_variable eventSocket_;

    int type_;

    bool running_;

    NetworkBuffer<BUFFER_SIZE, BUFFER_ZERO> buffer_;

    void checkInput();

    void checkSocket();

    bool getInput();

    bool getSocket();

    bool authentificate(const MessageAuth& auth);

    bool sendMessage(const MessageClientText& msg);

    void printMessage(MessageServerText* msg) const;
};

#endif
