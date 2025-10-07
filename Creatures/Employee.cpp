#define byte win_byte_override
#include <windows.h>
#undef byte
#include "Employee.h"
#include "../Graphs/ZooGraph.h"
#include <iostream>
#include <rpcdce.h>
#include <algorithm>
#include "../Logger/LoggerGlobal.h"

Employee::Employee(const string& name, int age, int salary, int experience)
        : name(name), age(age), salary(salary), experience(experience) {
    UUID uuid;
    RPC_STATUS status = UuidCreate(&uuid);
    if (status != RPC_S_OK && status != RPC_S_UUID_LOCAL_ONLY && status != RPC_S_UUID_NO_ADDRESS) {
        logger.error("UuidCreate failed, RPC_STATUS = " + to_string(status));
        throw runtime_error("UuidCreate failed, RPC_STATUS = " + to_string(status));
    }

    RPC_CSTR strUuid = nullptr;
    status = UuidToStringA(&uuid, &strUuid);
    if (status != RPC_S_OK) {
        logger.error("UuidToString failed, RPC_STATUS = " + to_string(status));
        throw runtime_error("UuidToString failed, RPC_STATUS = " + to_string(status));
    }

    id = string(reinterpret_cast<char*>(strUuid));
    RpcStringFreeA(&strUuid);

    logger.info("Created Employee: " + name + " (ID: " + id + ")");
}

string Employee::getId() const {
    logger.debug("getId() called for Employee: " + name);
    return id;
}

int Employee::getAge() const {
    logger.debug("getAge() called for Employee: " + name);
    return age;
}

int Employee::getSalary() const {
    logger.debug("getSalary() called for Employee: " + name);
    return salary;
}

int Employee::getExperience() const {
    logger.debug("getExperience() called for Employee: " + name);
    return experience;
}

string Employee::getName() const {
    logger.debug("getName() called for Employee: " + name);
    return name;
}

string Employee::getFullInfoAboutEmployee() const {
    logger.info("Full info requested for Employee: " + name);
    string info = "Employee[" + id + "] | Name: " + name + " | Age: " + to_string(age) +
        " | Salary: " + to_string(salary) + " | Experience: " + to_string(experience) + " | Aviaries: " + listAviaries();
    return info;
}

string Employee::listAviaries() const {
    if (aviaryIds.empty()) {
        logger.warn("Employee " + name + " has no aviaries assigned");
        return "No aviaries assigned.";
    }

    string result;
    for (size_t i = 0; i < aviaryIds.size(); ++i) {
        result += aviaryIds[i];
        if (i != aviaryIds.size() - 1) {
            result += ", ";
        }
    }

    logger.debug("Listed aviaries for Employee " + name + ": " + result);
    return result;
}

void Employee::replaceAviary(const string& fromAviary, const string& toAviary) {
    bool replaced = false;
    for (auto& id : aviaryIds) {
        if (id == fromAviary) {
            id = toAviary;
            replaced = true;
            break;
        }
    }

    if (replaced) {
        logger.info("Employee " + name + ": replaced aviary " + fromAviary + " with " + toAviary);
    } else {
        aviaryIds.push_back(toAviary);
        logger.warn("Employee " + name + ": aviary " + fromAviary + " not found, assigned " + toAviary + " instead");
    }
}

void Employee::removeAviary(const string& aviaryId) {
    aviaryIds.erase(remove(aviaryIds.begin(), aviaryIds.end(), aviaryId), aviaryIds.end());
}

bool Employee::isAssigned() const {
    return !aviaryIds.empty();
}

const vector<string>& Employee::getAviaryIds() const {
    logger.debug("getAviaryIds() called for Employee: " + name);
    return aviaryIds;
}

void Employee::assignAviary(const string& aviary) {
    aviaryIds.push_back(aviary);
    logger.info("Employee " + name + " assigned to aviary " + aviary);
}
