#ifndef ADMINMENU_H
#define ADMINMENU_H
#include "../Accounts/AuthManager.h"
#include "../Graphs/ZooGraph.h"

namespace AdminMenu {
    void show(const std::string& username, AuthManager& auth, ZooGraph& zoo);
}

#endif //ADMINMENU_H
