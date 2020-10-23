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
        int currEnd = values.size();//virtual interval
        if(!(key >= 0 && key <= endValue))
            return nullptr;
        while(true){
            int pointer = (currEnd - currStart)/2 + currStart;
            int startingIntervalValueAtPointer = intervals.at(pointer);

            if (currStart == currEnd - 1 || startingIntervalValueAtPointer == key){
                return values.at(pointer);
            }

            if(startingIntervalValueAtPointer > key){
                currEnd = pointer;
            } else if(startingIntervalValueAtPointer < key){
                currStart = pointer;
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
    Vertex * getOutputVertex(){
        return output;
    }
    void setOutputVertex(Vertex * v){
        this->output = v;
    }
    int getValue(){
        return value;
    }
};
class Vertex{
    int startPos;
    int endPos;
    int floor;
    vector<Edge *> outputEdges;
    vector<Edge *> inputEdges;
    int idInFloor;

    bool isShortestSet = false;
    int shortestPath;
public:
    void addOutputEdge(Edge * e){
        this->outputEdges.push_back(e);
    }
    void addInputEdge(Edge * e){
        this->inputEdges.push_back(e);
    }

    const vector<Edge*> &getInputEdges(){
        return inputEdges;
    }
    const vector<Edge*> &getOutputEdges(){
        return outputEdges;
    }
    int getShortestPath(){
        return shortestPath;
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
    bool isShortestPathSet(){
        return isShortestSet;
    }
    void setShortestPath(int pathSize){
        this->isShortestSet = true;
        this->shortestPath = pathSize;
    }
    Vertex(int idInFloor, int startPos, int endPos, int floor){
        this->idInFloor = idInFloor;
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
int findShortestPath(Vertex * v, int endPointX){
    if(v->isShortestPathSet())
        return v->getShortestPath();
    vector<int> paths;
    if(v->getEndPos() == endPointX)
        return 0;
    for(int i = 0; i < v->getOutputEdges().size(); i ++){
        Edge * outputEdge = v->getOutputEdges().at(i);
        paths.push_back(findShortestPath(outputEdge->getOutputVertex(), endPointX) + outputEdge->getValue());
    }
    int minPath = paths.at(0);
    for(int i = 0; i < paths.size(); i ++){
        if(paths.at(i) < minPath)
            minPath = paths.at(i);
    }
    v->setShortestPath(minPath);
    return minPath;
}
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    string line;
    getline(cin, line);
    vector<string> args = split(line, ' ');
    int platforms = stoi(args[0]);
    int length = stoi(args[1]);
    int requestsCount = stoi(args[2]);

    vector<vector<Vertex *>> floors;
    vector<IntervalMap<Vertex * >*> intervalMapList;
    for(int i = 0; i < platforms; i ++){
        getline(cin, line);
        args = split(line, ' ');
        int holesCount = stoi(args[0]);
        int startPos = 0;
        floors.push_back(vector<Vertex *>());
        IntervalMap<Vertex * > * intervalMap = new IntervalMap<Vertex * >(length - 1);
        for(int j = 0; j < holesCount + 1; j++){//because there are n+1 platforms per level
            int endPos;
            if(j == holesCount){
                endPos = length - 1;
            }else{
                endPos = stoi(args[j+1]) - 2;
            }
            Vertex * v = new Vertex(j, startPos, endPos, i);
            floors.at(i).push_back(v);
            intervalMap->addValue(startPos, v);
            if(j != holesCount){
                Edge * e = new Edge(1);
                e->setInputVertex(v);
                v->addOutputEdge(e);
                startPos = stoi(args[j + 1]);
                intervalMap->addValue(endPos + 1, nullptr);
            }
            if(j != 0){
                Edge * prevEdge = floors.at(i).at(j - 1)->getOutputEdges().at(0); //there must be only one edge!
                prevEdge->setOutputVertex(v);
            }
        }
        intervalMapList.push_back(intervalMap);
    }
    //now insert all down and up outputEdges//
    for(int i = 0; i < floors.size() - 1; i ++){//we request from up to down. Thats why we cant iterate over the last as theres no down!
        vector<Vertex *> floor = floors.at(i);
        for(int j = 0; j < floor.size(); j ++){//do not include the last one. Theres no hole in the end
            Vertex * consideredVertex = floor.at(j);
            if(j != floor.size() - 1) {
                int holeAfterConsideredPos = consideredVertex->getEndPos() + 1;
                Vertex * vertexUnderHole = intervalMapList.at(i + 1)->getValue(holeAfterConsideredPos);
                Edge * upToDown = new Edge(0);
                upToDown->setInputVertex(consideredVertex);
                upToDown->setOutputVertex(vertexUnderHole);
                consideredVertex->addOutputEdge(upToDown);
                vertexUnderHole->addInputEdge(upToDown);
            }
            if(j != 0){
                int holeBeforeConsideredPos = consideredVertex->getStartPos() - 1;
                Vertex * vertexUnderHole = intervalMapList.at(i + 1)->getValue(holeBeforeConsideredPos);
                Edge * downToUp = new Edge(1);
                downToUp->setInputVertex(vertexUnderHole);
                downToUp->setOutputVertex(consideredVertex);
                vertexUnderHole->addOutputEdge(downToUp);
                consideredVertex->addInputEdge(downToUp);
            }
        }
    }
    for(int i = 0; i < requestsCount; i ++){
        getline(cin, line);
        int startFloor = stoi(split(line, ' ')[0]) - 1;
        Vertex * startingVertex = floors.at(startFloor).at(0);
        int shortest = findShortestPath(startingVertex, length - 1);
        cout<<shortest<<"\n";
    }
    return 0;
}