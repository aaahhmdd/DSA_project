#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
using namespace std;

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

template<class VertexType>Graph<VertexType>::Graph(int maxV) {
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
template<class VertexType>Graph<VertexType>::~Graph() {
    delete[] vertices;
    for (int i = 0; i < maxVertices; i++) {
        delete[] edges[i];
    }
    delete[] edges;
}
template<class VertexType>void Graph<VertexType>::AddVertex(VertexType vertex) {
    vertices[numVertices] = vertex;
    for (int index = 0; index <= numVertices; index++) {
        edges[numVertices][index] = NULL_EDGE;
        edges[index][numVertices] = NULL_EDGE;
    }
    numVertices++;
}
template<class VertexType>void Graph<VertexType>::AddEdge(VertexType fromVertex, VertexType toVertex) {
    int row = IndexIs(vertices, fromVertex);
    int col = IndexIs(vertices, toVertex);
    if (row != -1 && col != -1) {
        edges[row][col] = 1; // Add directed edge
    }
}
template<class VertexType>int Graph<VertexType>::IndexIs(VertexType* vertices, VertexType vertex) const {
    for (int i = 0; i < numVertices; i++) {
        if (vertices[i] == vertex) {
            return i;
        }
    }
    return -1;
}
template<class VertexType>void Graph<VertexType>::VisualizeGraph(const std::string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file for writing." << endl;
        return;
    }

    file << "digraph G {" << endl;

    for (int i = 0; i < numVertices; i++) {
        file << "  \"" << vertices[i] << "\";" << endl;
    }

    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (edges[i][j] != NULL_EDGE) {
                file << "  \"" << vertices[i] << "\" -> \"" << vertices[j] << "\";" << endl;
            }
        }
    }

    file << "}" << endl;

    file.close();
    cout << "Graph written to " << filename << " in DOT format." << endl;
}
struct Element {
    string tag;
    string value;
    vector<Element> children;
};
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}
Element parseXML(const string& xml) {
    Element root;
    stringstream ss(xml);
    string line;
    vector<Element*> stack;


    while (getline(ss, line)) {
        bool flag=false;
        size_t start = line.find('<') + 1;
        size_t end = line.find('>');
        if (start == string::npos || end == string::npos) {
            stack.back()->value=trim(line);
            continue;
        }
        string tag = line.substr(start, end - start);
        if (tag[0] == '/') {
            stack.pop_back();
        } else {
            Element element;
            element.tag = tag;
            start = line.find('>') + 1;
            end = line.find('<', start);
            if (end != string::npos) {
                flag=true;
                element.value = line.substr(start, end - start);
            }

            if (stack.empty()) {
                root = element;
                stack.push_back(&root);
            } else {
                stack.back()->children.push_back(element);
                if(!flag)stack.push_back(&stack.back()->children.back());


            }
        }
    }
    return root;
}
void generateGraphFromMap(const map<string, vector<string>>& userFollowersMap, Graph<string>& graph) {
    for (const auto& userEntry : userFollowersMap) {
        const string& userId = userEntry.first;
        graph.AddVertex(userId);
    }
    for (const auto& userEntry : userFollowersMap) {
        const string& userId = userEntry.first;
        const vector<string>& followers = userEntry.second;

        for (const string& follower : followers) {
            graph.AddEdge(follower, userId);
        }
    }
}
void toMap(const Element& element, map<string, vector<string>>& userFollowersMap) {
    if (element.tag == "user") {
        string userId;
        vector<string> followers;
        for (const auto& child : element.children) {
            if (child.tag == "id") {
                userId = child.value;
            } else if (child.tag == "followers") {
                for (const auto& follower : child.children) {
                    if (follower.tag == "follower") {
                        for (const auto& followerChild : follower.children) {
                            if (followerChild.tag == "id") {
                                followers.push_back(followerChild.value);
                            }
                        }
                    }
                }
            }
        }
        if (!userId.empty()) {
            userFollowersMap[userId] = followers;
            cout << endl;
        }
    }
    for (const auto& child : element.children) {
        toMap(child, userFollowersMap);
    }
}



int main() {
    string filename;
    cout << "Enter the path to the XML file: ";
    cin >> filename;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string xml = buffer.str();
    file.close();
    Element root = parseXML(xml);
    map<string, vector<string>> userFollowersMap;
    toMap(root, userFollowersMap);
    Graph<string> graph(10);
    generateGraphFromMap(userFollowersMap, graph);
    graph.VisualizeGraph("graph.dot");
    //dot -Tjpg graph.dot -o graph.jpg
return 0;
}