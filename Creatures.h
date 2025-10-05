#ifndef CREATURES_H
#define CREATURES_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
class ZooGraph;

using namespace std;

class Animal {
protected:
    string id;
    string name;
    string species;  // вид тварини(лев, тигр, папуга...)
    string type;
    int age;
    double weight;
    bool isFed = false;

public:
    Animal(const string& name, const string& species, int age, double weight, const string& type);

    virtual ~Animal() = default;

    string getId() const;
    string getName() const;
    string getSpecies() const;
    int getAge() const;
    double getWeight() const;
    bool getIsFed() const;

    void setName(const string& n);
    void setAge(int a);
    void setWeight(double w);

    virtual void feed();
    virtual void makeSound() const = 0;
    virtual void move() const;
    virtual bool isCompatibleWith(const shared_ptr<Animal>& other) const;

    virtual void printInfoAboutAnimal() const;
};

class Mammal : public Animal {
public:
    Mammal(const string& name, const string& species, int age, double weight, const string& type);
    void makeSound() const override;
};

class Bird : public Animal {
public:
    Bird(const string& name, const string& species, int age, double weight, const string& type);
    void makeSound() const override;
    void move() const override;
};

class Reptile : public Animal {
public:
    Reptile(const string& name, const string& species, int age, double weight, const string& type);
    void makeSound() const override;
};

class Fish : public Animal {
public:
    Fish(const string& name, const string& species, int age, double weight, const string& type);

    void makeSound() const override;
    void move() const override;
};

class Amphibian : public Animal {
public:
    Amphibian(const string& name, const string& species, int age, double weight, const string& type);

    void makeSound() const override;
    void move() const override;
};

class Insect : public Animal {
public:
    Insect(const string& name, const string& species, int age, double weight, const string& type);

    void makeSound() const override;
    void move() const override;
};

class Arachnid : public Animal {
public:
    Arachnid(const string& name, const string& species, int age, double weight, const string& type);

    void makeSound() const override;
    void move() const override;
};

class AnimalManager {
private:
    ZooGraph& zooGraph;
    unordered_map<string, shared_ptr<Animal>> animals;

public:
    AnimalManager(ZooGraph& graph) : zooGraph(graph) {}

    vector<shared_ptr<Animal>> getAnimals() const {return animals}

    void createAnimal(const string& name, const string& species, int age, double weight, const string& type);

    bool addAnimalInAviary(const string& aviaryId, const string& animalId);
    bool removeAnimalFromAviary(const string& aviaryId, const string& animalId);
    bool moveAnimalBetweenAviaries(const string& fromAviaryId, const string& toAviaryId, const string& animalId);

    vector<shared_ptr<Animal>> getAnimalsNotInAviaries() const;
    bool allAnimalsAssigned() const;

    void feedById(const std::string& animalId);

    void listAllAnimals() const;
};


#endif //CREATURES_H
