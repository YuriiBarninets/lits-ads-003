#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <set>

#include "types.h"
#include "graph.h"
#include "vertex.h"
#include "edge.h"
#include "govern.h"

using namespace std;

GraphPtr readDocumentsFromFile(const string& inputFilePath);
VertexPtr getVertex(list<VertexPtr> vertices, const string& label);

list<VertexPtr> unvisitedVertices;
set<VertexPtr> visitedVertices;
list<string> docOrder;

void dfs(VertexPtr startVertex)
{
    list<VertexPtr> verticesStack;
    verticesStack.push_back(startVertex); // push initial vertex in stack

    while (!verticesStack.empty()) // get vertex for analysis
    {
        VertexPtr vertex = verticesStack.back();
        verticesStack.pop_back();

        if (visitedVertices.find(vertex) == visitedVertices.end())
        {
            // hard code
            unvisitedVertices.remove(vertex);
            visitedVertices.insert(vertex);
        }

        list<VertexPtr> unvisitedNeighbors;
        list<EdgePtr>& outBoundEdges = vertex->getOutboundEdges();
        for (EdgePtr edge : outBoundEdges)
        {
            // iterate over all neighborhoods and add to stack if vertex not visited
            VertexPtr vertex = edge->getEndVertex();
            if(visitedVertices.find(vertex) == visitedVertices.end())
                unvisitedNeighbors.push_back(vertex);
        }

        if (unvisitedNeighbors.empty())
        {
            docOrder.push_back(vertex->getLabel());
        }
        else
        {
            verticesStack.push_back(vertex);
            verticesStack.insert(verticesStack.end(), unvisitedNeighbors.begin(), unvisitedNeighbors.end());
        }

    }


}

int main(int argc, char** argv)
{
    string inputFilePath = "govern.in";
    string outputFilePath = "govern.out";

    if (argc > 1)
    {
        inputFilePath = argv[1];
        outputFilePath = argv[2];
    }

    //
    GraphPtr graph = readDocumentsFromFile(inputFilePath);

    // we know that our graph is acycle, so do DFS sorting
    list<VertexPtr>& vertices = graph->getVertices();
    unvisitedVertices = vertices;

    while (unvisitedVertices.size() > 0)
        dfs(*unvisitedVertices.begin());

    // write data to file
    ofstream outputFile(outputFilePath, ifstream::out);
    if (outputFile.is_open())
    {
        for (const string& doc : docOrder)
            outputFile << doc << std::endl;

        outputFile.close();
    }
    else
    {
        cout << "Can't open file : " << outputFilePath << endl;

    }

    return 1;
}

GraphPtr readDocumentsFromFile(const string& inputFilePath)
{
    ifstream inputFile(inputFilePath);
    if (inputFile.is_open())
    {
        list<VertexPtr> vertices;
        list<EdgePtr> edges;

        while (!inputFile.eof())
        {
            string startVertexLabel, endVertexLabel;
            inputFile >> startVertexLabel >> endVertexLabel;
            if (startVertexLabel.empty() || endVertexLabel.empty()) continue;

            //
            VertexPtr startVertex = getVertex(vertices, startVertexLabel);
            if (startVertex == nullptr)
            {
                startVertex = make_shared<Vertex>(startVertexLabel);
                vertices.push_back(startVertex);
            }

            VertexPtr endVertex = getVertex(vertices, endVertexLabel);
            if (endVertex == nullptr)
            {
                endVertex = make_shared<Vertex>(endVertexLabel);
                vertices.push_back(endVertex);
            }

            EdgePtr edge = make_shared<Edge>(startVertex, endVertex);
            startVertex->getOutboundEdges().push_back(edge);
            edges.push_back(edge);
        }

        inputFile.close();

        return make_shared<Graph>(vertices, edges);
    }
    else
        cout << "Can't read file : " << inputFilePath << endl;

    return nullptr;
}

VertexPtr getVertex(list<VertexPtr> vertices, const string& label)
{
    for (VertexPtr vertex : vertices)
    {
        if (vertex->getLabel() == label)
            return vertex;
    }

    return nullptr;
}
