#include <iostream>
#include <math.h>
#include <memory>
#include <map>
#include <stack>
#include <vector>

using namespace std;
struct Vertex;
struct Edge;
struct Range{
    int min;
    int max;
    Range(int min, int max){
        this->min = min;
        this->max = max;
    }
};
struct Vertex{
    vector<Edge *> inputEdges;
    vector<Edge *> outputEdges;
    int id;
    int numInFloor;
    int shortestPath = 2147483600;
    bool visited = false;
    Range * rangeInPlatform;
    Vertex(int id, int numInFloor, Range * r){
        this->id = id;
        this->numInFloor = numInFloor;
        rangeInPlatform = new Range(r->min, r->max);
    }
    ~Vertex(){
        delete rangeInPlatform;
    }
};
struct Edge{
    Vertex * input;
    Vertex * output;
    bool weight;
    Edge(Vertex * i, Vertex * o, bool w){
        this->input = i;
        this->output = o;
        this->weight = w;
    }

};
vector<Vertex*> topologicalSort(vector<vector<Vertex*>> &floors){
    vector<Vertex*> order;

    return order;
}
void findShortestPaths(const vector<Vertex *> &vertices){
    return;
    vertices[0]->shortestPath = 0;
    for(auto v : vertices){
        for(auto e : v->inputEdges){
            if(v->shortestPath + e->weight < e->input->shortestPath){
                e->input->shortestPath = v->shortestPath + e->weight;
            }
        }
    }
}
template <typename T>
struct IntervalMap{
    vector<pair<Range, T* >> interval;
    void pushBack(Range * r, T* o){
        interval.push_back(make_pair(Range(r->min, r->max), o));
    }
    T* getObject(const int point){
        int min = 0;
        int max = interval.size();

        ;//points to index in vector!
        int pointer;
        while(true){
            pointer = (max - min) / 2 + min;
            Range r = interval[pointer].first;
            if(r.min <= point && r.max >= point)
                return interval[pointer].second;

            if(point > r.max){
                min = pointer;
            }
            if(point < r.min){
                max = pointer;
            }
        }
    }
    ~IntervalMap(){
        interval.clear();
        for(auto p : interval){
            delete p.second;
        }
    }
};

vector<string> split(string str, char divider){
    vector<string> result;

    string currWord;
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
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    string line;
    getline(cin, line);
    vector<string> args = split(line, ' ');
    int platformsCount = stoi(args[0]);
    int maxX = stoi(args[1]) - 1;
    int requestsCount = stoi(args[2]);

    vector<vector<Vertex* >> floors;
    vector<Vertex *> floorStartingVertices;

    vector<IntervalMap<Vertex>* > intervals;
    vector<Edge *> edges;

    int currID = 0;
    Range * r = new Range(0,0);
    for(int i = 0; i < platformsCount; i++){
        r->min = 0;
        r->max = 0;
        getline(cin, line);
        args = split(line, ' ');
        int holesCount = stoi(args[0]);

        IntervalMap<Vertex> * interval = new IntervalMap<Vertex>();
        vector<Vertex *> floor;
        for(int j = 0; j < holesCount + 1; j++){
            if(j == 0 && j != holesCount){
                r->max = stoi(args[j + 1]) - 2;
            }else if(j != holesCount){
                r->min = r->max + 2;
                r->max = stoi(args[j + 1]) - 2;;
            }else{
                r->min = r->max + 2;
                r->max = maxX;
            }

            Vertex * v = new Vertex(currID, j, r);
            currID ++;
            interval->pushBack(r, v);
            floor.push_back(v);
            if(j == 0){
                floorStartingVertices.push_back(v);
            }
        }
        floors.push_back(floor);
        intervals.push_back(interval);
    }

    for(int i = 0; i < floors.size(); i++){
        for(int j = 0; j < floors[i].size() - 1; j ++){

            Vertex * subjectVertex = floors[i][j];
            Vertex * nextVertex = intervals[i]->getObject(subjectVertex->rangeInPlatform->max + 2);

            Edge * sameLevel = new Edge(subjectVertex, nextVertex, true);
            edges.push_back(sameLevel);
            subjectVertex->outputEdges.push_back(sameLevel);
            nextVertex->inputEdges.push_back(sameLevel);

            if( i > floors.size() - 1 ){
                Vertex * underVertex = intervals[i + 1]->getObject(subjectVertex->rangeInPlatform->max + 1);
                Edge * downToUp = new Edge(underVertex, nextVertex, true);
                edges.push_back(downToUp);
                underVertex->outputEdges.push_back(downToUp);
                nextVertex->inputEdges.push_back(downToUp);

                Edge * upToDown = new Edge(subjectVertex, underVertex, false);
                edges.push_back(upToDown);
                subjectVertex->outputEdges.push_back(upToDown);
                underVertex->inputEdges.push_back(upToDown);
            }

        }
    }

    for(auto i : intervals){
        delete i;
    }

    vector<Vertex *> ordered = topologicalSort(floors);
    findShortestPaths(ordered);


    for(int i = 0; i < requestsCount; i ++){
        getline(cin, line);
        args = split(line, ' ');
        int platformCount = stoi(args[0]) - 1;
        cout<<floorStartingVertices[platformCount]->shortestPath<<"\n";
    }
    for(auto v : floors){
        for(auto f : v){
            delete f;
        }
    }
    for(auto v : ordered){
        delete v;
    }
    delete r;
    for(auto e : edges){
        delete e;
    }

    cout<<"";
    return 0;
}