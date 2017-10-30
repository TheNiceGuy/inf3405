#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    User();

    User(const std::string& name, const std::string& pass);

    ~User();

    std::string getName() const;

    bool validatePass(const std::string& pass) const;

private:
    std::string name_;
    std::string pass_;
};

#endif
