#include "AnimalRepository.h"
#include "../Creatures/AnimalManager.h"
#include "../Logger/LoggerGlobal.h"
#include "../Graphs/ZooGraph.h"
#include "SQLUtilities/SQLUtils.h"
#include <unordered_map>
#include <memory>
#include <sstream>

using namespace std;

void AnimalRepository::initTable() {
    string sql = R"(
            CREATE TABLE IF NOT EXISTS Animals (
                id CHAR(36) PRIMARY KEY,
                name TEXT,
                species TEXT,
                type TEXT,
                aviaryId CHAR(36),
                age INTEGER,
                weight DOUBLE PRECISION,
                FOREIGN KEY (aviaryId) REFERENCES Aviaries(id) ON DELETE SET NULL
            );
        )";
    db.execute(sql);
    if (!db.getDB()) {
        logger.error("Database pointer is null before initTable!");
        return;
    }
    const char* dbFile = sqlite3_db_filename(db.getDB(), "main");
    logger.debug(string("AnimalsRepository::initTable - DB file: ") + (dbFile ? dbFile : "unknown"));

    if (!db.execute(sql)) {
        logger.error("Failed to create table 'Animals'. See previous SQL error.");
        return;
    }
    logger.info("Table 'Animals' ensured.");

}

void AnimalRepository::addAnimal(const Animal& a) {
    string safeName = escapeSQL(a.getName());
    string safeSpecies = escapeSQL(a.getSpecies());

    string sql = "INSERT INTO Animals (id, name, species, type, aviaryId, age, weight) VALUES ('" +
        a.getId() + "', '" +
        safeName + "', '" +
        safeSpecies + "', '" +
        a.getType() + "', '" +
        a.getAviaryId() + "', " +
        to_string(a.getAge()) + ", " +
        to_string(a.getWeight()) + ");";
    bool ok = db.execute(sql);
    if (ok) logger.info("Animal added: " + a.getId());
    else logger.error("Failed to insert Animal: " + a.getId());
}

bool AnimalRepository::removeAnimal(const string& id) {
    string sql = "DELETE FROM Animals WHERE id = '" + id + "';";
    return db.execute(sql);
}

void AnimalRepository::addAnimalInAviary(const string& aviaryId, const string& animalId) {
    string sql1 = "UPDATE Animals SET aviaryId = '" + aviaryId + "' WHERE id = '" + animalId + "';";
    db.execute(sql1);

    string selectSql = "SELECT animals FROM Aviaries WHERE id = '" + aviaryId + "';";
    sqlite3_stmt* stmt;
    string currentAnimals;

    if (sqlite3_prepare_v2(db.getDB(), selectSql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* text = sqlite3_column_text(stmt, 0);
            if (text) currentAnimals = reinterpret_cast<const char*>(text);
        }
    }
    sqlite3_finalize(stmt);

    if (!currentAnimals.empty()) currentAnimals += ",";
    currentAnimals += animalId;

    string updateSql = "UPDATE Aviaries SET animals = '" + currentAnimals + "' WHERE id = '" + aviaryId + "';";
    db.execute(updateSql);
}

void AnimalRepository::removeAnimalFromAviary(const string& aviaryId, const string& animalId) {
    string sql1 = "UPDATE Animals SET aviaryId = NULL WHERE id = '" + animalId + "';";
    db.execute(sql1);

    string selectSql = "SELECT animals FROM Aviaries WHERE id = '" + aviaryId + "';";
    sqlite3_stmt* stmt;
    string currentAnimals;

    if (sqlite3_prepare_v2(db.getDB(), selectSql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* text = sqlite3_column_text(stmt, 0);
            if (text) currentAnimals = reinterpret_cast<const char*>(text);
        }
    }
    sqlite3_finalize(stmt);

    if (!currentAnimals.empty()) {
        stringstream ss(currentAnimals);
        string token, newList;
        bool first = true;

        while (getline(ss, token, ',')) {
            if (token != animalId) {
                if (!first) newList += ",";
                newList += token;
                first = false;
            }
        }
        string updateSql = "UPDATE Aviaries SET animals = '" + newList + "' WHERE id = '" + aviaryId + "';";
        db.execute(updateSql);
    }
}

bool AnimalRepository::moveAnimal(const string& id, const string& oldAviaryId, const string& newAviaryId) {
    removeAnimalFromAviary(oldAviaryId,id);
    addAnimalInAviary(newAviaryId, id);
}

unordered_map<string, shared_ptr<Animal>> AnimalRepository::getAllAnimals() {
    unordered_map<string, shared_ptr<Animal>> animals;
    string sql = "SELECT * FROM Animals;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db.getDB(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Request preparation error: " << sqlite3_errmsg(db.getDB()) << endl;
        return animals;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string id        = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string name      = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string species   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        string type      = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        string aviaryId;
        const unsigned char* aviaryText = sqlite3_column_text(stmt, 4);
        if (aviaryText) aviaryId = reinterpret_cast<const char*>(aviaryText);

        int age          = sqlite3_column_int(stmt, 5);
        double weight    = sqlite3_column_double(stmt, 6);

        shared_ptr<Animal> animal;

        if (type == "Mammal")
            animal = make_shared<Mammal>(id, name, species, age, weight, "Mammal", aviaryId);
        else if (type == "Reptile")
            animal = make_shared<Reptile>(id, name, species, age, weight, "Reptile", aviaryId);
        else if (type == "Bird")
            animal = make_shared<Bird>(id, name, species, age, weight, "Bird", aviaryId);
        else if (type == "Fish")
            animal = make_shared<Fish>(id, name, species, age, weight, "Fish", aviaryId);
        else if (type == "Amphibian")
            animal = make_shared<Amphibian>(id, name, species, age, weight, "Amphibian", aviaryId);
        else if (type == "Arachnid")
            animal = make_shared<Arachnid>(id, name, species, age, weight, "Arachnid", aviaryId);
        else if (type == "Insect")
            animal = make_shared<Insect>(id, name, species, age, weight, "Insect", aviaryId);
        else
            logger.error("Unknown species while loading from datebase: " + species);

        animals[id] = animal;
    }

    sqlite3_finalize(stmt);
    return animals;
}


void AnimalRepository::clearAll() {
    db.execute("DELETE FROM Animals;");
}