#include "Message/SerializableObject.h"
#include "Message/Auth.h"
#include "Message/Quit.h"
#include "Message/Status.h"
#include "Message/ClientText.h"
#include "Message/ServerText.h"
#include "Message/Types.h"

SerializableObject::SerializableObject() {

}

SerializableObject::~SerializableObject() {

}

SerializableObject* SerializableObject::deserialize(const uint8_t* buffer, uint_t size) {
    /* that'd be pretty dumb */
    if(size < 1) return nullptr;

    /* deserialize the message */
    switch((uint8_t) buffer[0]) {
        case MSG_AUTH:
            return MessageAuth::deserialize(buffer, size);
        case MSG_QUIT:
            return MessageQuit::deserialize(buffer, size);
        case MSG_RESP:
            return MessageStatus::deserialize(buffer, size);
        case MSG_CLIENT_TEXT:
            return MessageClientText::deserialize(buffer, size);
        case MSG_SERVER_TEXT:
            return MessageServerText::deserialize(buffer, size);
        default:
            return nullptr;
    }
}
