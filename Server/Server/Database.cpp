#include "Utils.h"
#include "Database.h"

using namespace std;

Database::Database() {
    /* TODO: implement this method */
}

Database::Database(const string& file) {
    /* TODO: implement this method */
}

Database::~Database() {
    /* TODO: implement this method */
}

bool Database::save(const string& file) const {
    return false;
}

void Database::addMsg(struct msg_server_text* msg) {
    /* TODO: implement this method */
}

User* Database::addUser(const std::string& name, const std::string& pass) {
    /* TODO: implement this method */
    return nullptr;
}

User* Database::getUser(const string& name) const {
    /* TODO: implement this method */
    return nullptr;
}
