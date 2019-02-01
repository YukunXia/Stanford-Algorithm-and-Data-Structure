#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using std::cout;
using std::endl;
using std::cin;
using std::vector;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::queue;

struct Vertex{
    Vertex(const int num_, const vector<int> neighbor_): num{num_}, neighbor{neighbor_} {}
    Vertex(){};
    int num;
    vector<int> neighbor;
};

struct Graph{
    Graph(std::ifstream &myfile){
        string line;
        if (myfile.is_open()){
            while(std::getline(myfile,line)){
                std::stringstream stream(line);
                std::istream_iterator<string> begin(stream),end;
                vector<string> vstring(begin,end);

                vector<int> neighbor;
                std::transform(vstring.begin()+1,vstring.end(),std::inserter(neighbor,neighbor.end()),\
                              [](const string& str){return std::stoi(str);});
                int num = std::stoi(*vstring.begin());
                Vertex v(num,neighbor);
                vertices[num] = v;
            }
        }
        myfile.close();
    }
    Graph(){};

    int distance(){
        obselete_vertices.clear();
        path_log.clear();

        queue<int> q;
        q.push(n1);
        obselete_vertices.insert(n1);
        unordered_map<int,vector<int>> layers;
        vector<int> s = {n1};
        layers[1] = s;
        int count = 1;
        vector<int> temp;

        while(q.size()>0){
            for (auto i:vertices.at(q.front()).neighbor){
                if (obselete_vertices.count(i)==0 & i!=n2){
                    path_log[i] = q.front();
                    q.push(i);
                    obselete_vertices.insert(i);
                    temp.emplace_back(i);
                }
                else if (i == n2){
                    path_log[i] = q.front();
                    return count;
                }
            }
            if (layers[count].back() == q.front()){
                ++count;
                std::copy(temp.begin(),temp.end(),std::back_inserter(layers[count]));
                temp.clear();
            }
            q.pop();
        }
        return -1;
    }

    void path_print(){
        int n_temp = n2;
        cout << n2 << " <= ";
        while (path_log[n_temp] != n1){
            cout << path_log[n_temp] << " <= ";
            n_temp = path_log[n_temp];
        }
        cout << n1 << endl;
    }

    int n1,n2;
    unordered_map<int,Vertex> vertices;
    unordered_set<int> obselete_vertices;
    unordered_map<int,int> path_log;
};

int main(){

    std::ifstream myfile("KargerMinCut.txt");
    Graph graph(myfile);
    cout << "Please input two numbers for two nodes:" << endl;
    cin >> graph.n1 >> graph.n2;
    cout << "Distance between " << graph.n1 << " and " << graph.n2 << " is: " << graph.distance() << endl;
    graph.path_print();
    return 0;

}