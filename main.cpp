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
    Edge(int value){
        this->value = value;
    }
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

    vector<Vertex *> vertices;

    for(int i = 0; i < platforms; i ++){
        getline(cin, line);
        args = split(line, ' ');
        int holesCount = stoi(args[0]);
        int startPos = 0;

        for(int j = 0; j < holesCount + 1; j++){//because there are n+1 platforms per level
            int endPos;
            if(j == holesCount){
                endPos = length - 1;
            }else{
                endPos = stoi(args[j+1]) - 2;
            }
            Vertex * v = new Vertex(i * j + j, startPos, endPos, i);
            vertices.push_back(v);
            if(j != holesCount){
                Edge * e = new Edge(1);
                e->setInputVertex(v);
                v->addEdge(e);
                startPos = stoi(args[j + 1]);
            }
            if(j != 0){
                Edge * prevEdge = vertices.at(j * i + j - 1)->getEdges().at(0); //there must be only one edge!
                v->addEdge(prevEdge);
                prevEdge->setOutputVertex(v);
            }
        }
    }
    //now insert all down and up edges
    return 0;
}