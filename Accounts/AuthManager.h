#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H
#include "Account.h"
#include <vector>
#include <optional>
#include <string>
#include <utility>
using namespace std;

class AuthManager {
private:
    vector<Account> accounts;
    string accountsFile = "accounts.txt";

    void loadAccounts();
    void saveAccounts() const;

public:
    AuthManager();
    ~AuthManager();

    optional<pair<string, Role>> authenticateInteractive();

    bool addAccount(const string& username, const string& password, Role role);
    bool removeAccount(const string& username);
    bool editAccount(const std::string& username);

    bool accountExists(const string& username) const;

    vector<string> listUsernames() const;
};

#endif //AUTHMANAGER_H
