#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include "User.h"

/**
 * This class defines the database of users and messages.
 */
class Database {
public:
    /**
     * Default constructor.
     */
    Database();

    /**
     * Values contructor.
     *
     * @param file The file of the database.
     */
    Database(const std::string& file);

    /**
     * Destructor.
     */
    ~Database();

    /**
     * This method saves the database into a file.
     *
     * @param file The name of the file.
     *
     * @return If the database has been saved, then `true`, else `false`.
     */
    bool save(const std::string& file) const;

    /**
     * This method adds a new message into the backlog.
     *
     * @param msg The new message to add.
     */
    void addMsg(struct msg_server_text* msg);

    /**
     * This method adds a new user to the database.
     *
     * @param name The name of the user.
     * @param pass The password of the user.
     *
     * @return The user that has been created.
     */
    User* addUser(const std::string& name, const std::string& pass);

    /*************
     * Accessors *
     *************/

    /**
     * This method returns a user in the database.
     *
     * @param name The name of the user.
     *
     * @return The requested user.
     */
    User* getUser(const std::string& name) const;

private:
    /** The list of users in the database. */
    std::vector<User> users_;

    /** The list of messages that has been sent. */
    std::vector<struct msg_server_text*> backlog_;
};

#endif
