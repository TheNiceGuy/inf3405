#include "Message.h"

#include <string.h>
#include <time.h>

using namespace std;

Message::Message(struct msg_client_text* msg, string sender, uint8_t addr[4], uint32_t port) :
    sender_(sender),
    message_((char*) msg->text) {

    addr_[0] = addr[0];
    addr_[1] = addr[1];
    addr_[2] = addr[2];
    addr_[3] = addr[3];

    port_ = port;
    time_ = (unsigned) time(NULL);
}

Message::Message(struct db_message* msg) :
    sender_((char*) msg->name),
    message_((char*) msg->text) {

    addr_[0] = msg->addr[0];
    addr_[1] = msg->addr[1];
    addr_[2] = msg->addr[2];
    addr_[3] = msg->addr[3];

    port_ = msg->port;
    time_ = msg->time;
}

Message::~Message() {

}

void Message::serialize(uint8_t* buffer) const {
    struct db_message* msg = (struct db_message*) buffer;

    msg->type = DB_MESSAGE;

    memcpy(&msg->name, sender_.c_str(), NAME_MAX_LENGTH);

    msg->addr[0] = addr_[0];
    msg->addr[1] = addr_[1];
    msg->addr[2] = addr_[2];
    msg->addr[3] = addr_[3];
    msg->port = port_;
    msg->time = time_;

    memcpy(&msg->text, message_.c_str(), TEXT_MAX_LENGTH);
}
