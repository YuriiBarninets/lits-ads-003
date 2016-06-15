#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <list>
using namespace std;

class Graph {
public:
    Graph(list<VertexPtr> vertices, list<EdgePtr> edges);
    Graph();
    ~Graph();

    list<VertexPtr>& getVertices() { return mVertices; }
    list<EdgePtr>& getEdges() { return mEdges; }

private:
    list<VertexPtr> mVertices;
    list<EdgePtr> mEdges;
};

#endif // _GRAPH_H_