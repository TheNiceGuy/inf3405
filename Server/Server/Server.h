#ifndef SERVER_H
#define SERVER_H

#ifdef __LINUX__
    #include <sys/socket.h>
#endif

#ifdef __WIN32__

#endif

#include <vector>
#include "Database.h"
#include "Client.h"

/* client class prototype */
class Client;

/**
 * This class defines the main server logic of the application. It handles
 * client connections and chat room's database.
 */
class Server {
public:
    /**
     * The value constructor.
     *
     * @param db   The database file.
     * @param addr The listening address.
     * @param port The TCP port of the listening address.
     */
    Server(const std::string& db, const std::string& addr, uint_t port);

    /**
     * The destructor.
     */
    ~Server();

    /**
     * This method performs the server's initialisation.
     *
     * @return If the server was successfully initialised, `true`, else `false`.
     */
    bool init();

    /**
     * This method handles the connection of new clients. Each time a client
     * connects, a new thread is started in order to handle the client
     * requests. This method blocks the main thread until the server gets
     * shutdown.
     */
    void waitConnexion();

    /**
     * This method authentificates a client. A client has a username and a
     * password. If the username/password combinaison isn't found in the
     * database, a new entry will be added. If the username is already
     * connected, the authentification will be refused.
     *
     * @param name The name of the user.
     * @param pass The password of the user.
     *
     * @return If the client has been successfully authentificated, `true`,
     *         else `false` if the password is wrong or an another client is
     *         already connected with the username.
     */
    bool authentificate(const std::string& name, const std::string& pass);

    /**
     * This method send a message back to the other client. When a client
     * thread receives a message from a user, we need to send it back to the
     * other users connected. In order to avoir sending the message back to the
     * same user, checks the thread ID.
     *
     * @param client The client thread sending the message.
     * @param msg    The message to send to the other client threads.
     *
     * @return If the text was sent, `true`, else `false`.
     */
    bool sendText(Client* client, const std::string& msg);

private:
    /** The address to listen to. */
    std::string addr_;

    /** The TCP port to listen to. */
    uint_t port_;

    /** The listening socket. */
    socket_t socket_;

    /** The database of users. */
    Database db_;

    /** The list of current client opened. */
    std::vector<Client*> clients_;

    /** The mutex used to syncronize the database. */
    mutex_t mutex_;
};

#endif
