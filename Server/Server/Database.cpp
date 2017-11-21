#include "Utils.h"
#include "Message/Buffer.h"
#include "Message/Types.h"

#include <iostream>
#include <fstream>

#ifdef __LINUX__
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
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
    /* delete the messages */
    for(MessageServerText* text : backlog_)
        delete text;

    /* delete the users */
    for(User* user : users_)
        delete user;
}

bool Database::init() {
	/* start with an empty database if the file doesn't exist */
	if(!fileExists(file_))
		return true;

	/* try to load the database file */
	if(!load())
		return false;

	/* the database has been loaded */
	return true;
}

bool Database::load() {
    Buffer<BUFFER_SIZE,-1> buffer;

    /* open the file */
    int fd = open(file_.c_str(), O_RDONLY);
    if(fd < 0) {
        cout << "The database file could not be read." << endl;
        return false;
    }

    uint_t pos = 0;
    int len;
    do {
        /* read some data */
        char* buf = (char*) buffer.getBuffer();
        len = read(fd, &(buf[pos]), BUFFER_SIZE-pos);

        /* make sure the read() call was sucessfull */
        if(len < 0) {
            close(fd);
            return false;
        }

        /**
         * FIXME: This can be optimized in order to reduce the number of calls
         *        to the rewind() method.
         */

        int size = pos+len;
        while(true) {
            /* deserialize the data that has been read */
            SerializableObject* obj = nullptr;
            obj = SerializableObject::deserialize(buffer.getBuffer(), size);

            /* check if something could be deserialized */
            if(obj == nullptr)
                break;

            /* rewind the buffer */
            buffer.rewind(obj->getSize());
            size -= obj->getSize();

            /* add the object into the database */
            bool status = addSerializableObject(obj);

            /* delete the object if it wasn't added into the database */
            if(status == false)
                delete obj;
        }

        /* update the reading position */
        pos = size;
    } while(len > 0);

    /* close the file */
    close(fd);

    return true;
}

bool Database::save() const {
    uint8_t buffer[BUFFER_SIZE];

    /* open the file */
	ofstream file(file_);
	if (!file.is_open()) {
		cout << "Le fichier n'a pas pu être ouvert." << endl;
		return false;
	}

    /* write the backlog */
    for(MessageServerText* text : backlog_) {
        int len = text->serialize(buffer, BUFFER_SIZE);
        file.write((char*) buffer, len);
    }

    /* write the users */
    for(User* user : users_) {
        int len = user->serialize(buffer, BUFFER_SIZE);
        file.write((char*) buffer, len);
    }

    /* close the file */
	file.close();

	return true;
}

void Database::addMsg(MessageServerText* msg) {
	backlog_.push_back(msg);
}

User* Database::addUser(const std::string& name, const std::string& pass) {
    /* make sure no user has already this name */
    for(User* user : users_)
        if(user->getName() == name)
            return nullptr;

    /* create the user */
	User* user = new User(name, pass);
	users_.push_back(user);

	return user;
}

void Database::print() const {
    cout << "The list of messages:" << endl;
    for(MessageServerText* text : backlog_)
        cout << text->getSize()    << "\t"
             << text->getAddress() << ":"
             << text->getPort()    << "\t"
             << text->getTime()    << "\t"
             << text->getName()    << "\t"
             << text->getMessage() << endl;
    cout << endl;

    cout << "The list of users:" << endl;
    for(User* user : users_)
        cout << user->getName() << endl;
}

User* Database::getUser(const string& name) const {
    /* search for the user */
    for(User* user : users_)
        if(user->getName() == name)
            return user;

    /* the user hasn't been found */
    return nullptr;
}

string Database::getFile() const {
	return file_;
}

void Database::setFile(const string& file) {
	file_ = file;
}

bool Database::addSerializableObject(SerializableObject* obj) {
    User* user = nullptr;
    MessageServerText* text = nullptr;
    
    switch(obj->getID()) {
        case MSG_AUTH:
            user = static_cast<User*>(obj);
            users_.push_back(user);
            return true;
        case MSG_SERVER_TEXT:
            text = static_cast<MessageServerText*>(obj);
            backlog_.push_back(text);
            return true;
        default:
            return false;
    }
}

#ifdef __LINUX__
bool fileExists(const string& path) {
	/* allocate memory for the structure */
	struct stat buffer;

	/* test if the file exists */
	return (stat(path.c_str(), &buffer) == 0);
}
#endif
#ifdef __WIN32__
bool fileExists(const string& path) {
	/* get attributes of the file */
	DWORD attrib = GetFileAttributesW(widen(path).c_str());

	/* test file attributes */
	if (attrib == INVALID_FILE_ATTRIBUTES)
		return false;

	/* test directory attributes */
	if (!(attrib & FILE_ATTRIBUTE_DIRECTORY))
		return false;

	return true;
}
#endif
