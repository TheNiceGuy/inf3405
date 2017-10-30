#ifndef USER_H
#define USER_H

#include <string>

/**
 * This class defines a user in the database.
 */
class User {
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

    /*************
     * Accessors *
     *************/

    /**
     * This method returns the name of the user.
     *
     * @return The name of the user.
     */
    std::string getName() const;


private:
    /** The name of the user. */
    std::string name_;

    /** The password of the user. */
    std::string pass_;
};

#endif
