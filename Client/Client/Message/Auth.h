#ifndef MESSAGE_AUTH_H
#define MESSAGE_AUTH_H

#include "Message/SerializableObject.h"

/**
 * This class defines a message containing an authentification.
 */
class MessageAuth : public SerializableObject {
public:
    /**
     * Constructor by values.
     *
     * @param name The name of the user authentificating.
     * @param pass The password of the user.
     */
    MessageAuth(const std::string& name, const std::string& pass);

    /**
     * Destructor.
     */
    ~MessageAuth();

    /**
     * This method deserializes an authentification message from a buffer.
     *
     * @param buffer The buffer containing a serialized message.
     * @param size   The size of the buffer.
     *
     * @return A pointer to a newly allocated object, or `nullptr` if the
     *         deserialization failed.
     */
    static MessageAuth* deserialize(const uint8_t* buffer, uint_t size);

    /*************
     * Accessors *
     *************/

    /**
     * This method returns the name of the authentificating user.
     *
     * @return The name of the user.
     */
    std::string getName() const;

    /**
     * This method returns the password of the authentificating user.
     *
     * @return The password of the user.
     */
    std::string getPass() const;

    /*********************
     * Overloaded method *
     *********************/

    uint8_t getID() const;
    uint_t getSize() const;
    int serialize(uint8_t* buffer, uint_t size) const;
protected:
    /** The name of the user authentificating. */
    std::string name_;

    /** The password of the user authentificating. */
    std::string pass_;
};

#endif
