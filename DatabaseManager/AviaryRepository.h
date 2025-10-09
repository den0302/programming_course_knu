#ifndef AVIARYREPOSITORY_H
#define AVIARYREPOSITORY_H

#include "DatabaseManager.h"
#include "../Graphs/ZooGraph.h"
#include <unordered_map>
#include <memory>
#include <iostream>

class AviaryRepository {
private:
    DatabaseManager& db;

public:
    explicit AviaryRepository(DatabaseManager& database) : db(database) {}

    void initTable();

    void addAviary(const Aviary& a);
    void removeAviary(const std::string& id);

    void updateAviaryEmployee(const std::string& aviaryId, const std::string& employeeId);

    std::unordered_map<std::string, std::shared_ptr<Aviary>> getAllAviaries();

    void clearAll();
};

#endif // AVIARYREPOSITORY_H
