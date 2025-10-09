#ifndef ANIMALREPOSITORY_H
#define ANIMALREPOSITORY_H
#include "DatabaseManager.h"
#include "../Creatures/AnimalManager.h"

class AnimalRepository {
private:
    DatabaseManager& db;

public:
    AnimalRepository(DatabaseManager& database) : db(database) {}
    void initTable();

    void addAnimal(const Animal& a);
    bool removeAnimal(const string& id);

    void addAnimalInAviary(const std::string& aviaryId, const std::string& animalId);
    void removeAnimalFromAviary(const std::string& aviaryId, const std::string& animalId);
    bool moveAnimal(const string& id, const string& newAviaryId);

    unordered_map<string, shared_ptr<Animal>> getAllAnimals();
    void clearAll();
};

#endif //ANIMALREPOSITORY_H
