#include "Utils.h"
#include "Message.h"

#include <iostream>
#include <fstream>

#ifdef __LINUX__
#include <sys/stat.h>
#endif
#ifdef __WIN32__
#include <windows.h>
#endif

#include "Database.h"

using namespace std;

bool fileExists(const string& path);

Database::Database(const string& file) : file_(file)
{
}

Database::~Database() {
	/* TODO: implement this method */
}

bool Database::init() {
	/* start with an empty database if the file doesn't exist */
	if (!fileExists(file_))
		return true;

	/* try to load the database file */
	if (!load())
		return false;

	/* the database has been loaded */
	return true;
}

bool Database::load() {
	ifstream fileRead(file_);
	int i(0), j(0);
	if (fileRead.is_open())
	{
		uint8_t buffer[MESSAGE_MAX_SIZE];
		while (!fileRead.eof()) {
			uint8_t* current = buffer;
			fileRead.read((char*)current++, 1);
			if (current[0] == DB_MESSAGE) {
				fileRead.read((char*)current, sizeof(struct db_message));
				struct db_message* message = (struct db_message*) --current;
				backlog_.push_back(new Message(message));
				i++;
			}
			else if (current[0] == DB_USER) {
				fileRead.read((char*)current, sizeof(struct db_user));
				struct db_user* user = (struct db_user*) --current;
				users_.push_back(new User(user));
				j++;
			}
		}

		cout << "the entire file content is in memory";

		return true;
	}
	else return false;

}

bool Database::save() const {
	ofstream file(file_);
	if (!file.is_open()) {
		cout << "Le fichier n'a pas pu être ouvert." << endl;
		return false;
	}

	for (uint_t i = 0; i < users_.size(); i++) {
		struct db_user buffer;
		users_[i]->serialize((uint8_t*)&buffer);
		file.write((char*)&buffer, sizeof(struct db_user));
	}

	for (uint_t i = 0; i < backlog_.size(); i++) {
		struct db_message buffer;
		backlog_[i]->serialize((uint8_t*)&buffer);
		file.write((char*)&buffer, sizeof(struct db_message));
	}

	file.close();

	return true;
}

void Database::addMsg(const Message& msg) {
	backlog_.push_back(new Message(msg));
}

User* Database::addUser(const std::string& name, const std::string& pass) {
	User* user = new User(name, pass);
	users_.push_back(user);

	return user;
}

User* Database::getUser(const string& name) const {
	for(uint_t i = 0; i < users_.size(); i++)
		if (users_[i]->getName() == name)
			return users_[i];
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
