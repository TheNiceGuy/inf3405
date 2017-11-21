#include "Utils.h"
#include "User.h"

#include <string.h>

using namespace std;

User::User(const string& name, const string& pass) :
    MessageAuth(name, pass) {

}

User::User(const MessageAuth& msg) :
    MessageAuth(msg) {

}

User::~User() {

}

bool User::validatePass(const string& pass) const {
    return (pass_ == pass);
}
