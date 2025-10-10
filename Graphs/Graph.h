#ifndef GRAPH_H
#define GRAPH_H
using namespace std;

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

class Vertex {
private:
    string id;
public:
    Vertex();
    explicit Vertex(const string& id);
    virtual ~Vertex();
    string getId() const;
};

class Edge {
private:
    string fromId;
    string toId;
    double weight;
public:
    Edge(const string& from, const string& to, double w) : fromId(from), toId(to), weight(w) {}

    string getFrom() const;
    string getTo() const;
    double getWeight() const;

};

class Graph {
private:
    unordered_map<string, shared_ptr<Vertex>> vertices;
    vector<Edge> edges;
protected:
    const unordered_map<string, shared_ptr<Vertex>>& getVertices() const;
    const vector<Edge>& getEdges() const;
    const Edge* getEdge(const string& fromId, const string& toId) const;
    shared_ptr<Vertex> getVertex(const string& id) const;
    vector<string> getNeighbors(const string& id) const;

    void setVertices(const unordered_map<string, shared_ptr<Vertex>>& newVertices);
    void setEdges(const vector<Edge>& newEdges);

    void addVertex(shared_ptr<Vertex> v);
    void addEdge(const string& fromId, const string& toId, double weight);

    void removeVertex(const string& id);
    void removeEdge(const string& fromId, const string& toId);

    vector<string> findPath(const string& startId, const string& endId);
    vector<string> findPathByWeight(const string& startId, const string& endId) const;
    double distanceBetween(const std::string& fromId, const std::string& toId) const;

    bool checkConnectivity() const;
    void printGraph() const;
};
#endif //GRAPH_H
