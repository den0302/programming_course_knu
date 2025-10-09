#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include "Account.h"
#include <optional>
#include <string>
#include <vector>
class AccountRepository;

class AuthManager {
private:
    AccountRepository& repo;

public:
    explicit AuthManager(AccountRepository& repository);

    std::optional<std::pair<std::string, Role>> authenticateInteractive();

    bool addAccount(const std::string& username, const std::string& password, Role role);
    bool removeAccount(const std::string& username);
    bool editAccount(const std::string& username);

    bool accountExists(const std::string& username) const;

    std::vector<std::string> listUsernames() const;
};

#endif // AUTHMANAGER_H
