#include "types.h"
#include "edge.h"

Edge::Edge(VertexPtr startVertex, VertexPtr endVertex) : mStartVertex(startVertex)
                                                       , mEndVertex(endVertex)
{
}

Edge::~Edge()
{
}
