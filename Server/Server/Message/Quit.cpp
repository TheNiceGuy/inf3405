#include "Message/Quit.h"
#include "Types.h"

using namespace std;

MessageQuit::MessageQuit() {

}

MessageQuit::~MessageQuit() {

}

uint8_t MessageQuit::getID() {
    return MSG_QUIT;
}

MessageQuit* MessageQuit::deserialize(const uint8_t* buffer, uint_t size) {
    uint_t pos = 0;

    /* make sure the type is correct */
    if(buffer[pos++] != getID())
        return nullptr;

    return new MessageQuit();
}

uint_t MessageQuit::getSize() const {
    uint_t size = 0;

    /* the type */
    size += sizeof(uint8_t);

    return size;
}

int MessageQuit::serialize(uint8_t* buffer, uint_t size) const {   
    /* make sure that the buffer can contains this object */
    if(size < getSize())
        return SERIALIZE_ERROR;

    /* set the ID */
    buffer[0] = getID();
    buffer += 1;

    return getSize();
}
