#include "EmployeeManager.h"
#include "../Graphs/ZooGraph.h"
#include <iostream>

using namespace std;

bool EmployeeManager::addEmployee(const shared_ptr<Employee>& employee) {
    if (!employee) return false;
    string id = employee->getId();
    if (employees.count(id)) {
        cerr << "[Error] Employee with ID " << id << " already exists.\n";
        return false;
    }
    employees[id] = employee;
    return true;
}

shared_ptr<Employee> EmployeeManager::getEmployee(const string& id) const {
    auto it = employees.find(id);
    if (it != employees.end()) return it->second;
    return nullptr;
}

const unordered_map<string, shared_ptr<Employee>>& EmployeeManager::getAllEmployees() const {
    return employees;
}

bool EmployeeManager::assignEmployeeToAviary(const string& employeeId, const string& aviaryId) {
    auto emp = getEmployee(employeeId);
    if (!emp) {
        cerr << "[Error] Employee with ID " << employeeId << " not found.\n";
        return false;
    }

    auto aviary = dynamic_pointer_cast<Aviary>(zooGraph.getAviaryById(aviaryId));
    if (!aviary) {
        cerr << "[Error] Vertex with ID " << aviaryId << " not found.\n";
        return false;
    }

    aviary->setAssignedEmployee(emp);
    emp->assignAviary(aviaryId);

    cout << "[Info] Employee " << emp->getName()
         << " assigned to aviary " << aviaryId << ".\n";
    return true;
}

bool EmployeeManager::removeEmployee(const string& id) {
    auto it = employees.find(id);
    if (it == employees.end()) return false;
    employees.erase(it);
    return true;
}

void EmployeeManager::listAllEmployees() const {
    cout << "=== Zoo Employees ===\n";
    for (const auto& [id, emp] : employees) {
        cout << emp->getFullInfoAboutEmployee() << endl;
    }
}
