#ifndef _EDGE_H_
#define _EDGE_H_

class Edge {
public:
    Edge(VertexPtr startVertex, VertexPtr endVertex);
    ~Edge();

    VertexPtr getStartVertex() { return mStartVertex; }
    VertexPtr getEndVertex() { return mEndVertex; }

private:
    VertexPtr mStartVertex;
    VertexPtr mEndVertex;
};

#endif // _EDGE_H_