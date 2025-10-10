#ifndef AVIARYREPOSITORY_H
#define AVIARYREPOSITORY_H

#include "DatabaseManager.h"
#include <unordered_map>
#include <memory>
#include <iostream>
class Aviary;

using namespace std;

class AviaryRepository {
private:
    DatabaseManager& db;

public:
    explicit AviaryRepository(DatabaseManager& database) : db(database) {}

    void initTable();

    void addAviary(const Aviary& a);
    void removeAviary(const string& id);

    void updateAviaryEmployee(const string& aviaryId, const string& employeeId);

    unordered_map<string, shared_ptr<Aviary>> getAllAviaries();

    void clearAll();
};

#endif // AVIARYREPOSITORY_H
