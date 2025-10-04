#ifndef ZOOGRAPH_H
#define ZOOGRAPH_H
#include <memory>
#include <string>
#include "Graph.h"

class Aviary : public Vertex {
private:
    string name;
    string type;
    double area;
public:
    Aviary(const string& name, const string& type, double area)
        : name(name), type(type), area(area) {}

    string getName() const;
    string getType() const;
    double getArea() const;

    void setName(const string& n);
    void setType(const string& t);
    void setArea(double a);

    void printInfo() const;
};

class Path : public Edge {
public:
    Path(const std::string& from, const std::string& to, int length)
        : Edge(from, to, length) {}

    int getLength() const;
};

class ZooGraph : public Graph {
public:
    void addAviary(std::shared_ptr<Aviary> aviary);
    void removeAviary(const std::string& id);
    void addPath(const std::string& fromId, const std::string& toId, int length);
    void removePath(const std::string& fromId, const std::string& toId);
    void printZoo() const;
};

#endif //ZOOGRAPH_H
