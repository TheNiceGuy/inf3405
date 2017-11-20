#include "Message/ClientText.h"
#include "Types.h"

using namespace std;

MessageClientText::MessageClientText(const string& msg) :
    msg_(msg) {
    /* TODO: check message length */
}

MessageClientText::~MessageClientText() {

}

uint8_t MessageClientText::getID() {
    return MSG_CLIENT_TEXT;
}

MessageClientText* MessageClientText::deserialize(const uint8_t* buffer, uint_t size) {
    uint_t pos = 0;

    /* make sure the type is correct */
    if(buffer[pos++] != getID())
        return nullptr;

    /* get the size of the message */
    uint_t msgsize = buffer[pos++];
    if(size < pos+msgsize)
        return nullptr;

    /* get the message */
    string msg((char*) &buffer[pos], msgsize);
    pos += msgsize;

    return new MessageClientText(msg);
}

string MessageClientText::getMessage() const {
    return msg_;
}

uint_t MessageClientText::getSize() const {
    uint_t size = 0;

    /* the type */
    size += sizeof(uint8_t);

    /* the message length */
    size += sizeof(uint8_t);

    /* the message */
    size += msg_.size();

    return size;
}

int MessageClientText::serialize(uint8_t* buffer, uint_t size) const {   
    /* make sure that the buffer can contains this object */
    if(size < getSize())
        return SERIALIZE_ERROR;

    /* set the ID */
    buffer[0] = getID();
    buffer += 1;

    /* set the message length */
    buffer[0] = (uint8_t) msg_.size();
    buffer += 1;

    /* copy the message */
    msg_.copy((char*) buffer, msg_.size(), 0);
    buffer += msg_.size();

    return getSize();
}
