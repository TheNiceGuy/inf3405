#include "Utils.h"
#include "User.h"

#include <string.h>

using namespace std;

User::User() {
    name_ = "";
    pass_ = "";
}

User::User(const string& name, const string& pass) {
    name_ = name;
    pass_ = pass;
}

User::User(const struct db_user* user) {

}

User::~User() {

}

string User::getName() const {
    return name_;
}

bool User::validatePass(const string& pass) const {
    return (pass_ == pass);
}

void User::serialize(uint8_t* buffer) const {
    struct db_user* user = (struct db_user*) buffer;

    /* copy the fields */
    buffer[0] = DB_USER;
    memcpy(&user->name, name_.c_str(), NAME_MAX_LENGTH);
    memcpy(&user->pass, pass_.c_str(), PASS_MAX_LENGTH);
}
