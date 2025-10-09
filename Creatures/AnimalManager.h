#ifndef ANIMALMANAGER_H
#define ANIMALMANAGER_H
#include "Animals.h"
#include <unordered_map>
#include <vector>

class ZooGraph;

class AnimalManager {
private:
    ZooGraph& zooGraph;
    unordered_map<string, shared_ptr<Animal>> animals;

public:
    explicit AnimalManager(ZooGraph& graph) : zooGraph(graph) {}

    const unordered_map<string, shared_ptr<Animal>>& getAnimals() const;

    void createAnimal(const string& name, const string& species, int age, double weight, const string& type);
    shared_ptr<Animal> createAnimal(const string& id, string& name, string& species, int age, double weight, string& type, string& aviaryId);

    bool addAnimalInAviary(const string& aviaryId, const string& animalId);
    bool removeAnimalFromAviary(const string& aviaryId, const string& animalId);
    bool removeAnimalFromAnimals(const string& animalId);
    bool moveAnimalBetweenAviaries(const string& fromAviaryId, const string& toAviaryId, const string& animalId);

    vector<shared_ptr<Animal>> getAnimalsNotInAviaries() const;
    bool allAnimalsAssigned() const;

    void feedById(const string& animalId);

    void listAllAnimals() const;
};


#endif //ANIMALMANAGER_H
