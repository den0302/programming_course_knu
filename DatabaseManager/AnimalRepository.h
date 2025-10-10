#ifndef ANIMALREPOSITORY_H
#define ANIMALREPOSITORY_H
#include "DatabaseManager.h"
#include <unordered_map>
#include <memory>

using namespace std;

class AnimalManager;
class Animal;

class AnimalRepository {
private:
    DatabaseManager& db;

public:
    AnimalRepository(DatabaseManager& database) : db(database) {}
    void initTable();

    void addAnimal(const Animal& a);
    bool removeAnimal(const string& id);

    void addAnimalInAviary(const string& aviaryId, const string& animalId);
    void removeAnimalFromAviary(const string& aviaryId, const string& animalId);
    bool moveAnimal(const string& id, const string& oldAviaryId, const string& newAviaryId);

    unordered_map<string, shared_ptr<Animal>> getAllAnimals();
    void clearAll();
};

#endif //ANIMALREPOSITORY_H
