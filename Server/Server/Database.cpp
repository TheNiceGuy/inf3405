#include "Utils.h"

#include <iostream>
#ifdef __LINUX__
    #include <sys/stat.h>
#endif
#ifdef __WIN32__
    #include <windows.h>
#endif

#include "Database.h"

using namespace std;

bool fileExists(const string& path);

Database::Database(const string& file) :
    file_(file) {
}

Database::~Database() {
    /* TODO: implement this method */
}

bool Database::init() {
    /* start with an empty database if the file doesn't exist */
    if(!fileExists(file_))
        return true;

    cout << "wow" << endl;

    /* try to load the database file */
    if(!load())
        return false;

    /* the database has been loaded */
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

#ifdef __LINUX__
bool fileExists(const string& path) {
    /* allocate memory for the structure */
    struct stat buffer;   

    /* test if the file exists */
    return (stat (path.c_str(), &buffer) == 0); 
}
#endif
#ifdef __WIN32__
bool fileExists(const string& path) {
    /* get attributes of the file */
    DWORD attrib = GetFileAttributesW(widen(path).c_str());

    /* test file attributes */
    if(attrib == INVALID_FILE_ATTRIBUTES)
        return false;

    /* test directory attributes */
    if(!(attrib & FILE_ATTRIBUTE_DIRECTORY))
        return false;

    return true;
}
#endif
