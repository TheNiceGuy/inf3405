#ifndef MESSAGE_STATUS_H
#define MESSAGE_STATUS_H

#include "Message/SerializableObject.h"

/**
 * This enumeration defines the statuses returned from the server.
 */
enum status : uint8_t {
    /** The last command executed with success. */
    STATUS_OK = 0,

    /** The last command executed with an error. */
    STATUS_NOT_OK = 1,

    /** The client isn't authentificated. */
    STATUS_NOT_AUTH = 2,
};

/**
 * This class defines a message containing a response status.
 */
class MessageStatus : public SerializableObject {
public:
    /**
     * Constructor by values.
     *
     * @param status The status of the message.
     */
    MessageStatus(uint8_t status);

    /**
     * Destructor.
     */
    ~MessageStatus();

    /**
     * This method deserializes a status message from a buffer.
     *
     * @param buffer The buffer containing a serialized message.
     * @param size   The size of the buffer.
     *
     * @return A pointer to a newly allocated object, or `nullptr` if the
     *         deserialization failed.
     */
    static MessageStatus* deserialize(const uint8_t* buffer, uint_t size);

    /*************
     * Accessors *
     *************/

    /**
     * This method returns the status of the message.
     *
     * @return The status of the message.
     */
    uint8_t getStatus() const;

    /*********************
     * Overloaded method *
     *********************/

    uint8_t getID() const;
    uint_t getSize() const;
    int serialize(uint8_t* buffer, uint_t size) const;
private:
    /** The status of the message. */
    uint8_t status_;
};

#endif
