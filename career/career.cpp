#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

class Edge;
class Vertex;
class Graph;

class Edge
{
public:
    Edge(shared_ptr<Vertex> start, shared_ptr<Vertex> end) : mStartVertex(start)
                                                           , mEndVertex(end)
    {}
    ~Edge() {}

    shared_ptr<Vertex> getStartVertex() { return mStartVertex; }
    shared_ptr<Vertex> getEndVertex() { return mEndVertex; }

private:
    shared_ptr<Vertex> mStartVertex;
    shared_ptr<Vertex> mEndVertex;
};

class Vertex
{
public:
    Vertex(unsigned short index, unsigned short weight) : mIndex(index)
                                                        , mWeight(weight) { }
    ~Vertex() { }

    void addOutboundEdge(std::shared_ptr<Edge> edge)
    {
        mOutboundEdges.push_back(edge);
    }

    unsigned short getIndex() { return mIndex; }
    unsigned short getWeight() { return mWeight; }
    vector<shared_ptr<Edge>>&   getOutboundEdges() { return mOutboundEdges; }

private:
    unsigned short              mIndex;
    unsigned short              mWeight;
    vector<shared_ptr<Edge>>    mOutboundEdges;
};

class Graph
{
public:
    Graph(vector<shared_ptr<Vertex>> vertices, vector<shared_ptr<Edge>> edges, unsigned short levelsCount)
                                                                                : mVertices(vertices)
                                                                                , mEdges(edges)
                                                                                , mLevelsCount(levelsCount)  { }

    ~Graph() {}

    vector<shared_ptr<Vertex>>& getVertices() { return mVertices; }
    vector<shared_ptr<Edge>>&   getEdges() { return mEdges; }
    unsigned short              getLevelsCount() { return mLevelsCount; }

private:
    vector<shared_ptr<Vertex>> mVertices;
    vector<shared_ptr<Edge>>   mEdges;
    unsigned short             mLevelsCount; // I know that's not the best place where we should keep level :)
};

//
shared_ptr<Graph> readCompanyStruct(const string& inputFilePath);
unsigned int findMaxExperience(shared_ptr<Graph> comapnyStruct);
void writeResultToDisk(const string& outputFilePath, unsigned int maxExperience);

unsigned int dijkstra(shared_ptr<Graph> graph, shared_ptr<Vertex> startVertex, shared_ptr<Vertex> endVertex);

int main(int argc, char** argv)
{
    string inputFilePath = (argc > 1) ? argv[1] : "ijones.in";
    string outputFilePath = (argc > 2) ? argv[2] : "ijones.out";

    shared_ptr<Graph> companyStruct = readCompanyStruct(inputFilePath);

    if (companyStruct)
    {
        unsigned int  maxExperience = findMaxExperience(companyStruct);
        writeResultToDisk(outputFilePath, maxExperience);
    }
    else
    {
        cout << "Can't read company struct from file" << endl;
    }

    return 1;
}

shared_ptr<Graph> readCompanyStruct(const string& inputFilePath)
{
    ifstream inputFile(inputFilePath);
    if (inputFile.is_open())
    {
        vector<shared_ptr<Vertex>>  vertices;
        vector<shared_ptr<Edge>>    edges;
        unsigned short              levelsCount;

        inputFile >> levelsCount;

        unsigned short experience = 0, vertexIndexInLevel = 0;
        
        // create vertices
        int vertexIndex = 0;
        for (int currentLevel = 1; currentLevel <= levelsCount; ++currentLevel)
        {
            vertexIndexInLevel = currentLevel;

            // read position experience
            while (vertexIndexInLevel > 0)
            {
                inputFile >> experience;
                shared_ptr<Vertex> vertex(new Vertex(vertexIndex, experience));
                vertices.push_back(vertex);

                vertexIndex++;
                vertexIndexInLevel--;
            }
        }

        // make connection between vertices
        vertexIndex = vertices.size() - 1;
        for (int currentLevel = levelsCount; currentLevel > 1; --currentLevel)
        {
            vertexIndexInLevel = currentLevel;
            while (vertexIndexInLevel > 0)
            {
                shared_ptr<Vertex> startVertex = vertices[vertexIndex];
                shared_ptr<Vertex> endVertex, endVertex2;
                // we have 1 outbound edge for most left & right vertex
                if (vertexIndexInLevel == currentLevel) // the right vertex
                {
                    endVertex = vertices[vertexIndex - currentLevel];
                } 
                else if(vertexIndexInLevel == 1 && currentLevel != 1) // the left vertex and not root
                {
                    endVertex = vertices[vertexIndex - currentLevel + 1];
                }
                else // middle vertex
                {
                    endVertex = vertices[vertexIndex - currentLevel + 1];
                    endVertex2 = vertices[vertexIndex - currentLevel];
                }

                // create edges
                if (endVertex)
                {
                    shared_ptr<Edge> edge = make_shared<Edge>(startVertex, endVertex);
                    startVertex->addOutboundEdge(edge);
                    edges.push_back(edge);
                }

                if (endVertex2)
                {
                    shared_ptr<Edge> edge = make_shared<Edge>(startVertex, endVertex2);
                    startVertex->addOutboundEdge(edge);
                    edges.push_back(edge);
                }

                vertexIndex--;
                vertexIndexInLevel--;
            }
        }

        return make_shared<Graph>(vertices, edges, levelsCount);
    }

    return nullptr;
}

unsigned int findMaxExperience(shared_ptr<Graph> comapnyStruct)
{
    vector<shared_ptr<Vertex>>& vertices = comapnyStruct->getVertices();
    int levelsCount = comapnyStruct->getLevelsCount() - 1;

    // launch dijkstra's algorithm from each leaves and choose the most experienced one
    int vertexIndex = vertices.size() - 1;
    unsigned int maxExperience = 0;

    while (levelsCount >= 0)
    {
        unsigned int experience = dijkstra(comapnyStruct, vertices[vertexIndex - levelsCount], vertices[0]);
        maxExperience = std::max(experience, maxExperience);
        levelsCount--;
    }

    return maxExperience;
}

void writeResultToDisk(const string& outputFilePath, unsigned int maxExperience)
{
    ofstream outputFile(outputFilePath, ifstream::out);
    if (outputFile.is_open())
    {
        outputFile << maxExperience;
        outputFile.close();
    }
    else
        cout << "Can't open file : " << endl;
}

unsigned int dijkstra(shared_ptr<Graph> graph, shared_ptr<Vertex> startVertex, shared_ptr<Vertex> endVertex)
{
    vector<shared_ptr<Vertex>>& vertices = graph->getVertices();
    int verticesCount = vertices.size();

    // visited vertices
    int verticesCountVisited = 0;
    bool* visitedVertices = new bool[verticesCount];
    memset(visitedVertices, 0, sizeof(bool) * verticesCount);

    // distances
    unsigned int* distances = new unsigned int[verticesCount]; // by default all distances = inf and start distance = 0, for max path start equal 0
    for (int i = 0; i < verticesCount; ++i) distances[i] = 0;
    distances[startVertex->getIndex()] = startVertex->getWeight();

    int vertexWithLongestDistance = startVertex->getIndex();
    while (verticesCountVisited < verticesCount)
    {
        // looking for unvisited vertex with the min distance
        for (int i = 0; i < verticesCount; ++i)
            if (visitedVertices[i] == false && distances[i] > distances[vertexWithLongestDistance])
                vertexWithLongestDistance = i;

        visitedVertices[vertexWithLongestDistance] = true;

        if (vertexWithLongestDistance == endVertex->getIndex()) // we have found the shortest path to end vertex
            break;

        // looking for better way then we know at the moment
        vector<shared_ptr<Edge>>& outBoundEdges = vertices[vertexWithLongestDistance]->getOutboundEdges();
        for (shared_ptr<Edge> edge : outBoundEdges)
        {
            int endVertexIndex = edge->getEndVertex()->getIndex();
            int newDistance = distances[vertexWithLongestDistance] + vertices[endVertexIndex]->getWeight();

            if (newDistance > distances[endVertexIndex])
                distances[endVertexIndex] = newDistance;
        }

        vertexWithLongestDistance = 0;
        verticesCountVisited++;
    }

    int shortestDistance = distances[endVertex->getIndex()];
    delete[] distances;
    delete[] visitedVertices;

    return shortestDistance;
}