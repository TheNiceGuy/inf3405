#ifndef MESSAGE_H
#define MESSAGE_H

#include "DatabaseObject.h"

#include <string>
#include <ctime>

class Message : public DatabaseObject {
public:
    Message(struct msg_client_text* msg, std::string sender, uint8_t addr[4], uint32_t port);

    Message(struct db_message* msg);

    ~Message();

    void serialize(uint8_t* buffer) const;

private:
    std::string sender_;
    
    uint8_t addr_[4];

    uint32_t port_;

    uint64_t time_;

    std::string message_;

};

#endif
