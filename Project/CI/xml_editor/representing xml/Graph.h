#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <fstream>

const int NULL_EDGE = 0;

template<class VertexType>
class Graph {
private:
    int numVertices;
    int maxVertices;
    VertexType* vertices;
    int** edges;

    int IndexIs(VertexType* vertices, VertexType vertex) const;

public:
    Graph(int maxV);
    ~Graph();
    void AddVertex(VertexType vertex);
    void AddEdge(VertexType fromVertex, VertexType toVertex);
    void VisualizeGraph(const std::string& filename) const;
};

#include "Graph.tpp" // Include the implementation

#endif // GRAPH_H
