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
     * This method initialises the database. If the database file exists, it
     * will load it, else it will start with an empty database.
     *
     * @return If the database has been initialised, `true`, else `false`.
     */
    bool init();

    /**
     * This method loads the database from a file.
     *
     * @return If the database has been loaded, the `true`, else `false`.
     */
    bool load();

    /**
     * This method saves the database into the file.
     *
     * @return If the database has been saved, then `true`, else `false`.
     */
    bool save() const;

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

    /**
     * This method returns the name of the database file.
     *
     * @return The name of the database file.
     */
    std::string getFile() const;

    /************
     * Mutators *
     ************/

    /**
     * This method sets the name of the database file.
     *
     * @param file The new database file.
     */
    void setFile(const std::string& file);

private:
    /** The name of the database file. */
    std::string file_;

    /** The list of users in the database. */
    std::vector<User> users_;

    /** The list of messages that has been sent. */
    std::vector<struct msg_server_text*> backlog_;
};

#endif
