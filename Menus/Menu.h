#ifndef MENU_H
#define MENU_H
#include "../Accounts/Account.h"
#include "../Accounts/AuthManager.h"
#include "../Graphs/ZooGraph.h"
#include <string>

class Menu {
public:
    static void showFor(const std::string& username, Role role, AuthManager& auth, ZooGraph& zoo);
};

#endif //MENU_H
