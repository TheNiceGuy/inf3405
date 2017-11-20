#include "Message/Auth.h"
#include "Types.h"

using namespace std;

MessageAuth::MessageAuth(const string& name, const string& pass) :
    name_(name),
    pass_(pass) {

}

MessageAuth::~MessageAuth() {

}

uint8_t MessageAuth::getID() {
    return MSG_AUTH;
}

MessageAuth* MessageAuth::deserialize(const uint8_t* buffer, uint_t size) {
    uint_t pos = 0;

    /* make sure the type is correct */
    if(buffer[pos++] != getID())
        return nullptr;

    /* get the size of the name */
    uint_t namesize = buffer[pos++];
    if(size < pos+namesize)
        return nullptr;

    /* get the name */
    string name((char*) &buffer[pos], namesize);
    pos += namesize;

    /* get the size of the password */
    uint_t passsize = buffer[pos++];
    if(size < pos+passsize)
        return nullptr;

    /* get the password */
    string pass((char*) &buffer[pos], passsize);
    pos += namesize;

    return new MessageAuth(name, pass);
}

string MessageAuth::getName() const {
    return name_;
}

string MessageAuth::getPass() const {
    return pass_;
}

uint_t MessageAuth::getSize() const {
    uint_t size = 0;

    /* the type */
    size += sizeof(uint8_t);

    /* the name length */
    size += sizeof(uint8_t);

    /* the name string */
    size += name_.size();

    /* the pass length */
    size += sizeof(uint8_t);

    /* the pass string */
    size += pass_.size();

    return size;
}

int MessageAuth::serialize(uint8_t* buffer, uint_t size) const {   
    /* make sure that the buffer can contains this object */
    if(size < getSize())
        return SERIALIZE_ERROR;

    /* set the ID */
    buffer[0] = getID();
    buffer += 1;

    /* set the name length */
    buffer[0] = (uint8_t) name_.size();
    buffer += 1;

    /* copy the name */
    name_.copy((char*) buffer, name_.size(), 0);
    buffer += name_.size();

    /* set the pass length */
    buffer[0] = (uint8_t) pass_.size();
    buffer += 1;

    /* copy the name */
    pass_.copy((char*) buffer, pass_.size(), 0);
    buffer += pass_.size();

    return getSize();
}
