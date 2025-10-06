#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include <sstream>
using namespace std;

enum class Role {
    ADMIN = 0,
    MANAGER = 1,
    EMPLOYEE = 2
};

class Account {
private:
    string username;
    size_t passwordHash;
    Role role;

public:
    Account() = default;
    Account(string u, size_t hash, Role r);

    const string& getUsername() const;
    size_t getPasswordHash() const;
    Role getRole() const;

    static string roleToString(Role r);
    static Role intToRole(int v);
    static int roleToInt(Role r);

    string serialize() const;
    static Account deserialize(const string& line);
};


#endif //ACCOUNT_H
