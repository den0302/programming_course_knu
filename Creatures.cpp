#include "Creatures.h"
#include "ZooGraph.h"
#include <algorithm>
#include <iostream>
#define byte win_byte_override
#include <windows.h>
#undef byte
#include <rpcdce.h>
#include <iomanip>

using namespace std;

//===========Animal===========
Animal::Animal(const string& name, const string& species, int age, double weight, const string& type)
    : name(name), species(species), age(age), weight(weight), type(type) {

    UUID uuid;
    RPC_STATUS status = UuidCreate(&uuid);
    if (status != RPC_S_OK && status != RPC_S_UUID_LOCAL_ONLY && status != RPC_S_UUID_NO_ADDRESS) {
        throw runtime_error("UuidCreate failed, RPC_STATUS = " + to_string(status));
    }

    RPC_CSTR strUuid = nullptr;
    status = UuidToStringA(&uuid, &strUuid);
    if (status != RPC_S_OK) {
        throw runtime_error("UuidToString failed, RPC_STATUS = " + to_string(status));
    }

    id = string(reinterpret_cast<char*>(strUuid));
    RpcStringFreeA(&strUuid);
}

string Animal::getId() const { return id; }
string Animal::getName() const { return name; }
string Animal::getSpecies() const { return species; }
int Animal::getAge() const { return age; }
double Animal::getWeight() const { return weight; }
bool Animal::getIsFed() const { return isFed; }

void Animal::setName(const string& n) { name = n; }
void Animal::setAge(int a) { age = a; }
void Animal::setWeight(double w) { weight = w; }

void Animal::feed() {
    if (!isFed) {
        isFed = true;
        cout << name << " (" << species << ") ate \n";
    } else {
        cout << name << " already full.\n";
    }
}

void Animal::move() const {
    cout << name << " moves across the territory.\n";
}

bool Animal::isCompatibleWith(const std::shared_ptr<Animal>& other) const {
    // Тварини одного виду хижаків, що не можуть жити разом
    if ((species == "Lion" && other->species == "Tiger") ||
        (species == "Tiger" && other->species == "Lion") ||
        (species == "Wolf" && other->species == "Bear") ||
        (species == "Bear" && other->species == "Wolf")) {
        return false;
        }

    // Птахи різних видів, які агресивні або заважають один одному
    if ((species == "Eagle" && other->species == "Parrot") ||
        (species == "Parrot" && other->species == "Eagle") ||
        (species == "Owl" && other->species == "Crow") ||
        (species == "Crow" && other->species == "Owl")) {
        return false;
        }

    // Рептилії хижі з іншими дрібними тваринами
    if ((species == "Snake" && (other->type == "Mammal" || other->type == "Bird")) ||
        ((other->species == "Snake") && (type == "Mammal" || type == "Bird"))) {
        return false;
        }

    // Риби різних видів з агресивною поведінкою
    if ((type == "Fish" && other->type == "Fish") &&
        ((species == "Piranha") || (other->species == "Piranha"))) {
        return false;
        }

    // Амфібії та дрібні безхребетні (небезпека для їжі)
    if ((type == "Amphibian" && other->type == "Insect") ||
        (type == "Insect" && other->type == "Amphibian")) {
        return false;
        }

    // Павук (Arachnid) та дрібні тварини — може з’їсти
    if ((type == "Arachnid") &&
        (other->type == "Insect" || other->type == "Amphibian" || other->type == "Fish")) {
        return false;
        }

    return true;
}


void Animal::printInfoAboutAnimal() const {
    cout << "ID: " << id << "\n"
         << "Name: " << name << "\n"
         << "Species: " << species << "\n"
         << "Age: " << age << "\n"
         << "Weight: " << weight << " kg\n"
         << "Satiety: " << (isFed ? "Yes" : "No") << "\n\n";
}

//===========Mammal===========
Mammal::Mammal(const string& name, const string& species, int age, double weight, const string& type)
    : Animal(name, species, age, weight, type) {}

void Mammal::makeSound() const {
    cout << name << " (" << species << ") said: Rrrr!\n";
}

//===========Bird===========
Bird::Bird(const string& name, const string& species, int age, double weight, const string& type)
    : Animal(name, species, age, weight, type) {}

void Bird::makeSound() const {
    cout << name << " (" << species << ") said: Tweet!\n";
}

void Bird::move() const {
    cout << name << " flying around the aviary \n";
}

//===========Reptile===========
Reptile::Reptile(const string& name, const string& species, int age, double weight, const string& type)
    : Animal(name, species, age, weight, type) {}

void Reptile::makeSound() const {
    cout << name << " (" << species << ") hisses: Ssssss!\n";
}

//===========Fish===========
Fish::Fish(const string& name, const string& species, int age, double weight, const string& type)
        : Animal(name, species, age, weight, type) {}

void Fish::makeSound() const  {
    cout << name << " (" << species << ") makes bubbling sounds!\n";
}

void Fish::move() const  {
    cout << name << " swims in the water.\n";
}

//===========Amphibian===========
Amphibian::Amphibian(const string& name, const string& species, int age, double weight, const string& type)
        : Animal(name, species, age, weight, type) {}
    void Amphibian::makeSound() const  {
        cout << name << " (" << species << ") croaks!\n";
    }

    void Amphibian::move() const  {
        cout << name << " hops around.\n";
    }

//===========Insect===========
Insect::Insect(const string& name, const string& species, int age, double weight, const string& type)
        : Animal(name, species, age, weight, type) {}
void Insect::makeSound() const  {
    cout << name << " (" << species << ") buzzes!\n";
}

void Insect::move() const  {
    cout << name << " crawls or flies.\n";
}

//===========Arachnid===========
Arachnid::Arachnid(const string& name, const string& species, int age, double weight, const string& type)
        : Animal(name, species, age, weight, type) {}
void Arachnid::makeSound() const  {
    cout << name << " (" << species << ") is silent.\n";
}

void Arachnid::move() const  {
    cout << name << " crawls slowly.\n";
}
//===========AnimalManager===========
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

    if (animalIt == animals.end() || itAviary == zooGraph.getAviaries().end()) return false;

    auto aviary = dynamic_pointer_cast<Aviary>(itAviary->second);
    auto animal = animalIt->second;

    cout << "Animal \"" << animal->getName() << "\" added into \"" << aviary->getName() << "\".\n";
    return aviary->addAnimal(animalIt->second);
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

void AnimalManager::feedById(const std::string& animalId) {
    auto it = animals.find(animalId);
    if (it == animals.end()) {
        std::cout << "Animal with ID " << animalId << " not found.\n";
        return;
    }

    auto& animal = it->second;
    animal->feed();
}


void AnimalManager::listAllAnimals() const {
    cout << "\n=== All animals ===\n";
    int i = 1;
    for (const auto& [id, a] : animals)
        cout << i++ << ". [" << a->getId() << "] " << a->getName() << " (" << a->getSpecies() << ")\n";
}
