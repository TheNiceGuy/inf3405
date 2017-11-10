#ifndef DATABASE_OBJECT_H
#define DATABASE_OBJECT_H

#include "MessageTypes.h"

class DatabaseObject {

    virtual void serialize(uint8_t* buffer) const = 0;

};

#endif
