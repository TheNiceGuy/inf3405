#ifndef CLIENT_H
#define CLIENT_H

#include "Utils.h"

#ifdef __LINUX__
    #include <sys/socket.h>
#endif
#ifdef __WIN32__
    #include <winsock2.h>
#endif

#include "Server.h"
#include "MessageTypes.h"

/* server class prototype */
class Server;

class Client {
public:
    /**
     * The value constructor.
     *
     * @param server The server that the client belongs to.
     * @param socket The socket of the client.
     */
    Client(Server* server, socket_t socket);

    /**
     * The destructor.
     */
    ~Client();

    /**
     * This method handles the client requests.
     */
    void handleClient();

    /**
     * This method sends data to the remote user.
     *
     * @param msg  The data to send.
     * @param size The size of the data.
     *
     * @return If the message was sent successfully, 'true', else 'false'.
     */
    bool sendMessage(void* msg, uint_t size);

    /*************
     * Accessors *
     *************/

    /**
     * This method returns the name of the user. If the user isn't
     * authentificated, his name will be empty.
     *
     * @return The name of the client.
     */
    std::string getName() const;

    /**
     * This method returns the thread ID.
     *
     * @return The thread ID.
     */
    tid_t getThreadID() const;

    /**
     * This method returns the thread handle.
     *
     * @return The thread handle.
     */
    thread_t getThread() const;

    struct in_addr* getSocketAddr() const;

    uint32_t getSocketPort() const;

private:
    /** The name of the user connected. */
    std::string name_;

    /** The server it belongs to. */
    Server* server_;

    /* The socket of the client. */
    socket_t socket_;

    struct sockaddr addr_;

    /** The ID of the client thread. */
    tid_t threadID_;

    /** The thread of the client. */
    thread_t thread_;

    /** The buffer for reading data. */
    struct msg_buffer buffer_;

    /**
     * This method reads the socket and put the corresponding message inside
     * the buffer of the client. By design, each call to this method guarantees
     * that there will be a message ready to be process at the start of the
     * buffer.
     *
     * @return If the read was successful, `true`, else `false`.
     */
    bool receiveMessage();

    /**
     * This method waits for the client to authentificate.
     *
     * @return If the authentification was successful, `true`, else `false`.
     */
    bool waitAuthentification();

    /**
     * This method waits for the client to send a message. The resulting
     * message will be put inside the buffer. It assumes that the client is
     * already authentificate. If this method receives a MSG_QUIT type, it will
     * return `false`.
     *
     * @return If a message was received, `true`, else `false`.
     */
    bool waitMessage();
};

#endif
