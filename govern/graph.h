#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <map>
#include <list>
using namespace std;

class Graph {
public:
    Graph(map<string, VertexPtr> vertices, list<EdgePtr> edges);
    Graph();
    ~Graph();

    map<string, VertexPtr>& getVertices() { return mVertices; }
    list<EdgePtr>& getEdges() { return mEdges; }

private:
    map<string, VertexPtr> mVertices;
    list<EdgePtr> mEdges;
};

#endif // _GRAPH_H_