#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include "User.h"

class Database {
public:
	Database();

	Database(const std::string& file);

	~Database();

	void addMsg(struct msg_server_text* msg);

    User* getUser(const std::string& name) const;


private:
	std::vector<User> users_;
	std::vector<std::string> backlog_;
};

#endif
