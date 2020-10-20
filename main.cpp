#include <iostream>

#include <vector>
using namespace std;
class Edge;
class Vertex;

class Edge{
    Vertex * input;
    Vertex * output;
    int value;
public:
    void setInputVertex(Vertex * v){
        this->input = v;
    }
    void setOutputVertex(Vertex * v){
        this->output = v;
    }
};
class Vertex{
    int startPos;
    int endPos;
    int floor;
    vector<Edge *> edges;
    int id;
public:
    void addEdge(Edge * e){
        this->edges.push_back(e);
    }
    vector<Edge*> getEdges(){
        return edges;
    }
    Vertex(int id, int startPos, int endPos, int floor){
        this->id = id;
        this->startPos = startPos;
        this->endPos = endPos;
        this->floor = floor;
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
    int platforms = stoi(args[0]);
    int length = stoi(args[1]);
    int requests = stoi(args[2]);
    int startPos = 0;

    vector<Vertex *> vertices;

    for(int i = 0; i < platforms; i ++){
        getline(cin, line);
        args = split(line, ' ');
        int holes = stoi(args[0]);
        for(int j = 0; j < holes + 1; j++){
            int holePos = stoi(args[j+1]);
            Vertex * v = new Vertex(i*j + j, startPos, holePos - 1, i);
            Edge * edge = new Edge();
            edge->setInputVertex(v);
            v->addEdge(edge);
            vertices.push_back(v);
            if(j != 0){
                Edge * prevEdge = vertices.at(i * j + j - 1)->getEdges().at(0);//there must be just one.
                prevEdge->setOutputVertex(v);
                v->addEdge(prevEdge);
            }
            startPos = holePos + 1;
        }
    }
    return 0;
}