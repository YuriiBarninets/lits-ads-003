#ifndef _TYPES_H_
#define _TYPES_H

#include <memory>

// forward declaration
class Vertex;
class Edge;
class Graph;

using VertexPtr = std::shared_ptr<Vertex>;
using EdgePtr = std::shared_ptr<Edge>;
using GraphPtr = std::shared_ptr<Graph>;

#endif // _TYPES_H