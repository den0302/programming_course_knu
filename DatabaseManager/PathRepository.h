#ifndef PATHREPOSITORY_H
#define PATHREPOSITORY_H

#include "DatabaseManager.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>
#include <string>

using namespace std;

class Path;

class PathRepository {
private:
    DatabaseManager& db;

public:
    explicit PathRepository(DatabaseManager& database) : db(database) {}

    void initTable();

    void addPath(const string& fromId, const string& toId, double length);

    void removePath(const string& fromId, const string& toId);

    vector<shared_ptr<Path>> getAllPaths();

    void clearAll();
};

#endif // PATHREPOSITORY_H
