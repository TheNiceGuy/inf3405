#ifndef MESSAGE_CLIENT_TEXT_H
#define MESSAGE_CLIENT_TEXT_H

#include "Message/SerializableObject.h"

/**
 * This class defines a message that a client sends.
 */
class MessageClientText : public SerializableObject {
public:
    /**
     * Constructor by values.
     *
     * @param msg The message to send.
     */
    MessageClientText(const std::string& msg);

    /**
     * Destructor.
     */
    ~MessageClientText();

    /**
     * This method deserializes an client message from a buffer.
     *
     * @param buffer The buffer containing a serialized message.
     * @param size   The size of the buffer.
     *
     * @return A pointer to a newly allocated object, or `nullptr` if the
     *         deserialization failed.
     */
    static MessageClientText* deserialize(const uint8_t* buffer, uint_t size);

    /*************
     * Accessors *
     *************/

    /**
     * This method returns the message contained.
     *
     * @return The message of the client.
     */
    std::string getMessage() const;

    /*********************
     * Overloaded method *
     *********************/

    uint8_t getID() const;
    uint_t getSize() const;
    int serialize(uint8_t* buffer, uint_t size) const;
private:
    /** The message of the client. */
    std::string msg_;
};

#endif
