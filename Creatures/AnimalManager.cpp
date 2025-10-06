#include "AnimalManager.h"
#include "../Graphs/ZooGraph.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

//===========AnimalManager===========

unordered_map<string, shared_ptr<Animal>> AnimalManager::getAnimals() const {return animals;}

void AnimalManager::createAnimal(const string& name, const string& species, int age, double weight, const string& type) {
    shared_ptr<Animal> animal;
    if (type == "Mammal") animal = make_shared<Mammal>(name, species, age, weight, "Mammal");
    else if (type == "Bird") animal = make_shared<Bird>(name, species, age, weight, "Bird");
    else if (type == "Reptile") animal = make_shared<Reptile>(name, species, age, weight, "Reptile");
    else if (type == "Fish") animal = make_shared<Fish>(name, species, age, weight, "Fish");
    else if (type == "Amphibian") animal = make_shared<Amphibian>(name, species, age, weight, "Amphibian");
    else if (type == "Insect") animal = make_shared<Insect>(name, species, age, weight, "Insect");
    else if (type == "Arachnid") animal = make_shared<Arachnid>(name, species, age, weight, "Arachnid");
    else {
        cout << "Unknown type. Animal not created.\n";
        return;
    }

    animals[animal->getId()] = animal;
    cout << "Created animal: " << name << " (" << species << "), ID: " << animal->getId() << endl;
}

bool AnimalManager::addAnimalInAviary(const string& aviaryId, const string& animalId) {
    auto itAviary = zooGraph.getAviaries().find(aviaryId);
    auto animalIt = animals.find(animalId);

    if (animalIt == animals.end() || itAviary == zooGraph.getAviaries().end())
        return false;

    auto aviary = dynamic_pointer_cast<Aviary>(itAviary->second);
    auto animal = animalIt->second;

    bool added = aviary->addAnimal(animal);

    if (added) {
        animal->setAviaryId(aviaryId);
        cout << "Animal \"" << animal->getName() << "\" added into \"" << aviary->getName() << "\".\n";
    }

    return added;
}

bool AnimalManager::removeAnimalFromAviary(const string& aviaryId, const string& animalId) {
    auto itAviary = zooGraph.getAviaries().find(aviaryId);
    if (itAviary == zooGraph.getAviaries().end()) return false;

    auto aviary = dynamic_pointer_cast<Aviary>(itAviary->second);
    return aviary->removeAnimal(animalId);
}

bool AnimalManager::moveAnimalBetweenAviaries(const string& fromAviaryId, const string& toAviaryId, const string& animalId){
    auto fromIt = zooGraph.getAviaries().find(fromAviaryId);
    auto toIt = zooGraph.getAviaries().find(toAviaryId);
    if (fromIt == zooGraph.getAviaries().end() || toIt == zooGraph.getAviaries().end()) {
        cout << "Error: One of the aviaries does not exist.\n";
        return false;
    }

    auto fromAviary = dynamic_pointer_cast<Aviary>(fromIt->second);
    auto toAviary = dynamic_pointer_cast<Aviary>(toIt->second);

    if (!fromAviary || !toAviary) return false;

    auto animalIt = animals.find(animalId);
    if (animalIt == animals.end()) {
        cout << "Error: Animal with ID " << animalId << " does not exist.\n";
        return false;
    }
    auto animal = animalIt->second;

    if (!fromAviary->hasAnimal(animalId)) {
        cout << "Error: Animal \"" << animal->getName() << "\" is not in aviary \""
             << fromAviary->getName() << "\".\n";
        return false;
    }

    if (!toAviary->canAddAnimal(animal)) {
        cout << "Error: Cannot move animal \"" << animal->getName()
             << "\" to aviary \"" << toAviary->getName()
             << "\" (incompatible or full).\n";
        return false;
    }

    fromAviary->removeAnimal(animalId);
    toAviary->addAnimal(animal);

    cout << "Animal \"" << animal->getName() << "\" successfully moved from \""
         << fromAviary->getName() << "\" to \"" << toAviary->getName() << "\".\n";

    return true;
}



vector<shared_ptr<Animal>> AnimalManager::getAnimalsNotInAviaries() const {
    vector<shared_ptr<Animal>> notPlaced;
    for (const auto& [id, animal] : animals) {
        bool inAviary = false;
        for (const auto& [aid, v] : zooGraph.getAviaries()) {
            auto aviary = dynamic_pointer_cast<Aviary>(v);
            if (aviary && aviary->hasAnimal(id)) {
                inAviary = true;
                break;
            }
        }
        if (!inAviary)
            notPlaced.push_back(animal);
    }
    return notPlaced;
}

bool AnimalManager::allAnimalsAssigned() const {
    return getAnimalsNotInAviaries().empty();
}

void AnimalManager::feedById(const string& animalId) {
    auto it = animals.find(animalId);
    if (it == animals.end()) {
        cout << "Animal with ID " << animalId << " not found.\n";
        return;
    }

    auto& animal = it->second;
    animal->feed();
}


void AnimalManager::listAllAnimals() const {
    cout << "\n=== All animals ===\n";
    int i = 1;
    for (const auto& [id, a] : animals) {
        string aviaryInfo = a->getAviaryId().empty() ? "Not assigned to any aviary" : a->getAviaryId();
        cout << i++ << ". [" << a->getId() << "] " << a->getName()
             << " (" << a->getSpecies() << ") - Aviary: " << aviaryInfo << "\n";
    }
}
