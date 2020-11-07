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
    vector<pair<Range *, T* >> interval;
    void pushBack(Range * r, T* o){
        interval.push_back(make_pair(new Range(r->min, r->max), o));
    }
    T* getObject(const int point){
        int min = 0;
        int max = interval.size();

        ;//points to index in vector!
        int pointer;
        while(true){
            pointer = (max - min) / 2 + min;
            Range * r = interval[pointer].first;
            if(r->min <= point && r->max >= point)
                return interval[pointer].second;

            if(point > r->max){
                min = pointer;
            }
            if(point < r->min){
                max = pointer;
            }
        }
    }
    ~IntervalMap(){
        for(auto p : interval){
            delete p.first;
        }
        interval.clear();
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

    vector<Edge *> edges;


    int currID = 0;
    Range * r = new Range(0,0);

    Vertex * finalVertex = new Vertex(-1, -1, r);

    IntervalMap<Vertex> * prevInterval;

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

            if(i > 0){
                prevInterval->getObject(v->rangeInPlatform->max + 1);
                cout<<"";
            }
        }

        if(i > 0){
            delete prevInterval;
        }
        if(i != platformsCount - 1) {
            prevInterval = interval;
        }else{
            delete interval;
        }
        floors.push_back(floor);
    }
    vector<Vertex *> ordered = topologicalSort(floors);
    for(auto v : floors){
        int i = 0;
        for(auto f : v){
            if(i != 0)
                delete f;
            i++;
        }
    }
    findShortestPaths(ordered);

    for(auto v : ordered){
        delete v;
    }
    for(auto e : edges){
        delete e;
    }
    delete r;
    delete finalVertex;

    for(int i = 0; i < requestsCount; i ++){
        getline(cin, line);
        args = split(line, ' ');
        int platformCount = stoi(args[0]) - 1;
        cout<<floorStartingVertices[platformCount]->shortestPath<<"\n";
    }

    for(auto n : floorStartingVertices){
        delete n;
    }


    cout<<"";
    return 0;
}