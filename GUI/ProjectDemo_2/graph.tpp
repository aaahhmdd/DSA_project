#ifndef GRAPH_TPP
#define GRAPH_TPP

#include "Graph.h"
#include <fstream>


// Constructor definition
template <class VertexType>
Graph<VertexType>::Graph(int maxV) {
    numVertices = 0;
    maxVertices = maxV;
    vertices = new VertexType[maxV];
    edges = new int*[maxV];
    for (int i = 0; i < maxV; i++) {
        edges[i] = new int[maxV];
        for (int j = 0; j < maxV; j++) {
            edges[i][j] = 0; // Initialize all edges to 0
        }
    }
}

// Destructor definition
template <class VertexType>
Graph<VertexType>::~Graph() {
    delete[] vertices;
    for (int i = 0; i < maxVertices; i++) {
        delete[] edges[i];
    }
    delete[] edges;
}

// AddVertex method definition
template <class VertexType>
void Graph<VertexType>::AddVertex(VertexType vertex) {
    vertices[numVertices] = vertex;
    for (int index = 0; index <= numVertices; index++) {
        edges[numVertices][index] = 0;  // Initialize edges between new vertex and others
        edges[index][numVertices] = 0;
    }
    numVertices++;
}

// AddEdge method definition
template <class VertexType>
void Graph<VertexType>::AddEdge(VertexType fromVertex, VertexType toVertex) {
    int row = IndexIs(vertices, fromVertex);
    int col = IndexIs(vertices, toVertex);
    if (row != -1 && col != -1) {
        edges[row][col] = 1;  // Directed edge from 'fromVertex' to 'toVertex'
    }
}

// IndexIs method definition
template <class VertexType>
int Graph<VertexType>::IndexIs(VertexType* vertices, VertexType vertex) const {
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i] == vertex) {
            return i;
        }
    }
    return -1;  // Return -1 if vertex is not found
}

// VisualizeGraph method definition
template <class VertexType>
void Graph<VertexType>::VisualizeGraph(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return;
    }

    file << "digraph G {" << std::endl;

    for (int i = 0; i < numVertices; i++) {
        file << "  \"" << vertices[i] << "\";" << std::endl;
    }

    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (edges[i][j] != 0) {
                file << "  \"" << vertices[i] << "\" -> \"" << vertices[j] << "\";" << std::endl;
            }
        }
    }

    file << "}" << std::endl;

    file.close();
    std::cout << "Graph written to " << filename << " in DOT format." << std::endl;
}

#endif
