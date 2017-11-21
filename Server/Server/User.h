#ifndef USER_H
#define USER_H

#include "Message/Auth.h"

#include <string>

/**
 * This class defines a user in the database.
 */
class User : public MessageAuth {
public:
    /**
     * Default constructor.
     */
    User();

    /**
     * Values constructor.
     *
     * @param name The name of the user.
     * @param pass The pass of the user.
     */
    User(const std::string& name, const std::string& pass);

    /**
     * Values constructor.
     *
     * @param user A pointer to a user structure.
     */
    User(const MessageAuth& user);

    /**
     * Destructor.
     */
    ~User();

    /**
     * This method validate a given password for the user.
     *
     * @param pass The password to validate.
     *
     * @return If the password is valid, then `true`, else `false`.
     */
    bool validatePass(const std::string& pass) const;
};

#endif
