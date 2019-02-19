#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <ctime>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::pair;
using std::set;

namespace std { // make pair hashable
template <> struct hash<std::pair<int, int>> {
    inline size_t operator()(const std::pair<int, int> &v) const {
        std::hash<int> int_hasher;
        return int_hasher(v.first) ^ int_hasher(v.second);
    }
};
}

// clock_t Begin, End;

struct vertex{
    vertex(int self_num_){
        self_num = self_num_;
        vertex_list.insert(self_num);
    }
    vertex(const vertex& v1, const vertex& v2, int new_num){
        self_num = new_num;
        vertex_list.insert(v1.vertex_list.begin(),v1.vertex_list.end());
        vertex_list.insert(v2.vertex_list.begin(),v2.vertex_list.end());
    }
    vertex(){};
    int self_num;
    unordered_set<int> vertex_list;
};

struct Graph{

    Graph(std::ifstream& myfile){
        string line;

        if (myfile.is_open()){
            while( std::getline(myfile,line) ){
        // Begin = clock();
                std::stringstream stream(line);
                std::istream_iterator<string> begin(stream),end;
                vector<string> vstring(begin,end);

                unordered_set<int> neighbor;
                std::transform(vstring.begin()+1,vstring.end(),\
                               std::inserter(neighbor,neighbor.end()),\
                               [](const string& str){return std::stoi(str);});

                vertex v(std::stoi(vstring.front()));
                vertices.emplace_back(v);

                // for (auto i=vstring.begin()+1;i < vstring.end();++i){
                //     neighbor.emplace_back(std::stoi(*i));
                // } // this initialization is more explicit to understand, but in ~2/3 cases, it's slower!
        // End = clock();
        // cout << float(End-Begin)/CLOCKS_PER_SEC << endl;
                for (auto i:neighbor){
                    if (i > v.self_num){
                        edges.insert(pair<int,int>(v.self_num,i));
                    }
                }
            }
            myfile.close();
            InitializeMajiaEdges();
        }
    }
    Graph(){InitializeMajiaEdges();};

    vector<vertex> vertices,vertices2;
    set<pair<int,int>> edges,edges2;
    unordered_map<int,int> majia,majia2;
    vector<int> edge_history,edge_history2;

    void InitializeMajiaEdges(){
        for (int i=0; i<vertices.size(); ++i){
            majia[i+1] = i+1;
        }
        edge_history.emplace_back(edges.size());
    }

    void RebuildEdges(vertex& v1, vertex& v2){
        for (auto i:v1.vertex_list){
            for (auto j:v2.vertex_list){
                if (i < j){
                    edges.erase(pair<int,int>(i,j));
                }
                else if (i > j){
                    edges.erase(pair<int,int>(j,i));
                }
                else{
                    throw std::logic_error("Fatal Error!!! Duplicate elememts in different group of vertices!");
                }
            }
        }
    }

    void PrintEdges(){
        for (auto i:edges){
            cout << "<" << i.first << "," << i.second << ">" << endl;
        }
    }

    void Backup(){
        vertices2 = vertices;
        edges2 = edges;
        majia2 = majia;
        edge_history2 = edge_history;
    }

    void Recovery(){
        vertices = vertices2;
        edges = edges2;
        majia = majia2;
        edge_history = edge_history2;
    }

    int Merge(){
        int epoch = 0;
        srand (time(NULL));
        while (true){
            int random = rand()%edges.size();
            int first = majia[std::next(edges.begin(),random)->first]; // encode of vertex in vector is equal
            int second = majia[std::next(edges.begin(),random)->second];// to the one in real data minus 1.
            vertex v1 = vertices[first-1];//dont!!! use vertex& v1
            vertex v2 = vertices[second-1];
            vertex v_temp(v1,v2,vertices.size()+1);
            vertices.emplace_back(v_temp);
            RebuildEdges(v1,v2);
            if (edges.size()<=0){
                // cout << "Merging terminated!!!" << endl;
                return edge_history.back();
            }
            edge_history.emplace_back(edges.size());
            for (auto i:vertices.back().vertex_list){
                majia[i] = vertices.size();
            }
            ++epoch;
        }
        return -1;
    }
};

int main(){

    std::ifstream myfile("KargerMinCut.txt");
    Graph graph(myfile);
    graph.Backup();
    int size = graph.vertices.size();
    size = int(size*5);//*log(size)
    cout << "size:" << size << endl;
    vector<int> mincut_history;
    mincut_history.reserve(size);

    for (int i=0;i<size;++i){
        cout << float(i)/float(size) << endl;
        int temp = graph.Merge();
        // cout << temp << "," << endl;
        mincut_history.emplace_back(temp);
        graph.Recovery();
    }
    cout << "\n\n" << *std::min_element(mincut_history.begin(),mincut_history.end()) << endl;
    //Ans: 17

    return 0;

}
