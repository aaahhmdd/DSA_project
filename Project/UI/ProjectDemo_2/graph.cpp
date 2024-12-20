#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>

template <class VertexType>
class Graph {
public:
    Graph(int maxV);
    ~Graph();
    void AddVertex(VertexType vertex);
    void AddEdge(VertexType fromVertex, VertexType toVertex);
    void VisualizeGraph(const std::string& filename) const;

private:
    int numVertices;
    int maxVertices;
    VertexType* vertices;
    int** edges;

    int IndexIs(VertexType* vertices, VertexType vertex) const;
};

#include "Graph.tpp"  // Include the implementation for template

#endif
