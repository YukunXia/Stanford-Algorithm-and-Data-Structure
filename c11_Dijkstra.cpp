#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <queue>
#include <limits>
#include <stack>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::pair;

#define print(a) cout<<(a)<<endl;
#define be(v) v.begin(),v.end()

struct cmp_the_int{
    bool operator()(const pair<string,int>& p1, const pair<string,int>& p2){
        return p1.second > p2.second;
    }
};

struct Vertex{
    Vertex(){};
    Vertex(const string name_): name(name_) {}
    string name;
    std::unordered_map<string,int> next;
    std::priority_queue<pair<string,int>,vector<pair<string,int>>,cmp_the_int> previous;
};

struct Graph{

    Graph(){};
    Graph(std::ifstream& myfile){
        string line;
        if(myfile.is_open()){
            while(std::getline(myfile,line)){
                std::stringstream stream(line);
                std::istream_iterator<string> Begin(stream),End;
                vector<string> vstring(Begin,End);
                int weight = std::stoi(vstring[2]);
                string& s1 = vstring.at(0);
                string& s2 = vstring.at(1);

                if (vertices.find(s1) == vertices.end()){
                    Vertex v1(s1);
                    vertices[s1] = v1;
                }
                if (vertices.find(s2) == vertices.end()){
                    Vertex v2(s2);
                    vertices[s2] = v2;
                }
                vertices[s1].next[s2] = weight;
                vertices[s2].next[s1] = weight;
                
            }
        }
    }

    int Min_distance(const string& s1, const string& s2){
        for (auto& i:vertices){
            if (i.first != s1){
                dijkstra_heap.push(make_pair(i.first,std::numeric_limits<int>::max()));
            }
        }
        archived[s1] = 0;
        string s_now = s1;
        while(s_now != s2 && archived.size() < vertices.size()){
            for (auto v_next:vertices[s_now].next){
                string s_next = v_next.first;
                if (archived.find(s_next) == archived.end()){
                    int cache = archived[s_now]+v_next.second;
                    dijkstra_heap.push(make_pair(s_next,cache));
                    vertices[s_next].previous.push(make_pair(s_now,cache));
                }
            }
            s_now = dijkstra_heap.top().first;
            archived[s_now] = dijkstra_heap.top().second;
            dijkstra_heap.pop();
        }

        if (archived.find(s2) != archived.end()){
            return archived[s2];
        }
        else{
            return -1;
        }
    }

    void Path_recall(const string& s1, const string& s2){
        string s_guess = s2;
        std::stack<string> memory;
        memory.push(s_guess);
        while(s_guess != s1){
            if(vertices[s_guess].previous.empty()){
                break;
            }
            s_guess = vertices[s_guess].previous.top().first;
            memory.push(s_guess);
        }
        while(!memory.empty()){
            cout << memory.top() << "->";
            memory.pop();
        }
        print("\n");
    }

    void Learn_to_forget(){

        archived.clear();

        for (auto &i:vertices){
            std::priority_queue<pair<string,int>,vector<pair<string,int>>,cmp_the_int> q;
            i.second.previous = q;
        }
    }

    std::unordered_map<string,Vertex> vertices;
    std::unordered_map<string,int> archived;
    std::priority_queue<pair<string,int>,vector<pair<string,int>>,cmp_the_int> dijkstra_heap;
};

int main(){

    std::ifstream myfile("Dijkstra_graph.txt");

    Graph graph(myfile);

    for (auto i:{"A","B","C","D","E","F"}){
        for (auto j:{"A","B","C","D","E","F"}){
            cout << "i: " << i << ", j: " << j << ", distance = ";
            print(graph.Min_distance(i,j));
            graph.Path_recall(i,j);
            graph.Learn_to_forget();
        }
        cout << endl;
    }

    return 0;

}

/* original file:

A B 5
A D 9
A E 2
E F 3
F D 2
D C 3
B C 2

*/

/*

i: A, j: A, distance = 0
A->

i: A, j: B, distance = 5
A->B->

i: A, j: C, distance = 7
A->B->C->

i: A, j: D, distance = 7
A->E->F->D->

i: A, j: E, distance = 2
A->E->

i: A, j: F, distance = 5
A->E->F->


i: B, j: A, distance = 5
B->A->

i: B, j: B, distance = 0
B->

i: B, j: C, distance = 2
B->C->

i: B, j: D, distance = 5
B->C->D->

i: B, j: E, distance = 7
B->A->E->

i: B, j: F, distance = 7
B->C->D->F->


i: C, j: A, distance = 7
C->B->A->

i: C, j: B, distance = 2
C->B->

i: C, j: C, distance = 0
C->

i: C, j: D, distance = 3
C->D->

i: C, j: E, distance = 8
C->D->F->E->

i: C, j: F, distance = 5
C->D->F->


i: D, j: A, distance = 7
D->F->E->A->

i: D, j: B, distance = 5
D->C->B->

i: D, j: C, distance = 3
D->C->

i: D, j: D, distance = 0
D->

i: D, j: E, distance = 5
D->F->E->

i: D, j: F, distance = 2
D->F->


i: E, j: A, distance = 2
E->A->

i: E, j: B, distance = 5
C->B->

i: E, j: C, distance = 7
E->A->B->C->

i: E, j: D, distance = 5
E->F->D->

i: E, j: E, distance = 0
E->

i: E, j: F, distance = 3
E->F->


i: F, j: A, distance = 5
F->E->A->

i: F, j: B, distance = 7
F->D->C->B->

i: F, j: C, distance = 5
F->D->C->

i: F, j: D, distance = 2
F->D->

i: F, j: E, distance = 3
F->E->

i: F, j: F, distance = 0
F->

*/