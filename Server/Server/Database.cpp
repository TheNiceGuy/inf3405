#include <iostream>

#include "Utils.h"
#include "Database.h"

using namespace std;

Database::Database(const string& file) :
    file_(file) {
}

Database::~Database() {
    /* TODO: implement this method */
}

bool Database::init() {


    if(!load())
        return false;

    return true;
}

bool Database::load() {
    /* TODO: implement this method */
    return false;
}

bool Database::save() const {
    /* TODO: implement this method */
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

string Database::getFile() const {
    return file_;
}

void Database::setFile(const string& file) {
    file_ = file;
}

#ifdef __WIN32__
bool fileExist(LPWSTR path) {
    string s = "This is surely ASCII.";
    wstring w(s.begin(), s.end());

    DWORD attrib = GetFileAttributesW(w.c_str());

    /* test file attributes */
    if(attrib == INVALID_FILE_ATTRIBUTES)
        return false;

    /* test directory attributes */
    if(!(attrib & FILE_ATTRIBUTE_DIRECTORY))
        return false;

    return true;
}
#endif
