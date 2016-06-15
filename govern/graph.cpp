#include "types.h"
#include "graph.h"

Graph::Graph(map<string, VertexPtr> vertices, list<EdgePtr> edges) : mVertices(vertices)
                                                                   , mEdges(edges)
{
}

Graph::Graph()
{
}

Graph::~Graph()
{
}
