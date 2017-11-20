#include "Message/ServerText.h"
#include "Types.h"

using namespace std;

MessageServerText::MessageServerText(const string& name, uint64_t time,
    uint32_t addr, uint32_t port, const string& msg) {
    /* TODO: check message length (<255) */
    name_ = name;
    msg_  = msg;
    addr_ = addr;
    port_ = port;
    time_ = time;
}

MessageServerText::~MessageServerText() {

}

uint8_t MessageServerText::getID() {
    return MSG_SERVER_TEXT;
}

MessageServerText* MessageServerText::deserialize(const uint8_t* buffer, uint_t size) {
    uint_t pos = 0;

    /* make sure the type is correct */
    if(buffer[pos++] != getID()) return nullptr;

    /* get the address */
    if(size < pos+sizeof(uint32_t)) return nullptr;
    uint32_t addr = 0;
    addr |= (buffer[pos++] <<  0);
    addr |= (buffer[pos++] <<  8);
    addr |= (buffer[pos++] << 16);
    addr |= (buffer[pos++] << 24);

    /* get the port */
    if(size < pos+sizeof(uint32_t)) return nullptr;
    uint32_t port = 0;
    port |= (buffer[pos++] <<  0);
    port |= (buffer[pos++] <<  8);
    port |= (buffer[pos++] << 16);
    port |= (buffer[pos++] << 24);

    /* get the time */
    if(size < pos+sizeof(uint64_t)) return nullptr;
    uint64_t time = 0;
    time |= ((uint64_t) buffer[pos++]) <<  0;
    time |= ((uint64_t) buffer[pos++]) <<  8;
    time |= ((uint64_t) buffer[pos++]) << 16;
    time |= ((uint64_t) buffer[pos++]) << 24;
    time |= ((uint64_t) buffer[pos++]) << 32;
    time |= ((uint64_t) buffer[pos++]) << 40;
    time |= ((uint64_t) buffer[pos++]) << 48;
    time |= ((uint64_t) buffer[pos++]) << 56;

    /* get the name length */
    if(size < pos+sizeof(uint8_t)) return nullptr;
    uint_t namesize = buffer[pos++];

    /* get the name */
    if(size < pos+namesize) return nullptr;
    string name((char*) &buffer[pos], namesize);
    pos += namesize;

    /* get the message length */
    if(size < pos+sizeof(uint8_t)) return nullptr;
    uint_t msgsize = buffer[pos++];

    /* get the message */
    string msg((char*) &buffer[pos], msgsize);
    pos += msgsize;

    return new MessageServerText(name, time, addr, port, msg);
}

string MessageServerText::getName() const {
    return name_;
}

string MessageServerText::getMessage() const {
    return msg_;
}

uint32_t MessageServerText::getAddress() const {
    return addr_;
}

uint32_t MessageServerText::getPort() const {
    return port_;
}

uint64_t MessageServerText::getTime() const {
    return time_;
}

uint_t MessageServerText::getSize() const {
    uint_t size = 0;

    /* the type */
    size += sizeof(uint8_t);

    /* the addr length */
    size += sizeof(uint32_t);

    /* the port length */
    size += sizeof(uint32_t);

    /* the timestamp length */
    size += sizeof(uint64_t);

    /* the length of the name */
    size += sizeof(uint8_t);

    /* the name */
    size += name_.size();

    /* the length of the message */
    size += sizeof(uint8_t);

    /* the message */
    size += msg_.size();

    return size;
}

int MessageServerText::serialize(uint8_t* buffer, uint_t size) const {   
    /* make sure that the buffer can contains this object */
    if(size < getSize())
        return SERIALIZE_ERROR;

    /* set the ID */
    buffer[0] = getID();
    buffer += 1;

    /* set the address */
    buffer[0] = (uint8_t) (addr_ >>  0);
    buffer[1] = (uint8_t) (addr_ >>  8);
    buffer[2] = (uint8_t) (addr_ >> 16);
    buffer[3] = (uint8_t) (addr_ >> 24);
    buffer += 4;

    /* set the port */
    buffer[0] = (uint8_t) (port_ >>  0);
    buffer[1] = (uint8_t) (port_ >>  8);
    buffer[2] = (uint8_t) (port_ >> 16);
    buffer[3] = (uint8_t) (port_ >> 24);
    buffer += 4;

    /* set the timestamp */
    buffer[0] = (uint8_t) (time_ >>  0);
    buffer[1] = (uint8_t) (time_ >>  8);
    buffer[2] = (uint8_t) (time_ >> 16);
    buffer[3] = (uint8_t) (time_ >> 24);
    buffer[4] = (uint8_t) (time_ >> 32);
    buffer[5] = (uint8_t) (time_ >> 40);
    buffer[6] = (uint8_t) (time_ >> 48);
    buffer[7] = (uint8_t) (time_ >> 56);
    buffer += 8;

    /* set the name length */
    buffer[0] = (uint8_t) name_.size();
    buffer += 1;

    /* copy the name */
    name_.copy((char*) buffer, name_.size(), 0);
    buffer += name_.size();

    /* set the message length */
    buffer[0] = (uint8_t) msg_.size();
    buffer += 1;

    /* copy the message */
    msg_.copy((char*) buffer, msg_.size(), 0);
    buffer += msg_.size();

    return getSize();
}
