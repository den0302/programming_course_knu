#define byte win_byte_override
#include <windows.h>
#undef byte
#include "Animals.h"
#include "../Graphs/ZooGraph.h"
#include <algorithm>
#include <iostream>
#include <rpcdce.h>
#include <iomanip>

using namespace std;

//===========Animal===========
Animal::Animal(const string& name, const string& species, int age, double weight, const string& type)
    : name(name), species(species), age(age), weight(weight), type(type), aviaryId("") {

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
string Animal::getAviaryId() const { return aviaryId; }

void Animal::setAviaryId(const string& id) { aviaryId = id; }
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

bool Animal::isCompatibleWith(const shared_ptr<Animal>& other) const {
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
