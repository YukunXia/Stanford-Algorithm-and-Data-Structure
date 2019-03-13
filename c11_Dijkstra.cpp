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

#define print(a) cout<<a<<endl;
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
                // std::stringstream stream(line);
                // std::istream_iterator<string> Begin(stream),End;
                // vector<string> vstring(Begin,End);
                // int weight = std::stoi(vstring[2]);
                // string& s1 = vstring.at(0);
                // string& s2 = vstring.at(1);

                // if (vertices.find(s1) == vertices.end()){
                //     Vertex v1(s1);
                //     vertices[s1] = v1;
                // }
                // if (vertices.find(s2) == vertices.end()){
                //     Vertex v2(s2);
                //     vertices[s2] = v2;
                // }
                // vertices[s1].next[s2] = weight;
                // vertices[s2].next[s1] = weight;


                std::stringstream stream(line);
                std::istream_iterator<string> Begin(stream),End;
                vector<string> vstring(Begin,End);
                string s1 = vstring.at(0);
                for (int i=1; i<vstring.size();++i){
                    std::istringstream ss(vstring.at(i));
                    string s2,string_for_weight;
                    std::getline(ss,s2,',');
                    std::getline(ss,string_for_weight);
                    // print(s2<<"\n"<<string_for_weight)
                    int weight = std::stoi(string_for_weight);

                    if (vertices.find(s1) == vertices.end()){
                        Vertex v1(s1);
                        vertices[s1] = v1;
                    }
                    vertices[s1].next[s2] = weight;
                }
                
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

    // std::ifstream myfile("Dijkstra_graph.txt");
    std::ifstream myfile("dijkstraData.txt");

    Graph graph(myfile);
 
    // for (auto i:{"A","B","C","D","E","F"}){
    //     for (auto j:{"A","B","C","D","E","F"}){
    //         cout << "i: " << i << ", j: " << j << ", distance = ";
    //         print(graph.Min_distance(i,j));
    //         graph.Path_recall(i,j);
    //         graph.Learn_to_forget();
    //     }
    //     cout << endl;
    // }

    string i="1";
    for (auto j:{"7","37","59","82","99","115","133","165","188","197"}){
        print(graph.Min_distance(i,j));
        graph.Path_recall(i,j);
        graph.Learn_to_forget();
    }

    return 0;

}

/*


Programming Assignment 5
10/10 points (graded)
In this programming problem you'll code up Dijkstra's shortest-path algorithm.

Download the following text file (Right click and select "Save As..."): dijkstraData.txt

The file contains an adjacency list representation of an undirected weighted graph with 200 vertices labeled 1 to 200. Each row consists of the node tuples that are adjacent to that particular vertex along with the length of that edge. For example, the 6th row has 6 as the first entry indicating that this row corresponds to the vertex labeled 6. The next entry of this row "141,8200" indicates that there is an edge between vertex 6 and vertex 141 that has length 8200. The rest of the pairs of this row indicate the other vertices adjacent to vertex 6 and the lengths of the corresponding edges.

Your task is to run Dijkstra's shortest-path algorithm on this graph, using 1 (the first vertex) as the source vertex, and to compute the shortest-path distances between 1 and every other vertex of the graph. If there is no path between a vertex  and vertex 1, we'll define the shortest-path distance between 1 and  to be 1000000.

You should report the shortest-path distances to the following ten vertices, in order: 7,37,59,82,99,115,133,165,188,197. Enter the shortest-path distances using the fields below for each of the vertices.

IMPLEMENTATION NOTES: This graph is small enough that the straightforward  time implementation of Dijkstra's algorithm should work fine. OPTIONAL: For those of you seeking an additional challenge, try implementing the heap-based version. Note this requires a heap that supports deletions, and you'll probably need to maintain some kind of mapping between vertices and their positions in the heap.




Answer:

2599
1->114->129->85->53->7->

2610
1->145->108->126->155->37->

2947
1->92->194->162->59->

2052
1->92->134->135->82->

2367
1->99->

2399
1->80->115->

2029
1->114->129->85->133->

2442
1->80->19->187->165->

2505
1->92->70->9->72->157->26->95->196->188->

3068
1->114->103->110->197->

*/

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