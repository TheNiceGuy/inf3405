#ifndef MESSAGE_BUFFER_H
#define MESSAGE_BUFFER_H

#include "Utils.h"

#ifdef __LINUX__
    #include <sys/socket.h>
#endif
#ifdef __WIN32__
#endif

/**
 * This class abstracts a buffer for reading messages.
 *
 * @tparam SIZE The size of the internal buffer.
 * @tparam ZERO The default value of the buffer.
 */
template <int SIZE, uint8_t ZERO>
class Buffer {
public:
    /**
     * Constructor by values.
     */
    Buffer() {
        pos_ = 0;

        /* reset the buffer */
        for(uint_t i = 0; i < SIZE; i++) buffer_[i] = ZERO;
    }

    /**
     * Destructor.
     */
    ~Buffer() {

    }

    /**
     * This method rewind the buffer.
     *
     * @param size The number of bytes to rewind in the buffer.
     */
    void rewind(uint_t size) {
        /* rewind the buffer */
        for(uint_t i = 0; i < SIZE-size; i++) buffer_[i] = buffer_[i+size];

        /* reset the data after */
        for(uint_t i = SIZE-size; i < SIZE; i++) buffer_[i] = ZERO;

        /* decrement the position */
        pos_ -= size;
    }

    /**
     * This method puts data at the position returned by getPos() using the
     * the maximum length of getSpace().
     *
     * @return The number of bytes written in the buffer.
     */
    virtual int getData() = 0;

    /*************
     * Accessors *
     *************/

    /**
     * This method returns the position to save bytes into the buffer.
     *
     * @return A pointer to a location inside the buffer.
     */
    uint8_t* getPos() {
        return &buffer_[pos_];
    }

    /**
     * This method returns the number of bytes available at the location
     * returned by getPos().
     *
     * @return The number of bytes available.
     */
    uint_t getSpace() const {
        return SIZE-pos_;
    }

    /**
     * This method retrieves a message from the buffer.
     *
     * @return A pointer the a message if possible, else `nullptr`.
     */
    SerializableObject* getMessage() {
        SerializableObject* obj;

        /* try to get a message */
        obj = SerializableObject::deserialize(buffer_, pos_);
        if(obj != nullptr) {
            rewind(obj->getSize());
            return obj;
        }

        /* read data from the file */
        int len = getData();
        if(len < 0)
            return nullptr;

        /* increment the position */
        pos_ += len;

        /* try to get a message */
        obj = SerializableObject::deserialize(buffer_, pos_);
        if(obj != nullptr) {
            rewind(obj->getSize());
            return obj;
        }

        return nullptr;
    }

protected:
    /** The buffer itself. */
    uint8_t buffer_[SIZE];

    /** The position to read data. */
    uint_t pos_;
};

/**
 * This class is used for reading serializable messages from a file.
 *
 * @tparam SIZE The size of the internal buffer.
 * @tparam ZERO The default value of the buffer.
 */
template <int SIZE, uint8_t ZERO>
class FileBuffer : public Buffer<SIZE, ZERO> {
public:
    /**
     * Constructor by values.
     *
     * @param file The file to read data from.
     */
    FileBuffer(FILE* file) : Buffer<SIZE, ZERO>() {
        file_ = file;
    }

    /**
     * Destructor.
     */
    ~FileBuffer() {

    }

    /*********************
     * Templated methods *
     *********************/

    using Buffer<SIZE, ZERO>::getPos;
    using Buffer<SIZE, ZERO>::getSpace;

    /**********************
     * Overloaded methods *
     **********************/
    
    int getData() {
        /* read data from the file */
        return fread((char*) getPos(), sizeof(uint8_t), getSpace(), file_);
    }

private:
    /** The file user for reading data. */
    FILE* file_;
};

/**
 * This class is used for reading serializable messages from a socket.
 *
 * @tparam SIZE    The size of the internal buffer.
 * @tparam ZERO    The default value of the buffer.
 */
template <int SIZE, uint8_t ZERO>
class NetworkBuffer : public Buffer<SIZE, ZERO> {
public:
    /**
     * Default constructor.
     */
    NetworkBuffer() : Buffer<SIZE, ZERO>() {
        socket_ = -1;
    }

    /**
     * Constructor by values.
     *
     * @param socket The socket to read data from.
     */
    NetworkBuffer(socket_t socket) : Buffer<SIZE, ZERO>() {
        socket_ = socket;
    }

    /**
     * Destructor.
     */
    ~NetworkBuffer() {

    }

    /************
     * Mutators *
     ************/

    /**
     * This methods sets the socket to read data from.
     *
     * @param socket The socket to read data from.
     */
    void setSocket(socket_t socket) {
        socket_ = socket;
    }

    /*********************
     * Templated methods *
     *********************/

    using Buffer<SIZE, ZERO>::getPos;
    using Buffer<SIZE, ZERO>::getSpace;

    /**********************
     * Overloaded methods *
     **********************/

    int getData() {
        /* read data from the socket */
        return recv(socket_, (char*) getPos(), getSpace(), 0);
    }

private:
    /** The socket user for reading data. */
    socket_t socket_;
};

#endif
