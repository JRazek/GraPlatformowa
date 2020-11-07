#include <iostream>
#include <math.h>
#include <memory>
#include <map>
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
    vector<shared_ptr<Edge>> inputEdges;
    vector<shared_ptr<Edge>> outputEdges;
    int id;
    int numInFloor;
    int shortestPath = 2147483647;
    unique_ptr<Range> rangeInPlatform;
    Vertex(int id, int numInFloor, Range r){
        this->id = id;
        this->numInFloor = numInFloor;
        rangeInPlatform = make_unique<Range>(r.min, r.max);
    }
    ~Vertex(){
        cout<<"goodbye!";
    }
};
struct Edge{
    weak_ptr<Vertex> input;
    weak_ptr<Vertex> output;
    Edge(weak_ptr<Vertex> i, weak_ptr<Vertex> o, bool w){
        this->input = i;
        this->output = o;
        this->weight = w;
    }
    bool weight;
};


template <typename T>
struct IntervalMap{
    vector<pair<Range, shared_ptr<T> >> interval;
    void pushBack(Range r, shared_ptr<T> o){
        interval.push_back(make_pair(Range(r.min, r.max), o));
    }
    shared_ptr<T> getObject(const int point){
        int min = 0;
        int max = interval.size();

        ;//points to index in vector!

        while(true){
            int pointer = (max - min) / 2 + min;
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

    vector<vector<shared_ptr<Vertex> >> floors;
    vector<IntervalMap<Vertex> > intervals;

    int currID = 0;
    for(int i = 0; i < platformsCount; i++){
        Range r(0, 0);
        getline(cin, line);
        args = split(line, ' ');
        int holesCount = stoi(args[0]);

        IntervalMap<Vertex> interval;
        vector<shared_ptr<Vertex>> floor;
        for(int j = 0; j < holesCount + 1; j++){
            if(j == 0){
                r.max = stoi(args[j + 1]) - 2;
            }else if(j != holesCount){
                r.min = r.max + 2;
                r.max = stoi(args[j + 1]) - 2;;
            }else{
                r.min = r.max + 2;
                r.max = maxX;
            }

            shared_ptr<Vertex> v = make_shared<Vertex>(currID, j, r);
            currID ++;

            interval.pushBack(r, v);
            floor.push_back(v);
        }
        floors.push_back(floor);
        intervals.push_back(interval);
    }

    for(int i = 0; i < floors.size(); i++){
        vector<shared_ptr<Vertex>> floor = floors[i];
        for(int j = 0; j < floor.size() - 1; j ++){
            shared_ptr<Vertex> vertexSubject = floor[j];
            shared_ptr<Vertex> vertexNext = floor[j + 1];

            if(i != floors.size() - 1){
                shared_ptr<Vertex> vertexUnder = intervals[i + 1].getObject(vertexSubject->rangeInPlatform->max + 1);
                shared_ptr<Edge> upToDown = make_shared<Edge>(vertexSubject, vertexUnder, false);
                shared_ptr<Edge> downToUp = make_shared<Edge>(vertexUnder, vertexNext, true);

                vertexSubject->outputEdges.push_back(upToDown);
                vertexUnder->inputEdges.push_back(upToDown);

                vertexUnder->outputEdges.push_back(downToUp);
                vertexNext->inputEdges.push_back(downToUp);
            }

            shared_ptr<Edge> edge = make_shared<Edge>(vertexSubject, vertexNext, true);
            vertexSubject->outputEdges.push_back(edge);
            vertexNext->inputEdges.push_back(edge);
        }
    }

    return 0;
}