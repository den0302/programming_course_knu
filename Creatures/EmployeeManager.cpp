#include "EmployeeManager.h"
#include "../Graphs/ZooGraph.h"
#include "../Logger/LoggerGlobal.h"
#include <iostream>
using namespace std;

EmployeeManager::EmployeeManager(ZooGraph& graph, EmployeeRepository& employeeRepo) : zooGraph(graph), employeeRepo(employeeRepo) {
    employeeRepo.initTable();
    loadEmployeesFromRepo(employeeRepo);
}

void EmployeeManager::loadEmployeesFromRepo(EmployeeRepository& employeeRepo) {
    employees = employeeRepo.getAllEmployes();
}

bool EmployeeManager::addEmployee(const shared_ptr<Employee>& employee) {
    if (!employee) {
        logger.error("Attempted to add null employee pointer.");
        return false;
    }

    string id = employee->getId();
    if (employees.count(id)) {
        logger.warn("Employee with ID " + id + " already exists.");
        return false;
    }

    employees[id] = employee;
    employeeRepo.addEmployee(*employee);
    logger.info("Added new employee: " + employee->getName() + " (ID: " + id + ")");
    return true;
}

shared_ptr<Employee> EmployeeManager::getEmployee(const string& id) const {
    auto it = employees.find(id);
    if (it != employees.end()) {
        logger.debug("Retrieved employee with ID: " + id);
        return it->second;
    }
    logger.warn("Employee with ID " + id + " not found.");
    return nullptr;
}

const unordered_map<string, shared_ptr<Employee>>& EmployeeManager::getAllEmployees() const {
    logger.debug("Requested list of all employees.");
    return employees;
}

vector<shared_ptr<Employee>> EmployeeManager::getUnassignedEmployees() const {
    logger.debug("Searching for unassigned employees.");
    vector<shared_ptr<Employee>> unassigned;

    for (const auto& [id, emp] : employees) {
        if (!emp->isAssigned()) {
            unassigned.push_back(emp);
        }
    }

    logger.info("Found " + to_string(unassigned.size()) + " unassigned employees.");
    return unassigned;
}

bool EmployeeManager::assignEmployeeToAviary(const string& employeeId, const string& aviaryId) {
    auto emp = getEmployee(employeeId);
    if (!emp) {
        logger.error("Cannot assign: employee with ID " + employeeId + " not found.");
        return false;
    }

    auto aviary = dynamic_pointer_cast<Aviary>(zooGraph.getAviaryById(aviaryId));
    if (!aviary) {
        logger.error("Cannot assign employee " + employeeId + ": aviary with ID " + aviaryId + " not found.");
        return false;
    }

    aviary->setAssignedEmployee(employeeId);
    emp->assignAviary(aviaryId);

    employeeRepo.assignEmployeeToAviary(employeeId, aviaryId);

    logger.info("Employee " + emp->getName() + " (ID: " + emp->getId() + 
                 ") assigned to aviary " + aviary->getName() + " (ID: " + aviaryId + ").");
    return true;
}

bool EmployeeManager::reassignEmployee(const string& empId, const string& fromAviaryId, const string& toAviaryId) {
    logger.info("Reassigning employee " + empId + " from aviary " + fromAviaryId + " to " + toAviaryId + ".");

    auto fromAviaryBase = zooGraph.getAviaryById(fromAviaryId);
    auto toAviaryBase   = zooGraph.getAviaryById(toAviaryId);

    auto fromAviary = dynamic_pointer_cast<Aviary>(fromAviaryBase);
    auto toAviary   = dynamic_pointer_cast<Aviary>(toAviaryBase);

    if (!fromAviary || !toAviary) {
        logger.error("Failed to reassign: one of aviaries not found or wrong type.");
        return false;
    }

    auto emp = getEmployee(empId);
    if (!emp) {
        logger.error("Failed to reassign: employee " + empId + " not found.");
        return false;
    }

    fromAviary->removeAssignedEmployee();
    toAviary->setAssignedEmployee(empId);
    emp->replaceAviary(fromAviaryId, toAviaryId);
    employeeRepo.moveEmployee(empId, fromAviaryId, toAviaryId);

    logger.info("Employee " + emp->getName() + " successfully reassigned.");
    return true;
}

bool EmployeeManager::removeEmployeeFromAviary(const string& employeeId, const string& aviaryId) {
    logger.debug("Removing employee " + employeeId + " from aviary " + aviaryId + ".");

    auto aviaryIt = zooGraph.getAviaries().find(aviaryId);
    if (aviaryIt == zooGraph.getAviaries().end()) {
        logger.error("Aviary with ID " + aviaryId + " not found.");
        return false;
    }

    auto empIt = employees.find(employeeId);
    if (empIt == employees.end()) {
        logger.error("Employee with ID " + employeeId + " not found.");
        return false;
    }

    auto aviaryPtr = std::dynamic_pointer_cast<Aviary>(aviaryIt->second);
    if (!aviaryPtr) {
        logger.error("Failed to cast vertex to Aviary type (id=" + aviaryId + ").");
        return false;
    }

    std::string assignedId = aviaryPtr->getAssignedEmployee();
    if (assignedId.empty()) {
        logger.warn("No employee currently assigned to aviary " + aviaryPtr->getName() + ".");
        return false;
    }

    if (assignedId != employeeId) {
        logger.warn("Employee " + employeeId + " is not assigned to aviary " + aviaryId + ".");
        return false;
    }

    aviaryPtr->removeAssignedEmployee();
    empIt->second->removeAviary(aviaryId);

    employeeRepo.removeEmployeeFromAviary(empIt->second->getId(), aviaryId);

    logger.info("Employee " + assignedId + " unassigned from aviary " + aviaryPtr->getName() + ".");
    return true;
}

bool EmployeeManager::removeEmployee(const string& id) {
    logger.debug("Removing employee with ID " + id + ".");

    auto emp = getEmployee(id);
    if (!emp) {
        logger.warn("No employee with ID " + id + " found.");
        return false;
    }


    for (auto& [aviaryId, aviaryPtr] : zooGraph.getAviaries()) {
        auto realAviaryPtr = std::dynamic_pointer_cast<Aviary>(aviaryPtr);
        if (!realAviaryPtr) {
            logger.warn("Vertex " + aviaryId + " is not an aviary.");
            continue;
        }

        if (realAviaryPtr->getAssignedEmployee() == id) {
            realAviaryPtr->removeAssignedEmployee();
            logger.info("Employee " + emp->getName() + " was removed from aviary " + realAviaryPtr->getName() + ".");
        }
    }

    employees.erase(id);
    employeeRepo.removeEmployee(id);
    logger.info("Employee " + emp->getName() + " removed from system.");
    return true;
}

void EmployeeManager::listAllEmployees() const {
    logger.debug("Listing all employees.");
    cout << "=== Zoo Employees ===\n";
    for (const auto& [id, emp] : employees) {
        cout << emp->getFullInfoAboutEmployee() << endl;
    }
    logger.info("Displayed " + to_string(employees.size()) + " employees.");
}
