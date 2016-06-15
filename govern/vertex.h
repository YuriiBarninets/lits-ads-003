#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <string>
#include <list>
using namespace std;

class Vertex {
public:
    Vertex(const string& label);
    ~Vertex();

    string getLabel() { return mLabel; }
    list<EdgePtr>&  getOutboundEdges() { return mOutBoundEdges; }
    
    void setLabel(const string& label) { mLabel = label; }

private:
    string mLabel;
    list<EdgePtr> mOutBoundEdges;
};

#endif // _VERTEX_H