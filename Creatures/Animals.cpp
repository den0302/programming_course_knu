#define byte win_byte_override
#include <windows.h>
#undef byte
#include "Animals.h"
#include "../Graphs/ZooGraph.h"
#include "../Logger/LoggerGlobal.h"
#include <iostream>
#include <rpcdce.h>
#include <iomanip>

//===========Animal===========
Animal::Animal(const string& name, const string& species, int age, double weight, const string& type)
    : name(name), species(species), age(age), weight(weight), type(type) {

    UUID uuid;
    RPC_STATUS status = UuidCreate(&uuid);
    if (status != RPC_S_OK && status != RPC_S_UUID_LOCAL_ONLY && status != RPC_S_UUID_NO_ADDRESS) {
        logger.error("UuidCreate failed, RPC_STATUS = " + to_string(status));
        throw runtime_error("UuidCreate failed, RPC_STATUS = " + to_string(status));
    }

    RPC_CSTR strUuid = nullptr;
    status = UuidToStringA(&uuid, &strUuid);
    if (status != RPC_S_OK) {
        logger.error("UuidToString failed, RPC_STATUS = " + to_string(status));
        throw runtime_error("UuidToString failed, RPC_STATUS = " + to_string(status));
    }

    id = string(reinterpret_cast<char*>(strUuid));
    RpcStringFreeA(&strUuid);

    logger.info("Created Animal: " + name + " (" + species + "), ID: " + id);
}

Animal::Animal(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId)
    : id(id), name(std::move(name)), species(std::move(species)), age(age), weight(weight), type(type), aviaryId(std::move(aviaryId)) {
    logger.info("Loading Animal from file with id: " + id);
}


string Animal::getId() const {
    logger.debug("getId() called for Animal: " + name);
    return id;
}

string Animal::getName() const {
    logger.debug("getName() called for Animal: " + name);
    return name;
}

string Animal::getSpecies() const {
    logger.debug("getSpecies() called for Animal: " + name);
    return species;
}

string Animal::getType() const { return type; }

int Animal::getAge() const {
    logger.debug("getAge() called for Animal: " + name);
    return age;
}

double Animal::getWeight() const {
    logger.debug("getWeight() called for Animal: " + name);
    return weight;
}

bool Animal::getIsFed() const {
    logger.debug("getIsFed() called for Animal: " + name);
    return isFed;
}

string Animal::getAviaryId() const {
    logger.debug("getAviaryId() called for Animal: " + name);
    return aviaryId;
}

void Animal::setAviaryId(const string& id) {
    aviaryId = id;
    logger.info("Animal " + name + " assigned to aviary " + id);
}

void Animal::setName(const string& n) {
    logger.debug("Animal " + name + " name changed to " + n);
    name = n;
}

void Animal::setAge(int a) {
    logger.debug("Animal " + name + " age updated to " + to_string(a));
    age = a;
}

void Animal::setWeight(double w) {
    logger.debug("Animal " + name + " weight updated to " + to_string(w));
    weight = w;
}

void Animal::feed() {
    if (!isFed) {
        isFed = true;
        cout << name << " (" << species << ") ate.\n";
        logger.info("Animal " + name + " (" + species + ") fed successfully");
    } else {
        cout << name << " already full.\n";
        logger.warn("Animal " + name + " already fed earlier");
    }
}

void Animal::move() const {
    cout << name << " moves across the territory.\n";
    logger.debug("Animal " + name + " moved across the territory");
}

bool Animal::isCompatibleWith(const shared_ptr<Animal>& other) const {
    logger.debug("Checking compatibility between " + name + " (" + species + ") and " + other->name + " (" + other->species + ")");

    if ((species == "Lion" && other->species == "Tiger") ||
        (species == "Tiger" && other->species == "Lion") ||
        (species == "Wolf" && other->species == "Bear") ||
        (species == "Bear" && other->species == "Wolf")) {
        logger.warn("Incompatible: predator species conflict (" + species + " vs " + other->species + ")");
        return false;
    }

    if ((species == "Eagle" && other->species == "Parrot") ||
        (species == "Parrot" && other->species == "Eagle") ||
        (species == "Owl" && other->species == "Crow") ||
        (species == "Crow" && other->species == "Owl")) {
        logger.warn("Incompatible: bird conflict (" + species + " vs " + other->species + ")");
        return false;
    }

    if ((species == "Snake" && (other->type == "Mammal" || other->type == "Bird")) ||
        ((other->species == "Snake") && (type == "Mammal" || type == "Bird"))) {
        logger.warn("Incompatible: reptile vs mammal/bird (" + species + " vs " + other->species + ")");
        return false;
    }

    if ((type == "Fish" && other->type == "Fish") &&
        ((species == "Piranha") || (other->species == "Piranha"))) {
        logger.warn("Incompatible: aggressive fish detected (" + species + " vs " + other->species + ")");
        return false;
    }

    if ((type == "Amphibian" && other->type == "Insect") ||
        (type == "Insect" && other->type == "Amphibian")) {
        logger.warn("Incompatible: amphibian vs insect (" + species + " vs " + other->species + ")");
        return false;
    }

    if ((type == "Arachnid") &&
        (other->type == "Insect" || other->type == "Amphibian" || other->type == "Fish")) {
        logger.warn("Incompatible: arachnid vs small species (" + species + " vs " + other->species + ")");
        return false;
    }

    logger.info("Animals " + name + " and " + other->name + " are compatible");
    return true;
}

void Animal::printInfoAboutAnimal() const {
    cout << "ID: " << id << "\n"
         << "Name: " << name << "\n"
         << "Species: " << species << "\n"
         << "Age: " << age << "\n"
         << "Weight: " << weight << " kg\n"
         << "Satiety: " << (isFed ? "Yes" : "No") << "\n\n";
    logger.debug("Displayed info for Animal: " + name);
}

//===========Mammal===========
Mammal::Mammal(const string& name, const string& species, int age, double weight, const string& type)
    : Animal(name, species, age, weight, type) {
    logger.info("Created Mammal: " + name + " (" + species + ")");
}

Mammal::Mammal(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId)
    : Animal (id, name, species, age, weight, type, aviaryId) {}

void Mammal::makeSound() const {
    cout << name << " (" << species << ") said: Rrrr!\n";
    logger.debug("Mammal " + name + " made a sound");
}

//===========Bird===========
Bird::Bird(const string& name, const string& species, int age, double weight, const string& type)
    : Animal(name, species, age, weight, type) {
    logger.info("Created Bird: " + name + " (" + species + ")");
}

Bird::Bird(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId)
    : Animal (id, name, species, age, weight, type, aviaryId) {}

void Bird::makeSound() const {
    cout << name << " (" << species << ") said: Tweet!\n";
    logger.debug("Bird " + name + " made a sound");
}

void Bird::move() const {
    cout << name << " flying around the aviary\n";
    logger.debug("Bird " + name + " is flying around the aviary");
}

//===========Reptile===========
Reptile::Reptile(const string& name, const string& species, int age, double weight, const string& type)
    : Animal(name, species, age, weight, type) {
    logger.info("Created Reptile: " + name + " (" + species + ")");
}

Reptile::Reptile(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId)
    : Animal (id, name, species, age, weight, type, aviaryId) {}

void Reptile::makeSound() const {
    cout << name << " (" << species << ") hisses: Ssssss!\n";
    logger.debug("Reptile " + name + " hissed");
}

//===========Fish===========
Fish::Fish(const string& name, const string& species, int age, double weight, const string& type)
        : Animal(name, species, age, weight, type) {
    logger.info("Created Fish: " + name + " (" + species + ")");
}

Fish::Fish(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId)
    : Animal (id, name, species, age, weight, type, aviaryId) {}

void Fish::makeSound() const  {
    cout << name << " (" << species << ") makes bubbling sounds!\n";
    logger.debug("Fish " + name + " made bubbling sounds");
}

void Fish::move() const  {
    cout << name << " swims in the water.\n";
    logger.debug("Fish " + name + " is swimming");
}

//===========Amphibian===========
Amphibian::Amphibian(const string& name, const string& species, int age, double weight, const string& type)
        : Animal(name, species, age, weight, type) {
    logger.info("Created Amphibian: " + name + " (" + species + ")");
}

Amphibian::Amphibian(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId)
    : Animal (id, name, species, age, weight, type, aviaryId) {}

void Amphibian::makeSound() const  {
    cout << name << " (" << species << ") croaks!\n";
    logger.debug("Amphibian " + name + " croaked");
}

void Amphibian::move() const  {
    cout << name << " hops around.\n";
    logger.debug("Amphibian " + name + " hops around");
}

//===========Insect===========
Insect::Insect(const string& name, const string& species, int age, double weight, const string& type)
        : Animal(name, species, age, weight, type) {
    logger.info("Created Insect: " + name + " (" + species + ")");
}

Insect::Insect(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId)
    : Animal (id, name, species, age, weight, type, aviaryId) {}

void Insect::makeSound() const  {
    cout << name << " (" << species << ") buzzes!\n";
    logger.debug("Insect " + name + " buzzed");
}

void Insect::move() const  {
    cout << name << " crawls or flies.\n";
    logger.debug("Insect " + name + " crawled or flew");
}

//===========Arachnid===========
Arachnid::Arachnid(const string& name, const string& species, int age, double weight, const string& type)
        : Animal(name, species, age, weight, type) {
    logger.info("Created Arachnid: " + name + " (" + species + ")");
}

Arachnid::Arachnid(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId)
    : Animal (id, name, species, age, weight, type, aviaryId) {}

void Arachnid::makeSound() const  {
    cout << name << " (" << species << ") is silent.\n";
    logger.debug("Arachnid " + name + " is silent (no sound)");
}

void Arachnid::move() const  {
    cout << name << " crawls slowly.\n";
    logger.debug("Arachnid " + name + " crawled slowly");
}
