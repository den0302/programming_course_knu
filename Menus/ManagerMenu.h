#ifndef MANAGERMENU_H
#define MANAGERMENU_H
#include "../Accounts/Account.h"
#include "../Accounts/AuthManager.h"
#include "../Graphs/ZooGraph.h"
#include <string>

namespace ManagerMenu {
    void show(const std::string& username, AuthManager& auth, ZooGraph& zoo);
}

#endif //MANAGERMENU_H
