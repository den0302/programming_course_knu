#include "AnimalRepository.h"
#include <unordered_map>
#include <memory>
#include <sstream>

#include "../Graphs/ZooGraph.h"

void AnimalRepository::initTable() {
    string sql = R"(
            CREATE TABLE IF NOT EXISTS Animals (
                id CHAR(36) PRIMARY KEY,
                name TEXT,
                species TEXT,
                type TEXT,
                aviaryId CHAR(36),
                FOREIGN KEY (aviaryId) REFERENCES Aviaries(id) ON DELETE SET NULL,
                age INTEGER,
                weight DOUBLE PRECISION
            );
        )";
    db.execute(sql);

    std::string sql2 = R"(
        CREATE TABLE IF NOT EXISTS Aviaries (
            id CHAR(36) PRIMARY KEY,
            name TEXT,
            type TEXT,
            area DOUBLE PRECISION,
            capacity INTEGER,
            animals TEXT,
            assignedEmployeeId CHAR(36),
            FOREIGN KEY (assignedEmployeeId) REFERENCES Employes(id) ON DELETE SET NULL
        );
    )";
    db.execute(sql2);
}

void AnimalRepository::addAnimal(const Animal& a) {
    string sql = "INSERT INTO Animals (id, name, species, type, aviaryId, age, weight) VALUES ('" +
        a.getId() + "', '" +
        a.getName() + "', '" +
        a.getSpecies() + "', '" +
        a.getType() + "', '" +
        a.getAviaryId() + "', " +
        to_string(a.getAge()) + ", " +
        to_string(a.getWeight()) + ");";
    db.execute(sql);
}

bool AnimalRepository::removeAnimal(const string& id) {
    string sql = "DELETE FROM Animals WHERE id = '" + id + "';";
    return db.execute(sql);
}

void AnimalRepository::addAnimalInAviary(const std::string& aviaryId, const std::string& animalId) {
    std::string sql1 = "UPDATE Animals SET aviaryId = '" + aviaryId + "' WHERE id = '" + animalId + "';";
    db.execute(sql1);

    std::string selectSql = "SELECT animals FROM Aviaries WHERE id = '" + aviaryId + "';";
    sqlite3_stmt* stmt;
    std::string currentAnimals;

    if (sqlite3_prepare_v2(db.getDB(), selectSql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* text = sqlite3_column_text(stmt, 0);
            if (text) currentAnimals = reinterpret_cast<const char*>(text);
        }
    }
    sqlite3_finalize(stmt);

    if (!currentAnimals.empty()) currentAnimals += ",";
    currentAnimals += animalId;

    std::string updateSql = "UPDATE Aviaries SET animals = '" + currentAnimals + "' WHERE id = '" + aviaryId + "';";
    db.execute(updateSql);
}

void AnimalRepository::removeAnimalFromAviary(const std::string& aviaryId, const std::string& animalId) {
    std::string sql1 = "UPDATE Animals SET aviaryId = NULL WHERE id = '" + animalId + "';";
    db.execute(sql1);

    std::string selectSql = "SELECT animals FROM Aviaries WHERE id = '" + aviaryId + "';";
    sqlite3_stmt* stmt;
    std::string currentAnimals;

    if (sqlite3_prepare_v2(db.getDB(), selectSql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* text = sqlite3_column_text(stmt, 0);
            if (text) currentAnimals = reinterpret_cast<const char*>(text);
        }
    }
    sqlite3_finalize(stmt);

    if (!currentAnimals.empty()) {
        std::stringstream ss(currentAnimals);
        std::string token, newList;
        bool first = true;

        while (std::getline(ss, token, ',')) {
            if (token != animalId) {
                if (!first) newList += ",";
                newList += token;
                first = false;
            }
        }
        std::string updateSql = "UPDATE Aviaries SET animals = '" + newList + "' WHERE id = '" + aviaryId + "';";
        db.execute(updateSql);
    }
}

bool AnimalRepository::moveAnimal(const string& id, const string& newAviaryId) {
    string sql =
        "UPDATE Animals SET aviaryId = '" + newAviaryId +
        "' WHERE id = '" + id + "';";
    return db.execute(sql);
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

        std::string aviaryId;
        const unsigned char* aviaryText = sqlite3_column_text(stmt, 4);
        if (aviaryText) aviaryId = reinterpret_cast<const char*>(aviaryText);

        int age          = sqlite3_column_int(stmt, 5);
        double weight    = sqlite3_column_double(stmt, 6);

        /*if (type != "Mammal" &&
            type !=  "Reptile" &&
            type !=   "Bird" &&
            type !=   "Fish" &&
            type !=   "Amphibian" &&
            type !=   "Arachnid" &&
            type !=   "Insect") {
        }*/

        auto animal = ZooGraph::getInstance().getAnimalManager().createAnimal(id, name, species, age, weight, type, aviaryId);
        //auto animal = make_shared<Animal>(id, name, species, age, weight, type, aviaryId);
        animals[id] = animal;
    }

    sqlite3_finalize(stmt);
    return animals;
}


void AnimalRepository::clearAll() {
    db.execute("DELETE FROM Animals;");
}