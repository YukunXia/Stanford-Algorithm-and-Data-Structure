#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>

using std::cout;
using std::endl;
using std::string;
using std::vector;

//##########################use c++ 14###################################
template<typename T, typename Container = std::deque<T>>
class iterable_stack
: public std::stack<T, Container>
{
    using std::stack<T, Container>::c;

public:

    // expose just the iterators of the underlying container
    auto begin() { return std::begin(c); }
    auto end() { return std::end(c); }

    auto begin() const { return std::begin(c); }
    auto end() const { return std::end(c); }
};
//######################################################################

struct Vertex{
    Vertex(){};
    Vertex(const int num_, const vector<int> next_){
        num = num_;
        std::copy(next_.begin(),next_.end(),std::back_inserter(next));
    }
    int num;
    vector<int> next = {-1};
    int out_index = 1;
};

struct Graph{

    Graph(){};

    Graph(std::ifstream& my_file){
        string line;
        if (my_file.is_open()){
            while(std::getline(my_file,line)){
                std::stringstream stream(line);
                std::istream_iterator<string> begin(stream),end;
                vector<string> vstring(begin,end);

                vector<int> next;
                std::transform(vstring.begin()+1,vstring.end(),std::inserter(next,next.end()),\
                              [](const string& str){return std::stoi(str);});
                int num = std::stoi(vstring[0]);
                no_in_degree.insert(num);
                Vertex v(num,next);
                vertices[num] = v;
            }
        }
        for (auto v:vertices){
            for (auto i:v.second.next){
                no_in_degree.erase(i);
            }
        }
    }

    void print_all(){
        cout << "graph structure:" << endl;
        for (auto v:this->vertices){
            cout << v.first << ": ";
            std::copy(v.second.next.begin()+v.second.out_index,v.second.next.end(),std::ostream_iterator<int>(cout," "));
            cout << endl;
        }
        cout << "------------\nno in degree vertex:" << endl;
        for (auto v:this->no_in_degree){
            cout << v << ", ";
        }
        cout << endl;
        cout << "------------" << endl;
    }

    void DFS(){

        for (auto i:no_in_degree){
            iterable_stack<int> stack;
            stack.push(i);
            cout << "\n---------------\ni: " << i << endl;
            while(!stack.empty()){

                if (vertices[stack.top()].next.size() > vertices[stack.top()].out_index){
                    ++vertices[stack.top()].out_index;
                    stack.push(vertices[stack.top()].next[vertices[stack.top()].out_index-1]);
                }
                else{

                    cout << endl;
                    for(auto j:stack){
                        cout << j << "->";
                    }
                    cout << endl;
                    

                    while(vertices[stack.top()].out_index >= vertices[stack.top()].next.size()){
                        stack.pop();
                        if (stack.empty()){
                            break;
                        }
                    }
                }
            }
            clear_index();
            cout << endl;
        }
    }

    void clear_index(){
        for (auto &v:this->vertices){
            v.second.out_index = 1;
        }
    }
    Vertex void_vertex;
    std::unordered_set<int> no_in_degree;
    std::unordered_map<int,Vertex> vertices;
};


int main(){

    std::ifstream my_file("KargerMinCut2.txt");
    Graph graph(my_file);

    // graph.print_all();
    graph.DFS();

    return 0;

}

/* output

graph structure:
21:
20: 21
19: 15
18:
17: 13 20
16: 17
15: 16
14:
13: 14 18
12:
10: 6 7 11
9: 5 15
8:
7: 8
6:
3: 4
2: 3
1: 2 10
5:
11: 12 13
0: 1 5
------------
no in degree vertex:
19, 9, 0,
------------

---------------
i: 19

19->15->16->17->13->14->

19->15->16->17->13->18->

19->15->16->17->20->21->


---------------
i: 9

9->5->

9->15->16->17->13->14->

9->15->16->17->13->18->

9->15->16->17->20->21->


---------------
i: 0

0->1->2->3->4->

0->1->10->6->

0->1->10->7->8->

0->1->10->11->12->

0->1->10->11->13->14->

0->1->10->11->13->18->

0->5->

*/