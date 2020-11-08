#include <iostream>
#include <map>
#include <stack>
#include <vector>

using namespace std;
struct Vertex;
struct Range{
    int min;
    int max;
    Range(int min, int max){
        this->min = min;
        this->max = max;
    }
};
struct Vertex{
    vector<pair<int, bool>> edges;//the outputVertices looking from final vertex to the start point
    int id;
    int shortestPath = 2147483600;
    bool deleted = false;
    int inDegree = 0;
    Range * rangeInPlatform;
    int numInFloor;
    Vertex(int id, int numInFloor, Range * r){
        this->id = id;
        this->numInFloor = numInFloor;
        rangeInPlatform = new Range(r->min, r->max);
    }
};

void findShortestPath(vector<Vertex *> &vertices, int startPoint){
    stack<int> queue;
    Vertex * root = vertices[startPoint];
    root->shortestPath = 0;
    queue.push(root->id);
    while(!queue.empty()){
        Vertex * subject = vertices[queue.top()];
        queue.pop();
        for(int i = 0; i < subject->edges.size(); i ++){
            Vertex * neighbour = vertices[subject->edges[i].first];
            if(subject->shortestPath + subject->edges[i].second < neighbour->shortestPath){
                neighbour->shortestPath = subject->shortestPath + subject->edges[i].second;
                queue.push(neighbour->id);
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

    vector<Vertex*> vertices;
    vector<int> floorStartingVertex;

    int currID = 0;
    Range * r = new Range(0,0);

    Vertex * finalVertex = new Vertex(-1, -1, r);
    int predictedEdgeCount = 0;
    int edgeCount = 0;
    for(int i = 0; i < platformsCount; i++){
        r->min = 0;
        r->max = 0;
        getline(cin, line);
        args = split(line, ' ');
        int holesCount = stoi(args[0]);

        predictedEdgeCount += (i != platformsCount - 1) ? holesCount * 3 : holesCount;

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
            vertices.push_back(v);
            interval->pushBack(r, v);
            floor.push_back(v);
            currID++;
        }

        if( i > 0 ){
            for(int j = 0; j < floors[i - 1].size() - 1; j ++){
                Vertex * subjectVertex = floors[i - 1][j];//from 0 to platformsCount - 1;

                Vertex * sameLineVertex = floors[i - 1][j + 1];
                Vertex * underVertex = interval->getObject(subjectVertex->rangeInPlatform->max + 1);

                sameLineVertex->edges.emplace_back(underVertex->id, true);
                underVertex->edges.emplace_back(subjectVertex->id, false);

                delete subjectVertex->rangeInPlatform;
                if(j == floors[i - 1].size() - 2){
                    delete sameLineVertex->rangeInPlatform;
                }
            }
        }

        delete interval;
        floors.push_back(floor);
        floorStartingVertex.push_back(floors[i][0]->id);
        finalVertex->edges.emplace_back(floors[i][floors[i].size() - 1]->id, false);

        for(int j = 0; j < floors[i].size() - 1; j ++){
            Vertex * subjectVertex = floors[i][j];
            Vertex * sameLineVertex = floors[i][j + 1];
            sameLineVertex->edges.emplace_back(subjectVertex->id, true);
            edgeCount +=1;
        }
        if(i == platformsCount - 1){
            for(auto n : floors[i]){
                delete n->rangeInPlatform;
            }
        }
    }
    floors.clear();
    delete finalVertex->rangeInPlatform;

    finalVertex->id = vertices.size();
    vertices.push_back(finalVertex);

    findShortestPath(vertices, vertices.size() - 1);



    for(int i = 0; i < requestsCount; i ++){
        getline(cin, line);
        args = split(line, ' ');
        int platformNum = stoi(args[0]) - 1;
        cout<<vertices[floorStartingVertex[platformNum]]->shortestPath<<"\n";
    }

    for(auto v : vertices){
        delete v;
        //put that in topologicalSort function and delete as soon as you put in the order vector
    }





    delete r;



    cout<<"";
    return 0;
}