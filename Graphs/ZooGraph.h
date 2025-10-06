#ifndef ZOOGRAPH_H
#define ZOOGRAPH_H
#include <memory>
#include <string>
#include "Graph.h"

#include "../Creatures/EmployeeManager.h"
#include "../Creatures/AnimalManager.h"
class Aviary : public Vertex {
private:
    string name;
    string type;
    double area;
    int capacity;
    vector<shared_ptr<Animal>> animals;
    shared_ptr<Employee> assignedEmployee;
public:
    Aviary(const string& name, const string& type, double area, int capacity)
        : name(name), type(type), area(area), capacity(capacity) {}

    string getIdAviary() const;
    string getName() const;
    string getType() const;
    double getArea() const;
    int getCapacity() const;
    const vector<shared_ptr<Animal>>& getAnimals() const;
    shared_ptr<Animal> getAnimalById(const string& id) const;
    shared_ptr<Employee> getAssignedEmployee() const;

    void setName(const string& n);
    void setType(const string& t);
    void setArea(double a);
    void setCapacity(int c);
    void setAnimals(vector<shared_ptr<Animal>> an);
    void setAssignedEmployee(const shared_ptr<Employee>& emp);

    void printInfoAboutAviary() const;

    bool hasAnimal(const string& animalId) const;
    bool canAddAnimal(const shared_ptr<Animal>& animal) const;

    bool addAnimal(const shared_ptr<Animal>& animal) ;
    bool removeAnimal(const string& id);

    void listAnimals() const;
};

class Path : public Edge {
public:
    Path(const string& from, const string& to, double length)
        : Edge(from, to, length) {}

    double getLength() const;
};

class ZooGraph : public Graph {
private:
    AnimalManager animalManager;
    EmployeeManager employeeManager;
public:
    ZooGraph(): animalManager(*this), employeeManager(*this) {}
    AnimalManager& getAnimalManager();
    EmployeeManager& getEmployeeManager();
    const unordered_map<string, shared_ptr<Vertex>>& getAviaries() const;
    shared_ptr<Vertex> getAviaryById(const string& id) const;
    string getAviaryNameById(const string& id) const;
    vector<string> getNeighborsNames(const string& aviaryId) const;
    vector<string> getNeighborsId(const string& aviaryId) const;

    void addAviary(shared_ptr<Aviary> aviary);
    void addPath(const string& fromId, const string& toId, double length);

    void removeAviary(const string& id);
    void removePath(const string& fromId, const string& toId);

    vector<string> findShortestPath(const string& startId, const string& endId) const;
    double distanceBetweenAviaries(const string& fromId, const string& toId) const;

    bool isZooConnected() const;

    void printPathBetweenAviaries(const string& fromId, const string& toId) const;

    void listAnimals() const;
    void listEmployees() const;
    void printAviaries() const;
    void printZoo() const;
};

#endif //ZOOGRAPH_H
