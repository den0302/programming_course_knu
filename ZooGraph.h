#ifndef ZOOGRAPH_H
#define ZOOGRAPH_H
#include <memory>
#include <string>
#include "Graph.h"

//#include "Employee.h"
#include "Creatures.h"

class Aviary : public Vertex {
private:
    string name;
    string type;
    double area;
    int capacity;
    vector<shared_ptr<Animal>> animals;
public:
    Aviary(const string& name, const string& type, double area, int capacity)
        : name(name), type(type), area(area), capacity(capacity) {}

    string getName() const;
    string getType() const;
    double getArea() const;
    int getCapacity() const;
    const vector<shared_ptr<Animal>>& getAnimals() const;
    shared_ptr<Animal> getAnimalById(const string& id) const;

    void setName(const string& n);
    void setType(const string& t);
    void setArea(double a);
    void setCapacity(int c);
    void setAnimals(vector<shared_ptr<Animal>> an);

    void printInfoAboutAviary() const;

    bool hasAnimal(const string& animalId) const;
    bool canAddAnimal(const shared_ptr<Animal>& animal) const;

    bool addAnimal(const shared_ptr<Animal>& animal) ;
    bool removeAnimal(const string& id);

    void listAnimals() const;
};

class Path : public Edge {
public:
    Path(const std::string& from, const std::string& to, double length)
        : Edge(from, to, length) {}

    double getLength() const;
};

class ZooGraph : public Graph {
public:
    const unordered_map<string, shared_ptr<Vertex>>& getAviaries() const;
    shared_ptr<Vertex> getAviaryById(const std::string& id) const;
    string getAviaryNameById(const string& id) const;
    vector<string> getNeighborsNames(const string& aviaryId) const;
    vector<string> getNeighborsId(const string& aviaryId) const;

    void addAviary(std::shared_ptr<Aviary> aviary);
    void addPath(const std::string& fromId, const std::string& toId, double length);

    void removeAviary(const std::string& id);
    void removePath(const std::string& fromId, const std::string& toId);

    vector<string> findShortestPath(const string& startId, const string& endId) const;
    double distanceBetweenAviaries(const std::string& fromId, const std::string& toId) const;

    bool isZooConnected() const;

    void printPathBetweenAviaries(const std::string& fromId, const std::string& toId) const;
    void printAviaries() const;
    void printZoo() const;
};

#endif //ZOOGRAPH_H
