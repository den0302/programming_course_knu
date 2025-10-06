#ifndef ANIMALMANAGER_H
#define ANIMALMANAGER_H
#include "Animals.h"

class ZooGraph;

class AnimalManager {
private:
    ZooGraph& zooGraph;
    unordered_map<string, shared_ptr<Animal>> animals;

public:
    AnimalManager(ZooGraph& graph) : zooGraph(graph) {}

    unordered_map<string, shared_ptr<Animal>> getAnimals() const;

    void createAnimal(const string& name, const string& species, int age, double weight, const string& type);

    bool addAnimalInAviary(const string& aviaryId, const string& animalId);
    bool removeAnimalFromAviary(const string& aviaryId, const string& animalId);
    bool moveAnimalBetweenAviaries(const string& fromAviaryId, const string& toAviaryId, const string& animalId);

    vector<shared_ptr<Animal>> getAnimalsNotInAviaries() const;
    bool allAnimalsAssigned() const;

    void feedById(const string& animalId);

    void listAllAnimals() const;
};


#endif //ANIMALMANAGER_H
