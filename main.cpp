#include <iostream>

#include <memory>
#include <stack>
#include <map>
#include <vector>
#include <memory>
#include <fstream>

using namespace std;
struct Vertex;
struct Edge;
struct Vertex{
    vector<shared_ptr<Edge>> edges;
    int id;
    int numInFloor;
    Vertex(int id, int numInFloor){
        this->id = id;
        this->numInFloor = numInFloor;
    }
};
struct Edge{
    shared_ptr<Edge> input;
    shared_ptr<Edge> output;

};

struct Range{
    int min;
    int max;
    Range(int min, int max){
        this->min = min;
        this->max = max;
    }
};

template <typename T>
struct IntervalMap{
    vector<pair<Range, shared_ptr<T> >> interval;
    void pushBack(Range r, shared_ptr<T> o){
        interval.push_back(make_pair(Range(r.min, r.max), o));
    }
    shared_ptr<T> getObject(int point){
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

    Range r(0, 0);
    int currID = 0;
    for(int i = 0; i < platformsCount; i++){
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

            shared_ptr<Vertex> v(new Vertex(currID, j));
            currID ++;

            interval.pushBack(r, v);
            floor.push_back(v);
        }
        floors.push_back(floor);
        intervals.push_back(interval);
    }
    shared_ptr<Vertex> v = intervals[0].getObject(5);

    return 0;
}