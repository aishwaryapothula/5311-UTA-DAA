#include<algorithm>
#include<iostream>
#include<stdio.h>
#include<vector>

using namespace std;

typedef pair<int, int> Pair;


class Graph{
public:
    vector<vector<Pair>> list;
    int cycleMinBegin;
    int cycleMinEnd;
    int cycleMin;
    int pathMin;

    int totalPaths = 0;

    void addEdge(int from, int to, int value);
    void findCycle();
    void findPaths();
    int getTotalSink();
    void traverse(vector<int> &cycle, vector<int> &visited);
    void traverseForPath(vector<int> &path);
    void reduceFlowInCycle(int from, int to, int value);
    void findMin(vector<int> cycle, int index);
    void findMinPath(vector<int> path, int index);
    int getEdgeValue(int from, int to);
    void printGraph();

    Graph(int size){
        list.resize(size);
    }
};

void Graph::addEdge(int from, int to, int value){
    list[from].push_back(make_pair(to, value));
}

int Graph::getEdgeValue(int from, int to){
    // printf("from:%d to:%d", from, to);
    for(pair<int, int> item:list[from]){
        if(item.first == to){
            return item.second;
        }
    }

    return -1;   
}

int Graph::getTotalSink(){
    int total = 0;

    for(int i=0; i<list.size(); i++){
        for(pair<int, int> item: list[i]){
            if(item.first == (list.size() - 1)){
                total = total + item.second;
            }
        }
    }

    return total;
}


bool findInList(int item, vector<int> &list){
    for(int i = 0; i < list.size(); i++){
        if(list[i] == item){
            return true;
        }
    }
    return false;
}

void printList(vector<int> list){
    for(int v: list){
        printf("%2d, ", v);
    }
    printf("\n");
}

void clean_up(vector<int> &list){
    if(findInList(list.back(), list)){
        int i = 0;
        for(int i = 0; i < list.size() - 1; i++){
            if(list[i] == list.back()){
                list.erase(list.begin(), list.begin() + i);
                return;
            }
        }
    }

    list.clear();
}

bool listContainsCycle(vector<int> cycle){
    for(int i=0; i<cycle.size() - 2; i++){
        if(cycle[i] == cycle.back()){
            return true;
        }
    }

    return false;
}


void Graph::traverse(vector<int> &cycle, vector<int> &visited){
    if(list[cycle.back()].size() == 0){
        return;
    }
    for(pair<int, int> item : list[cycle.back()]){
        if(item.second > 0){

            if(findInList(item.first, cycle)){
                cycle.push_back(item.first);
                visited.push_back(item.first);
                return;
            }


            cycle.push_back(item.first);
            visited.push_back(item.first);
            traverse(cycle, visited);
            
            if(listContainsCycle(cycle)){
                break;
            } else {
                cycle.pop_back();
            }
        }
    }
}


void Graph::reduceFlowInCycle(int from, int to, int value){
    int count = -1;
    for(pair<int, int> item: list[from]){
        count++;
        if(to == item.first){
            list[from][count] = make_pair(item.first, (item.second -  value));
            pair<int, int> new_item = list[from][count];
            // printf("reduced flow %d %d to %d \n", from, new_item.first, new_item.second);
            return;
        }
    }
}

void Graph::findMin(vector<int> cycle, int index){
    for(int i=0; i < cycle.size() - 1; i++){
        int newCycleMin = getEdgeValue(cycle[i], cycle[i+1]);
        if(newCycleMin <= cycleMin || cycleMin == -1){
            cycleMin = newCycleMin;
            cycleMinBegin = cycle[i];
            cycleMinEnd = cycle[i+1];
        }
    }
    
}

void Graph::findMinPath(vector<int> path, int index){
    for(int i=0; i < path.size() - 1; i++){
        int newPathMin = getEdgeValue(path[i], path[i+1]);
        if(newPathMin <= pathMin || pathMin == -1){
            pathMin = newPathMin;
        }
    }
}

void Graph::findCycle(){
    vector<int> cycles;
    vector<int> cycle;
    vector<int> visited;
    int cyclesFound;

    while(true){
        cyclesFound = 0;
        for(int i = 0; i < list.size() - 1; i++){
            // printf("begin traverse\n");
            cycleMin = -1;
            cycleMinBegin = 0;
            cycleMinEnd = 0;
            cycle.clear();
            cycle.push_back(i);
            traverse(cycle, visited);
            
            // printList(cycle);
            clean_up(cycle);

            if(cycle.size() > 2 && cycle.front() == cycle.back()){
                cyclesFound++;
                findMin(cycle, 0);
                printf("Cancel cycle of %d vertices, limiting edge %d->%d has flow %d\n", cycle.size() - 1, cycleMinBegin, cycleMinEnd, cycleMin);
                for(int v=0; v < cycle.size() - 1; v++){
                    reduceFlowInCycle(cycle[v], cycle[v+1], cycleMin);
                }
                break;
            }
        }

        if(cyclesFound == 0){
            break;
        }
    }    
}

vector<pair<int, int>> reduced_edges;

void Graph::traverseForPath(vector<int> &path){
    int lastVisited = path.back();

    if(lastVisited == list.size() - 1){
        pathMin = -1;
        findMinPath(path, 0);

        if(pathMin > 0){
            totalPaths++;
            printf("%d: ", totalPaths);
            for(int i = path.size() - 1; i >= 0; i--){            
                if(i < path.size() - 1 ){
                    if(getEdgeValue(path[i], path[i+1]) == pathMin){
                        printf("<=");
                    } else {
                        printf("<-");
                    }
                }

                printf("%d", path[i]);
            }

            for(int v=0; v < path.size() - 1; v++){
                reduceFlowInCycle(path[v], path[v+1], pathMin);
            }
            printf(" flow: %d\n", pathMin);
        }
        
        
        return;    
    }

    for(pair<int, int> item: list[lastVisited]){
        if(item.second > 0){
            path.push_back(item.first);

            traverseForPath(path);
            // printList(path);

            if(path.back() == list.size() - 1){
                path.pop_back();
            } else {
                path.pop_back();
            }
        }
    }
}


void Graph::findPaths(){
    vector<int> path;
    int pathsFound = 0;

    path.clear();
    path.push_back(0);
    traverseForPath(path);
}

// n = vertices
// m = edges
int n,m;

void Graph::printGraph(){
    for(int i = 0; i < list.size(); i++){
        printf("[%3d]: ", i);
        for(int j=0; j<list[i].size(); j++){
            pair<int, int> item = list[i][j];
            printf("%2d->[%3d], ", item.first, item.second);
        }
        printf("\n");
    }
    
}

int main(int argc, char const *argv[])
{
    cin >> n >> m;
    Graph graph(n);

    for(int i = 0; i < m; i++){
        int from, to, value;
        cin >> from >> to >> value;
        graph.addEdge(from, to, value);
    }
    

    int totalSink = graph.getTotalSink();
    printf("Verified flow is %d\n", totalSink);
    
    // graph.printGraph();
    graph.findCycle();
    printf("Flow decomposition: saturated edges <=, unsaturated edges <-\n");

    graph.findPaths();
    printf("Total flow in decomposition: %d\n\n", totalSink);
    return 0;
}
