#include "Message.h"

#include <string.h>

void Message::serialize(uint8_t* buffer) const {
    struct db_message* msg = (struct db_message*) buffer;

    memcpy(&msg->name, sender_.c_str(), NAME_MAX_LENGTH);

    msg->addr[0] = addr_[0];
    msg->addr[1] = addr_[1];
    msg->addr[2] = addr_[2];
    msg->addr[3] = addr_[3];
    msg->port = port_;
    msg->time = time_;

    memcpy(&msg->text, message_.c_str(), TEXT_MAX_LENGTH);
}
