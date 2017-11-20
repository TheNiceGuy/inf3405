#ifndef MESSAGE_QUIT_H
#define MESSAGE_QUIT_H

#include "Message/SerializableObject.h"

/**
 * This class defines a message telling the server that the user quitted.
 */
class MessageQuit : public SerializableObject {
public:
    /**
     * Constructor by values.
     */
    MessageQuit();

    /**
     * Destructor.
     */
    ~MessageQuit();

    /**
     * This method returns the unique ID of the class.
     *
     * @return The ID of the class.
     */
    static uint8_t getID();

    /**
     * This method deserializes a quit message from a buffer.
     *
     * @param buffer The buffer containing a serialized message.
     * @param size   The size of the buffer.
     *
     * @return A pointer to a newly allocated object, or `nullptr` if the
     *         deserialization failed.
     */
    static MessageQuit* deserialize(const uint8_t* buffer, uint_t size);

    /*********************
     * Overloaded method *
     *********************/

    uint_t getSize() const;
    int serialize(uint8_t* buffer, uint_t size) const;
};

#endif
