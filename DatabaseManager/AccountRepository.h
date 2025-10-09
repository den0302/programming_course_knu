#ifndef ACCOUNTREPOSITORY_H
#define ACCOUNTREPOSITORY_H

#include "DatabaseManager.h"
#include <string>
#include <vector>
#include <optional>
#include "../Accounts/AuthManager.h"


class AccountRepository {
private:
    DatabaseManager& db;

public:
    explicit AccountRepository(DatabaseManager& database);

    void initTable();

    bool addAccount(const std::string& username, size_t passwordHash, Role role);
    bool removeAccount(const std::string& username);
    bool updateAccount(const std::string& username, const std::string& newUsername,
                       std::optional<size_t> newPasswordHash, std::optional<Role> newRole);

    std::optional<Account> getAccount(const std::string& username);
    std::vector<Account> getAllAccounts();
    bool accountExists(const std::string& username);
};

#endif // ACCOUNTREPOSITORY_H
