#include "AnimalManager.h"
#include "../Logger/LoggerGlobal.h"
#include "../Graphs/ZooGraph.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

unordered_map<string, shared_ptr<Animal>> AnimalManager::getAnimals() const {
    logger.debug("Called getAnimals()");
    return animals;
}

void AnimalManager::createAnimal(const string& name, const string& species, int age, double weight, const string& type) {
    logger.info("Creating animal: " + name + " (" + species + "), type: " + type);

    shared_ptr<Animal> animal;
    if (type == "Mammal") animal = make_shared<Mammal>(name, species, age, weight, "Mammal");
    else if (type == "Bird") animal = make_shared<Bird>(name, species, age, weight, "Bird");
    else if (type == "Reptile") animal = make_shared<Reptile>(name, species, age, weight, "Reptile");
    else if (type == "Fish") animal = make_shared<Fish>(name, species, age, weight, "Fish");
    else if (type == "Amphibian") animal = make_shared<Amphibian>(name, species, age, weight, "Amphibian");
    else if (type == "Insect") animal = make_shared<Insect>(name, species, age, weight, "Insect");
    else if (type == "Arachnid") animal = make_shared<Arachnid>(name, species, age, weight, "Arachnid");
    else {
        logger.warn("Unknown type: " + type + ". Animal not created.");
        return;
    }

    animals[animal->getId()] = animal;
    logger.info("Animal created successfully: ID=" + animal->getId());
}

bool AnimalManager::addAnimalInAviary(const string& aviaryId, const string& animalId) {
    logger.debug("addAnimalInAviary() called with aviaryId=" + aviaryId + ", animalId=" + animalId);

    auto itAviary = zooGraph.getAviaries().find(aviaryId);
    auto animalIt = animals.find(animalId);

    if (animalIt == animals.end() || itAviary == zooGraph.getAviaries().end()) {
        logger.error("Invalid aviary or animal ID: aviaryId=" + aviaryId + ", animalId=" + animalId);
        return false;
    }

    auto aviary = dynamic_pointer_cast<Aviary>(itAviary->second);
    auto animal = animalIt->second;

    bool added = aviary->addAnimal(animal);

    if (added) {
        animal->setAviaryId(aviaryId);
        logger.info("Animal \"" + animal->getName() + "\" added into aviary \"" + aviary->getName() + "\".");
    } else {
        logger.warn("Failed to add animal \"" + animal->getName() + "\" to aviary \"" + aviary->getName() + "\".");
    }

    return added;
}

bool AnimalManager::removeAnimalFromAviary(const string& aviaryId, const string& animalId) {
    logger.debug("removeAnimalFromAviary() called with aviaryId=" + aviaryId + ", animalId=" + animalId);

    auto itAviary = zooGraph.getAviaries().find(aviaryId);
    if (itAviary == zooGraph.getAviaries().end()) {
        logger.error("Aviary not found: " + aviaryId);
        return false;
    }

    auto aviary = dynamic_pointer_cast<Aviary>(itAviary->second);
    bool removed = aviary->removeAnimal(animalId);

    if (removed) {
        logger.info("Removed animal " + animalId + " from aviary " + aviaryId);
    } else {
        logger.warn("Failed to remove animal " + animalId + " from aviary " + aviaryId);
    }

    return removed;
}

bool AnimalManager::removeAnimalFromAnimals(const string& animalId) {
    logger.debug("removeAnimalFromAnimals() called with animalId=" + animalId);

    auto it = animals.find(animalId);
    if (it == animals.end()) {
        logger.warn("Animal not found: " + animalId);
        return false;
    }

    auto& aviaries = zooGraph.getAviaries();
    for (auto& [aviaryId, vertexPtr] : aviaries) {
        auto aviaryPtr = dynamic_pointer_cast<Aviary>(vertexPtr);
        if (!aviaryPtr) continue;
        if (aviaryPtr->hasAnimal(animalId)) {
            removeAnimalFromAviary(aviaryId, animalId);
            break;
        }
    }

    animals.erase(it);
    logger.info("Animal " + animalId + " deleted from system.");
    return true;
}

bool AnimalManager::moveAnimalBetweenAviaries(const string& fromAviaryId, const string& toAviaryId, const string& animalId) {
    logger.debug("moveAnimalBetweenAviaries() called: from=" + fromAviaryId + ", to=" + toAviaryId + ", animalId=" + animalId);

    auto fromIt = zooGraph.getAviaries().find(fromAviaryId);
    auto toIt = zooGraph.getAviaries().find(toAviaryId);
    if (fromIt == zooGraph.getAviaries().end() || toIt == zooGraph.getAviaries().end()) {
        logger.error("One of the aviaries does not exist. From=" + fromAviaryId + ", To=" + toAviaryId);
        return false;
    }

    auto fromAviary = dynamic_pointer_cast<Aviary>(fromIt->second);
    auto toAviary = dynamic_pointer_cast<Aviary>(toIt->second);
    if (!fromAviary || !toAviary) {
        logger.error("Invalid aviary pointers detected.");
        return false;
    }

    auto animalIt = animals.find(animalId);
    if (animalIt == animals.end()) {
        logger.error("Animal not found: " + animalId);
        return false;
    }
    auto animal = animalIt->second;

    if (!fromAviary->hasAnimal(animalId)) {
        logger.warn("Animal " + animal->getName() + " not in aviary " + fromAviary->getName());
        return false;
    }

    if (!toAviary->canAddAnimal(animal)) {
        logger.warn("Cannot move animal \"" + animal->getName() + "\" to aviary \"" + toAviary->getName() + "\" (incompatible or full)");
        return false;
    }

    fromAviary->removeAnimal(animalId);
    toAviary->addAnimal(animal);

    logger.info("Animal \"" + animal->getName() + "\" moved from \"" + fromAviary->getName() + "\" to \"" + toAviary->getName() + "\".");

    return true;
}

vector<shared_ptr<Animal>> AnimalManager::getAnimalsNotInAviaries() const {
    logger.debug("getAnimalsNotInAviaries() called");
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
    logger.info("Found " + to_string(notPlaced.size()) + " animals not in aviaries.");
    return notPlaced;
}

bool AnimalManager::allAnimalsAssigned() const {
    bool allAssigned = getAnimalsNotInAviaries().empty();
    logger.debug(string("allAnimalsAssigned() -> ") + (allAssigned ? "true" : "false"));
    return allAssigned;
}

void AnimalManager::feedById(const string& animalId) {
    logger.debug("feedById() called with animalId=" + animalId);
    auto it = animals.find(animalId);
    if (it == animals.end()) {
        logger.warn("Animal not found for feeding: " + animalId);
        return;
    }

    auto& animal = it->second;
    animal->feed();
    logger.info("Animal fed: " + animal->getName());
}

void AnimalManager::listAllAnimals() const {
    logger.debug("listAllAnimals() called");
    cout << "\n=== All animals ===\n";
    int i = 1;
    for (const auto& [id, a] : animals) {
        string aviaryInfo = a->getAviaryId().empty() ? "Not assigned to any aviary" : a->getAviaryId();
        cout << i++ << ". [" << a->getId() << "] " << a->getName()
             << " (" << a->getSpecies() << ") - Aviary: " << aviaryInfo << "\n";
    }
    logger.info("Listed " + to_string(animals.size()) + " animals.");
}
