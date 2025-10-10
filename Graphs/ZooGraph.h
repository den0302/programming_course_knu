#ifndef ZOOGRAPH_H
#define ZOOGRAPH_H
#include <memory>
#include <string>
#include "Graph.h"
#include "../Creatures/EmployeeManager.h"
#include "../Creatures/AnimalManager.h"
#include "../DatabaseManager/AviaryRepository.h"
#include "../DatabaseManager/PathRepository.h"
#include "../DatabaseManager/AnimalRepository.h"
#include "../DatabaseManager/EmployeeRepository.h"
class Aviary : public Vertex {
private:
    string name;
    string type;
    double area;
    int capacity;
    vector<shared_ptr<Animal>> animals;
    string assignedEmployee;
    string animalsStrTemp;
public:
    Aviary(string& name,
           string& type,
           double area,
           int capacity);

    Aviary(string& id,
                   string& name,
                   string& type,
                   double area,
                   int capacity,
                   string& assignedEmployee,
                   const string& animalsStr);

    [[nodiscard]] const string& getAnimalsStrTemp() const;
    void clearAnimalsStrTemp();
    vector<shared_ptr<Animal>>& getAnimalsRef();
    [[nodiscard]] string getIdAviary() const;
    [[nodiscard]] string getName() const;
    [[nodiscard]] string getType() const;
    [[nodiscard]] double getArea() const;
    [[nodiscard]] int getCapacity() const;
    [[nodiscard]] const vector<shared_ptr<Animal>>& getAnimals() const;
    [[nodiscard]] shared_ptr<Animal> getAnimalById(const string& id) const;
    [[nodiscard]] string getAssignedEmployee() const;
    [[nodiscard]] string getAnimalsStr() const;

    void setName(const string& n);
    void setType(const string& t);
    void setArea(double a);
    void setCapacity(int c);
    void setAnimals(vector<shared_ptr<Animal>> an);
    void setAssignedEmployee(const string& employeeId);
    void setAnimals(const string& animals);

    void printInfoAboutAviary() const;

    [[nodiscard]] bool hasAnimal(const string& animalId) const;
    [[nodiscard]] bool canAddAnimal(const shared_ptr<Animal>& animal) const;

    bool addAnimal(const shared_ptr<Animal>& animal) ;
    bool removeAnimal(const string& id);
    void removeAssignedEmployee();

    void listAnimals() const;
};

class Path : public Edge {
public:
    Path(const string& from, const string& to, double length)
        : Edge(from, to, length) {}
    [[nodiscard]] string getFromId() const;
    [[nodiscard]] string getToId() const;
    [[nodiscard]] double getLength() const;
};

class ZooGraph : public Graph {
private:
    AviaryRepository& repoAv;
    PathRepository& repoPth;
    AnimalRepository& animalRepo;
    EmployeeRepository& employeeRepo;

    AnimalManager animalManager;
    EmployeeManager employeeManager;
public:
    ZooGraph(AviaryRepository& aviaryRrepository, PathRepository& pathRepository, AnimalRepository& animalRepository, EmployeeRepository& employeeRepository);

    void loadAviariesFromRepo(AviaryRepository& repo);
    void loadPathsFromRepo(PathRepository& repo);

    AnimalManager& getAnimalManager();
    EmployeeManager& getEmployeeManager();

    const unordered_map<string, shared_ptr<Vertex>>& getAviaries() const;
    vector<Edge> getPaths() const;
    const Edge* getPath(const string& fromId, const string& toId) const;
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
