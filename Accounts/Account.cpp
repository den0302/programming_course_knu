#include "Account.h"
#include <utility>
#include <stdexcept>
using namespace std;

Account::Account(string u, size_t hash, Role r)
    : username(move(u)), passwordHash(hash), role(r) {}

const string& Account::getUsername() const { return username; }
size_t Account::getPasswordHash() const { return passwordHash; }
Role Account::getRole() const { return role; }

string Account::roleToString(Role r) {
    switch (r) {
        case Role::ADMIN: return "Admin";
        case Role::MANAGER: return "Manager";
        case Role::EMPLOYEE: return "Employee";
        default: return "Unknown";
    }
}

Role Account::intToRole(int v) {
    switch (v) {
        case 0: return Role::ADMIN;
        case 1: return Role::MANAGER;
        case 2: return Role::EMPLOYEE;
        default: throw runtime_error("Invalid role int");
    }
}

int Account::roleToInt(Role r) {
    return static_cast<int>(r);
}

string Account::serialize() const {
    ostringstream oss;
    oss << username << ';' << passwordHash << ';' << roleToInt(role);
    return oss.str();
}

Account Account::deserialize(const string& line) {
    istringstream iss(line);
    string username;
    string hashStr;
    string roleStr;

    if (!getline(iss, username, ';')) throw runtime_error("bad accounts line");
    if (!getline(iss, hashStr, ';')) throw runtime_error("bad accounts line");
    if (!getline(iss, roleStr)) throw runtime_error("bad accounts line");

    size_t hash = 0;
    try { hash = static_cast<size_t>(stoull(hashStr)); } catch(...) {}
    int rInt = stoi(roleStr);
    Role r = intToRole(rInt);

    return Account(username, hash, r);
}
