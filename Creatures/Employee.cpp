#define byte win_byte_override
#include <windows.h>
#undef byte
#include "Employee.h"
#include "../Graphs/ZooGraph.h"
#include <iostream>
#include <rpcdce.h>

Employee::Employee(const string& name, int age, int salary, int experience)
        : name(name), age(age), salary(salary), experience(experience) {
    UUID uuid;
    RPC_STATUS status = UuidCreate(&uuid);
    if (status != RPC_S_OK && status != RPC_S_UUID_LOCAL_ONLY && status != RPC_S_UUID_NO_ADDRESS) {
        throw runtime_error("UuidCreate failed, RPC_STATUS = " + to_string(status));
    }

    RPC_CSTR strUuid = nullptr;
    status = UuidToStringA(&uuid, &strUuid);
    if (status != RPC_S_OK) {
        throw runtime_error("UuidToString failed, RPC_STATUS = " + to_string(status));
    }

    id = string(reinterpret_cast<char*>(strUuid));
    RpcStringFreeA(&strUuid);
}
string Employee::getId() const {
    return id;
}
int Employee::getAge() const{
    return age;
}
int Employee::getSalary() const{
    return salary;
}
int Employee::getExperience() const{
    return experience;
}
string Employee::getName() const{
    return name;
}

string Employee::getFullInfoAboutEmployee() const{
    return "Employee[" + id + "] | Name: " + name + " | Age: " + to_string(age) +
        " | Salary: " + to_string(salary) + " | Experience: " + to_string(experience) + " | Aviaries: " + listAviaries();
}

string Employee::listAviaries() const {
    if (aviaryIds.empty()) {
        return "No aviaries assigned.";
    }

    string result;
    for (size_t i = 0; i < aviaryIds.size(); ++i) {
        result += aviaryIds[i];
        if (i != aviaryIds.size() - 1) {
            result += ", ";
        }
    }
    return result;
}

void Employee::replaceAviary(const std::string& fromAviary, const std::string& toAviary) {
    for (auto& id : aviaryIds) {
        if (id == fromAviary) {
            id = toAviary;
            return;
        }
    }
    aviaryIds.push_back(toAviary);
}

const vector<string>& Employee::getAviaryIds() const { return aviaryIds; }
void Employee::assignAviary(const string& aviary) {
    aviaryIds.push_back(aviary);
}
