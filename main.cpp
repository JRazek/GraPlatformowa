#include <iostream>

#include <vector>
using namespace std;
class Edge;
class Vertex;

class Edge{
    Vertex * input;
    Vertex * output;
    int value;
};
class Vertex{
    vector<Edge *> edges;
    int id;
    Vertex(int id){
        this->id = id;
    }
    void addEdge(Edge * e){
        this->edges.push_back(e);
    }
};

vector<string> split(string str, char divider){
    vector<string> result;

    string currWord = "";
    for(int i = 0; i < str.size(); i ++){
        currWord+=str[i];
        if(str[i] == divider || str.size()-1 == i){
            result.push_back(currWord);
            currWord = "";
        }
    }
    return result;
}

int main() {
    string line;
    getline(cin, line);
    vector<string> args = split(line, ' ');
    for(int i = 0; i < args.size(); i ++){
        cout<<args.at(i);
    }
    return 0;
}