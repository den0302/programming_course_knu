#include "ZooGraph.h"
#include <iostream>
#include <set>
#include <utility>
#include <algorithm>
#include "../Logger/LoggerGlobal.h"
using namespace std;

//===========Aviary===========
void Aviary::printInfoAboutAviary() const {
    logger.debug("Printing info about aviary: " + name);
    cout << "Aviary [" << getId() << "]"
         << " Name: " << name
         << ", Type: " << type
         << ", Capacity: " << capacity
         << ", Area: " << area << " m^2"
         << ", Employee: " << (assignedEmployee ? assignedEmployee->getId() : "no employee");
    listAnimals();
    cout << endl;
}

string Aviary::getIdAviary() const { return getId(); }
string Aviary::getName() const { return name; }
string Aviary::getType() const { return type; }
double Aviary::getArea() const { return area; }
int Aviary::getCapacity() const { return capacity; }
const vector<shared_ptr<Animal>>& Aviary::getAnimals() const { return animals; }

shared_ptr<Animal> Aviary::getAnimalById(const string& id) const {
    for (auto& a : animals) {
        if (a->getId() == id) {
            logger.debug("Animal found in aviary " + name + ": " + id);
            return a;
        }
    }
    logger.warn("Animal not found in aviary " + name + ": " + id);
    return nullptr;
}

shared_ptr<Employee> Aviary::getAssignedEmployee() const { return assignedEmployee; }

void Aviary::setName(const string& n) { name = n; logger.info("Aviary name set to: " + n); }
void Aviary::setType(const string& t) { type = t; logger.info("Aviary type set to: " + t); }
void Aviary::setArea(double a) { area = a; logger.info("Aviary area set to: " + to_string(a)); }
void Aviary::setCapacity(int c) { capacity = c; logger.info("Aviary capacity set to: " + to_string(c)); }
void Aviary::setAnimals(vector<shared_ptr<Animal>> an){
    animals = move(an);
    logger.info("Animals list updated in aviary: " + name);
}
void Aviary::setAssignedEmployee(const shared_ptr<Employee>& emp) {
    assignedEmployee = emp;
    logger.info("Assigned employee " + (emp ? emp->getId() : "none") + " to aviary " + name);
}

bool Aviary::addAnimal(const shared_ptr<Animal>& animal) {
    if (!canAddAnimal(animal)) {
        logger.warn("Cannot add animal " + animal->getId() + " to aviary " + name);
        return false;
    }
    animals.push_back(animal);
    logger.info("Animal added to aviary " + name + ": " + animal->getName());
    return true;
}

bool Aviary::removeAnimal(const string& animalId) {
    for (auto it = animals.begin(); it != animals.end(); ++it) {
        if ((*it)->getId() == animalId) {
            animals.erase(it);
            logger.info("Animal removed from aviary " + name + ": " + animalId);
            return true;
        }
    }
    logger.warn("Attempted to remove non-existing animal from aviary " + name + ": " + animalId);
    return false;
}

void Aviary::removeAssignedEmployee() {
    logger.info("Employee unassigned from aviary " + name);
    assignedEmployee.reset();
}

void Aviary::listAnimals() const {
    logger.debug("Listing animals in aviary: " + name);
    if (animals.empty()) {
        cout << "\nAviary \"" << name << "\" empty.\n";
        return;
    }
    cout << "\n=== Animals in " << name << " ===\n";
    for (auto& a : animals) a->printInfoAboutAnimal();
}

bool Aviary::hasAnimal(const string& animalId) const {
    for (const auto& a : animals)
        if (a->getId() == animalId) return true;
    return false;
}

bool Aviary::canAddAnimal(const shared_ptr<Animal>& animal) const {
    if (hasAnimal(animal->getId())) return false;
    if ((int)animals.size() >= capacity) return false;
    for (const auto& existing : animals)
        if (!existing->isCompatibleWith(animal) || !animal->isCompatibleWith(existing))
            return false;
    return true;
}

//===========Path===========
double Path::getLength() const { return getWeight(); }

//===========ZooGraph===========
AnimalManager& ZooGraph::getAnimalManager() { return animalManager; }
EmployeeManager& ZooGraph::getEmployeeManager() { return employeeManager; }
const unordered_map<string, shared_ptr<Vertex>>& ZooGraph::getAviaries() const { return getVertices(); }

string ZooGraph::getAviaryNameById(const string& id) const {
    auto v = getVertex(id);
    if (auto aviary = dynamic_pointer_cast<Aviary>(v)) {
        logger.debug("Got aviary name by id: " + id);
        return aviary->getName();
    }
    logger.warn("Aviary not found by id: " + id);
    return {};
}

shared_ptr<Vertex> ZooGraph::getAviaryById(const string& id) const {
    logger.debug("Getting aviary by id: " + id);
    return getVertex(id);
}

vector<string> ZooGraph::getNeighborsId(const string& aviaryId) const {
    logger.debug("Getting neighbor IDs for aviary: " + aviaryId);
    return getNeighbors(aviaryId);
}

vector<string> ZooGraph::getNeighborsNames(const string& aviaryId) const {
    vector<string> result;
    vector<string> neighborIds = getNeighbors(aviaryId);
    logger.debug("Getting neighbor names for aviary: " + aviaryId);

    for (const auto& id : neighborIds)
        result.push_back(getAviaryNameById(id));

    return result;
}

void ZooGraph::addAviary(shared_ptr<Aviary> aviary) {
    addVertex(aviary);
    logger.info("Aviary added: " + aviary->getName());
}

void ZooGraph::removeAviary(const string& id) {
    removeVertex(id);
    logger.info("Aviary removed: " + id);
}

void ZooGraph::addPath(const string& fromId, const string& toId, double length) {
    addEdge(fromId, toId, length);
    logger.info("Path added: " + fromId + " <-> " + toId + " (" + to_string(length) + " m)");
}

void ZooGraph::removePath(const string& fromId, const string& toId) {
    removeEdge(fromId, toId);
    logger.info("Path removed: " + fromId + " <-> " + toId);
}

vector<string> ZooGraph::findShortestPath(const string& startId, const string& endId) const {
    logger.info("Finding shortest path from " + startId + " to " + endId);
    return findPathByWeight(startId, endId);
}

double ZooGraph::distanceBetweenAviaries(const string& fromId, const string& toId) const {
    double dist = distanceBetween(fromId, toId);
    logger.info("Distance between " + fromId + " and " + toId + ": " + to_string(dist));
    return dist;
}

bool ZooGraph::isZooConnected() const {
    bool connected = checkConnectivity();
    logger.info(string("Zoo connectivity check: ") + (connected ? "Connected" : "Disconnected"));
    return connected;
}

void ZooGraph::printPathBetweenAviaries(const string& fromId, const string& toId) const {
    logger.info("Printing path between aviaries " + fromId + " and " + toId);
    vector<string> path = findPathByWeight(fromId, toId);

    if (path.empty()) {
        cout << "No path between aviaries!" << endl;
        logger.warn("No path between " + fromId + " and " + toId);
        return;
    }

    cout << "Path: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << getAviaryNameById(path[i]);
        if (i + 1 < path.size()) cout << " -> ";
    }
    cout << endl;

    double totalDist = distanceBetween(path.front(), path.back());
    cout << "Total distance: " << totalDist << " m" << endl;
    logger.debug("Path printed successfully between " + fromId + " and " + toId);
}

void ZooGraph::listAnimals() const {
    logger.debug("Listing all animals in zoo");
    animalManager.listAllAnimals();
}


void ZooGraph::listEmployees() const {
    logger.debug("Listing all employees in zoo");
    employeeManager.listAllEmployees();
}

void ZooGraph::printAviaries() const {
    logger.info("Printing all aviaries");
    cout << "=======================================\n";
    cout << "Aviaries:\n";
    for (const auto& [id, aviary] : getVertices()) {
        auto av = dynamic_pointer_cast<Aviary>(aviary);
        if (av) av->printInfoAboutAviary();
    }
    cout << "=======================================\n";
}

void ZooGraph::printZoo() const {
    logger.info("Printing full zoo structure");
    cout << "=======================================\n";
    cout << "Zoo structure:\n";
    for (const auto& [id, aviary] : getVertices()) {
        auto av = dynamic_pointer_cast<Aviary>(aviary);
        if (av) av->printInfoAboutAviary();
    }

    cout << "\nPaths:\n";
    set<pair<string,string>> printed;
    for (const auto& e : getEdges()) {
        string u = e.getFrom();
        string v = e.getTo();
        pair p = { min(u,v), max(u,v) };
        if (printed.count(p)) continue;

        cout << getAviaryNameById(u)<< " <--(" << e.getWeight() << " m)--> "<< getAviaryNameById(v)<< endl;
        printed.insert(p);
    }
    cout << "=======================================\n";
    logger.info("Zoo printed successfully");
}
