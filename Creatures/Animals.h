#ifndef ANIMALS_H
#define ANIMALS_H

#include <string>
#include <memory>
class ZooGraph;

using namespace std;

class Animal {
protected:
    string id;
    string name;
    string species;  // вид тварини(лев, тигр, папуга...)
    int age;
    double weight;
    string type;
    string aviaryId;
    bool isFed = false;

public:
    Animal(const string& name, const string& species, int age, double weight, const string& type);
    Animal(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId);
    virtual ~Animal() = default;

    [[nodiscard]] string getId() const;
    [[nodiscard]] string getName() const;
    [[nodiscard]] string getSpecies() const;
    [[nodiscard]] string getType() const;
    [[nodiscard]] int getAge() const;
    [[nodiscard]] double getWeight() const;
    [[nodiscard]] bool getIsFed() const;
    [[nodiscard]] string getAviaryId() const;

    void setAviaryId(const string& id);
    void setName(const string& n);
    void setAge(int a);
    void setWeight(double w);

    virtual void feed();
    virtual void makeSound() const = 0;
    virtual void move() const;
    [[nodiscard]] virtual bool isCompatibleWith(const shared_ptr<Animal>& other) const;

    virtual void printInfoAboutAnimal() const;
};

class Mammal : public Animal {
public:
    Mammal(const string& name, const string& species, int age, double weight, const string& type);
    Mammal(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId);

    void makeSound() const override;
};

class Bird : public Animal {
public:
    Bird(const string& name, const string& species, int age, double weight, const string& type);

    Bird(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId);
    void makeSound() const override;
    void move() const override;
};

class Reptile : public Animal {
public:
    Reptile(const string& name, const string& species, int age, double weight, const string& type);
    Reptile(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId);

    void makeSound() const override;
};

class Fish : public Animal {
public:
    Fish(const string& name, const string& species, int age, double weight, const string& type);
    Fish(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId);

    void makeSound() const override;
    void move() const override;
};

class Amphibian : public Animal {
public:
    Amphibian(const string& name, const string& species, int age, double weight, const string& type);
    Amphibian(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId);

    void makeSound() const override;
    void move() const override;
};

class Insect : public Animal {
public:
    Insect(const string& name, const string& species, int age, double weight, const string& type);
    Insect(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId);

    void makeSound() const override;
    void move() const override;
};

class Arachnid : public Animal {
public:
    Arachnid(const string& name, const string& species, int age, double weight, const string& type);
    Arachnid(const string& id, string& name, string& species, int age, double weight, const string& type, string& aviaryId);

    void makeSound() const override;
    void move() const override;
};

#endif //ANIMALS_H
