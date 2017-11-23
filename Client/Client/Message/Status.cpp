#include "Message/Status.h"
#include "Types.h"

using namespace std;

MessageStatus::MessageStatus(uint8_t status) {
    status_ = status;
}

MessageStatus::~MessageStatus() {

}

uint8_t MessageStatus::getID() const {
    return MSG_RESP;
}

MessageStatus* MessageStatus::deserialize(const uint8_t* buffer, uint_t size) {
    uint_t pos = 0;

    /* this removes a warning */
    if(size < 1) return nullptr;

    /* make sure the type is correct */
    if(buffer[pos++] != MSG_RESP) return nullptr;

    /* get the status */
    uint8_t status = buffer[pos];

    return new MessageStatus(status);
}

uint8_t MessageStatus::getStatus() const {
    return status_;
}

uint_t MessageStatus::getSize() const {
    uint_t size = 0;

    /* the type */
    size += sizeof(uint8_t);

    /* the status */
    size += sizeof(uint8_t);

    return size;
}

int MessageStatus::serialize(uint8_t* buffer, uint_t size) const {   
    /* make sure that the buffer can contains this object */
    if(size < getSize())
        return SERIALIZE_ERROR;

    /* set the ID */
    buffer[0] = getID();
    buffer += 1;

    /* set the status */
    buffer[0] = status_;
    buffer += 1;

    return getSize();
}
