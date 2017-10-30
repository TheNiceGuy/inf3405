#include "Utils.h"
#include "User.h"

using namespace std;

User::User() {
    name_ = "";
    pass_ = "";
}

User::User(const string& name, const string& pass) {
    name_ = name;
    pass_ = pass;
}

User::~User() {

}

string User::getName() const {
    return name_;
}

bool User::validatePass(const string& pass) const {
    return (pass_ == pass);
}
