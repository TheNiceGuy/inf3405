#ifndef MESSAGE_BUFFER_H
#define MESSAGE_BUFFER_H

#include "Utils.h"
#include "SerializableObject.h"

#include <iostream>
#include <stdint.h>

/**
 * This class abstracts a buffer for reading messages.
 */
template <int SIZE, uint8_t ZERO>
class Buffer {
public:
    /**
     * Constructor by values.
     */
    Buffer() {
        /* reset the buffer */
        std::cout << SIZE << std::endl;
        for(uint_t i = 0; i < SIZE; i++) buffer_[i] = ZERO;
    }

    /**
     * Destructor.
     */
    ~Buffer() {

    }

    void rewind(uint_t size) {
        /* rewind the buffer */
        for(uint_t i = 0; i < SIZE-size; i++) buffer_[i] = buffer_[i+size];

        /* reset the data after */
        for(uint_t i = SIZE-size; i < SIZE; i++) buffer_[i] = ZERO;
    }

    /*************
     * Accessors *
     *************/

    /**
     * This method returns a pointer to the buffer of this object.
     *
     * @return The buffer of this object.
     */
    uint8_t* getBuffer() {
        return buffer_;
    }

    /**
     * This method returns the size of the buffer inside this object.
     *
     * @return The size of the buffer. 
     */
    uint_t getSize() const {
        return SIZE;
    }

    /**
     * This method is a small wrapper for deserializing a message.
     *
     * @return A pointer to a message or `nullptr` if the deserialization
     *         failed.
     */
    SerializableObject* getMessage(uint_t size) {
        return SerializableObject::deserialize(buffer_, size);
    }

private:
    /** The buffer itself. */
    uint8_t buffer_[SIZE];
};

#endif
