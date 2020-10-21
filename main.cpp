#include <iostream>

#include <map>
#include <vector>
using namespace std;
class Edge;
class Vertex;

template <typename T>
class IntervalMap{
    vector<int> intervals;//from start to next start - 1
    vector<T> values;//the index corresponds to index of interval
    int endValue;//including this val
public:
    IntervalMap(int endValue){
        this->endValue = endValue;
    }
    void addValue(int start, T v){
        if(start > endValue)
            return;
        intervals.push_back(start);
        values.push_back(v);
    }
    T getValue(int key){
        int currStart = 0;
        int currEnd = values.size() - 1;//virtual vector so we dont have to assign it over and over in O(n)
        if(!(key >= 0 && key <= endValue))
            return nullptr;
        while(true){
            int pointer = (currEnd - currStart)/2 + currStart;
            int startingIntervalValueAtPointer = intervals.at(pointer);

            if (currStart >= currEnd || startingIntervalValueAtPointer == key){
                return values.at(pointer);
            }

            if(startingIntervalValueAtPointer > key){
                currEnd = pointer - 1;
            } else if(startingIntervalValueAtPointer < key){
                currStart = pointer + 1;
            }

        }
    }
};
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
    int numInFloor;
    vector<Edge *> edges;
    int id;
public:
    void addEdge(Edge * e){
        this->edges.push_back(e);
    }
    vector<Edge*> getEdges(){
        return edges;
    }
    int getNumInFloor(){
        return numInFloor;
    }
    int getFloor(){
        return floor;
    }
    int getStartPos(){
        return startPos;
    }
    int getEndPos(){
        return endPos;
    }
    Vertex(int id, int numInFloor, int startPos, int endPos, int floor){
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
    int requestsCount = stoi(args[2]);

    vector<Vertex *> vertices;
    vector<IntervalMap<Vertex * >*> intervalMapList;
    for(int i = 0; i < platforms; i ++){
        getline(cin, line);
        args = split(line, ' ');
        int holesCount = stoi(args[0]);
        int startPos = 0;
        IntervalMap<Vertex * > * intervalMap = new IntervalMap<Vertex * >(length - 1);
        for(int j = 0; j < holesCount + 1; j++){//because there are n+1 platforms per level
            int endPos;
            if(j == holesCount){
                endPos = length - 1;
            }else{
                endPos = stoi(args[j+1]) - 2;
            }
            Vertex * v = new Vertex(i * j + j, j, startPos, endPos, i);
            vertices.push_back(v);
            intervalMap->addValue(startPos, v);
            if(j != holesCount){
                Edge * e = new Edge(1);
                e->setInputVertex(v);
                v->addEdge(e);
                startPos = stoi(args[j + 1]);
                intervalMap->addValue(endPos + 1, nullptr);
            }
            if(j != 0){
                Edge * prevEdge = vertices.at(j * i + j - 1)->getEdges().at(0); //there must be only one edge!
                v->addEdge(prevEdge);
                prevEdge->setOutputVertex(v);
            }
        }
        intervalMapList.push_back(intervalMap);
    }
    //now insert all down and up edges
    Vertex * v = intervalMapList.at(1)->getValue(8);
    return 0;
}