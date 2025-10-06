#ifndef ADMINMENU_H
#define ADMINMENU_H
#include "../Accounts/Account.h"
#include "../Accounts/AuthManager.h"
#include "../Graphs/ZooGraph.h"
#include <string>

namespace AdminMenu {
    void show(const std::string& username, AuthManager& auth, ZooGraph& zoo);
}

#endif //ADMINMENU_H
