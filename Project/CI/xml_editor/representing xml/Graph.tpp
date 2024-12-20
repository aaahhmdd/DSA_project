#ifndef GRAPH_TPP
#define GRAPH_TPP

#include "Graph.h"
#include <iostream>

template<class VertexType>
Graph<VertexType>::Graph(int maxV) {
    numVertices = 0;
    maxVertices = maxV;
    vertices = new VertexType[maxV];
    edges = new int*[maxV];
    for (int i = 0; i < maxV; i++) {
        edges[i] = new int[maxV];
        for (int j = 0; j < maxV; j++) {
            edges[i][j] = NULL_EDGE;
        }
    }
}

template<class VertexType>
Graph<VertexType>::~Graph() {
    delete[] vertices;
    for (int i = 0; i < maxVertices; i++) {
        delete[] edges[i];
    }
    delete[] edges;
}

template<class VertexType>
void Graph<VertexType>::AddVertex(VertexType vertex) {
    vertices[numVertices] = vertex;
    for (int index = 0; index <= numVertices; index++) {
        edges[numVertices][index] = NULL_EDGE;
        edges[index][numVertices] = NULL_EDGE;
    }
    numVertices++;
}

template<class VertexType>
void Graph<VertexType>::AddEdge(VertexType fromVertex, VertexType toVertex) {
    int row = IndexIs(vertices, fromVertex);
    int col = IndexIs(vertices, toVertex);
    if (row != -1 && col != -1) {
        edges[row][col] = 1; // Add directed edge
    }
}

template<class VertexType>
int Graph<VertexType>::IndexIs(VertexType* vertices, VertexType vertex) const {
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i] == vertex) {
            return i;
        }
    }
    return -1;
}

template<class VertexType>
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
            if (edges[i][j] != NULL_EDGE) {
                file << "  \"" << vertices[i] << "\" -> \"" << vertices[j] << "\";" << std::endl;
            }
        }
    }

    file << "}" << std::endl;

    file.close();
    std::cout << "Graph written to " << filename << " in DOT format." << std::endl;
}

#endif // GRAPH_TPP
