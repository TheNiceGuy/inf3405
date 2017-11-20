#ifndef MESSAGE_SERVER_TEXT_H
#define MESSAGE_SERVER_TEXT_H

#include "Message/SerializableObject.h"

/**
 * This class defines a message that the server sends.
 */
class MessageServerText : public SerializableObject {
public:
    /**
     * Constructor by values.
     *
     * @param name The name of the user sending the message.
     * @param time The time the server received the message.
     * @param addr The address of the user sending the message.
     * @param port The port of the user sending the message.
     * @param msg  The message to send.
     */
    MessageServerText(const std::string& name, uint64_t time,  uint32_t addr,
        uint32_t port, const std::string& msg);

    /**
     * Destructor.
     */
    ~MessageServerText();

    /**
     * This method returns the unique ID of the class.
     *
     * @return The ID of the class.
     */
    static uint8_t getID();

    /**
     * This method deserializes a server message from a buffer.
     *
     * @param buffer The buffer containing a serialized message.
     * @param size   The size of the buffer.
     *
     * @return A pointer to a newly allocated object, or `nullptr` if the
     *         deserialization failed.
     */
    static MessageServerText* deserialize(const uint8_t* buffer, uint_t size);

    /*************
     * Accessors *
     *************/

    /**
     * This method returns the name of the client.
     *
     * @return The name of the client.
     */
    std::string getName() const;

    /**
     * This method returns the message contained.
     *
     * @return The message of the client.
     */
    std::string getMessage() const;

    /**
     * This method return the address of the client.
     *
     * @return The address of the client.
     */
    uint32_t getAddress() const;

    /**
     * This method returns the port of the client.
     *
     * @return The port of the client.
     */
    uint32_t getPort() const;

    /**
     * This method returns the timestamp of the message.
     *
     * @return The timestamp of the message.
     */
    uint64_t getTime() const;

    /*********************
     * Overloaded method *
     *********************/

    uint_t getSize() const;
    int serialize(uint8_t* buffer, uint_t size) const;
private:
    /** The name of the client. */
    std::string name_;

    /** The message of the client. */
    std::string msg_;

    /** The address of the client. */
    uint32_t addr_;

    /** The port of the client. */
    uint32_t port_;

    /** The time of the message. */
    uint64_t time_;
};

#endif
