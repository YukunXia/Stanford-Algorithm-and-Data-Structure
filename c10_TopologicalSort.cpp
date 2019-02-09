#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <random>

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

    int DFS(int i){
        
        int topo_starting_point = topological_order.size();

        iterable_stack<int> stack;
        stack.push(i);
        // cout << "\n---------------\ni: " << i << endl;
        while(!stack.empty()){

            if (vertices[stack.top()].next.size() > vertices[stack.top()].out_index){
                ++vertices[stack.top()].out_index;
                // cout << "stack next: " << vertices[stack.top()].next[vertices[stack.top()].out_index-1];
                auto index = searched.find(vertices[stack.top()].next[vertices[stack.top()].out_index-1]);
                if (index == searched.end()){
                    // cout << " is not in seached" << endl;
                    stack.push(vertices[stack.top()].next[vertices[stack.top()].out_index-1]);
                }
                // else{
                //     cout << " is in seached" << endl;
                // }

            }
            else{

                // cout << endl;
                // for(auto j:stack){
                //     cout << j << "->";
                // }
                // cout << endl;
                

                while(vertices[stack.top()].out_index >= vertices[stack.top()].next.size()){
                    // cout << "top: " << stack.top() << ", num:" << vertices[stack.top()].num << endl;
                    topological_order.emplace_back(vertices[stack.top()].num);
                    searched.insert(vertices[stack.top()].num);
                    stack.pop();
                    if (stack.empty()){
                        break;
                    }
                }
            }
        }
        // clear_index();

        return topological_order.size() - topo_starting_point;
    }

    void topological_sort(){
        for (auto v:vertices){
            unsearched.emplace_back(v.first);
        }
        while(unsearched.size() > 0){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0,unsearched.size()-1);
            int start = dis(gen);
            // cout << "start: " << unsearched.at(start) << endl;
            int topo_size = DFS(unsearched.at(start));
            for (vector<int>::iterator i = topological_order.end()-topo_size ; i < topological_order.end(); ++i){
                auto index = std::find(unsearched.begin(),unsearched.end(),*i);
                if(index!=unsearched.end()){
                    unsearched.erase(index);
                }
            }
            // std::copy(topological_order.begin(),topological_order.end(),\
            //         std::ostream_iterator<int>(cout," <- "));
            // cout << "\nunsearched\n";
            // std::copy(unsearched.begin(),unsearched.end(),\
            //         std::ostream_iterator<int>(cout," "));
            // cout << "\nsearched\n";
            // std::copy(searched.begin(),searched.end(),\
            //         std::ostream_iterator<int>(cout," "));
            // cout << "\n============\n" << endl;
        }

    }

    void clear_index(){
        for (auto &v:vertices){
            v.second.out_index = 1;
        }
    }

    Vertex void_vertex;
    std::unordered_set<int> no_in_degree;
    std::unordered_map<int,Vertex> vertices;
    std::vector<int> topological_order;
    std::vector<int> unsearched;
    std::unordered_set<int> searched;
};


int main(){

    std::ifstream my_file("Graph2.txt");
    Graph graph(my_file);

    graph.topological_sort();

    // cout << (graph.topological_order.size() == graph.vertices.size()) << endl;

    std::copy(graph.topological_order.begin(),graph.topological_order.end(),\
            std::ostream_iterator<int>(cout," <- "));
    cout << endl;
    
    return 0;

}

/* output

https://images0.cnblogs.com/blog/413416/201308/02210011-935455e3d5b84bf492b11fa1e660937a.png

*/