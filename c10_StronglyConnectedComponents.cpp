#include <iostream>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <stack>
#include <random>

#define print(a) cout<<a<<endl;

using std::cout;
using std::endl;
using std::vector;
using std::string;

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
    Vertex(const int num_, const vector<int> tail_){
        num = num_;
        std::copy(tail_.begin(),tail_.end(),std::inserter(tail,tail.end()));
    }
    Vertex(){};

    void update_iter(){
        iter_head = head.begin();
        iter_tail = tail.begin();
    }

    int num;
    std::unordered_set<int> head;
    std::unordered_set<int>::iterator iter_head;
    std::unordered_set<int> tail;
    std::unordered_set<int>::iterator iter_tail;
};

struct Graph{

    Graph(){};
    Graph(std::ifstream& myfile){
        string line;
        if(myfile.is_open()){
            while(std::getline(myfile,line)){
                std::stringstream stream(line);
                std::istream_iterator<string> begin(stream),end;
                vector<string> vstring(begin,end);

                vector<int> tail;
                int num;

                std::transform(vstring.begin()+1,vstring.end(),\
                               std::back_inserter(tail),\
                               [](const string& str){return std::stoi(str);});
                num = std::stoi(*vstring.begin());
                // print(num);

                Vertex v(num,tail);
                vertices[num] = v;
                unsearched.emplace_back(num);
            }
        }

        for (auto m:vertices){
            for (auto t:m.second.tail){
                vertices[t].head.insert(m.first);
            }
        }

        for (auto &m:vertices){
            m.second.update_iter();
        }

    }

    void Print_all(){
        for (auto m:vertices){
            cout << "number: ";
            print(m.first);
            cout << "heads: ";
            for (auto h:m.second.head){
                cout << h << ", ";
            }
            print(" ");
            cout << "tails: ";
            for(auto t:m.second.tail){
                cout << t << ", ";
            }
            print("\n");
        }

        print("unsearched: \n");
        for(auto i:unsearched){
            cout << i << ", ";
        }
        print("\n");
    }

    int Random_unsearched(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0,unsearched.size()-1);
        return dis(gen);
    }

    void Search_update(int start){
        searched.insert(start);
        unsearched.erase(std::find(unsearched.begin(),unsearched.end(),start));
    }

    void DFS_initial_direction(){
        std::stack<int> temp_stack;

        while (searched.size() < vertices.size()){
            int start = unsearched[Random_unsearched()];
            Search_update(start);
            temp_stack.push(start);
            while(temp_stack.size() > 0){
                if(vertices[start].iter_tail != vertices[start].tail.end()){
                    int next = *vertices[start].iter_tail;
                    ++vertices[start].iter_tail;
                    if (searched.find(next) == searched.end()){
                        temp_stack.push(next);
                        start = next;
                        Search_update(start);
                    }
                    else{//start to pop
                        while(temp_stack.size() > 0){
                            if(vertices[temp_stack.top()].iter_tail != vertices[temp_stack.top()].tail.end()){
                                start = temp_stack.top();
                                break;
                            }
                            sequential.push(temp_stack.top());
                            temp_stack.pop();
                        }
                    }
                }
                else{//start to pop
                    while(temp_stack.size() > 0){
                        if(vertices[temp_stack.top()].iter_tail != vertices[temp_stack.top()].tail.end()){
                            start = temp_stack.top();
                            break;
                        }
                        sequential.push(temp_stack.top());
                        temp_stack.pop();
                    }
                }
            }
        }
    }

    void DFS_reverse_direction(){
        std::stack<int> temp_stack;
        std::vector<int> temp_group;
        searched.erase(searched.begin(),searched.end());
        int count = 0;
        while(sequential.size()>0){
            int start = sequential.top();
            sequential.pop();
            if (searched.find(start) != searched.end()){
                continue;
            }
            searched.insert(start);
            temp_stack.push(start);
            while(temp_stack.size()>0){
                if (vertices[start].iter_head != vertices[start].head.end()){
                    int next = *vertices[start].iter_head;
                    ++vertices[start].iter_head;
                    if (searched.find(next) != searched.end()){
                        while(temp_stack.size()>0){
                            if (vertices[temp_stack.top()].iter_head != vertices[temp_stack.top()].head.end()) {
                                start = temp_stack.top();
                                break;
                            }
                            temp_group.emplace_back(temp_stack.top());
                            temp_stack.pop();
                        }
                    }
                    else{
                        temp_stack.push(next);
                        searched.insert(next);
                        start = next;
                    }
                }
                else{
                    while(temp_stack.size()>0){
                        if (vertices[temp_stack.top()].iter_head != vertices[temp_stack.top()].head.end()) {
                            start = temp_stack.top();
                            break;
                        }
                        temp_group.emplace_back(temp_stack.top());
                        temp_stack.pop();
                    }
                }
            }
            ++count;
            cout << "NO." << count << " group of strongly connected conponents:\n";
            for(auto i:temp_group){
                cout << i << ", ";
            }
            print("\n");
            temp_group.erase(temp_group.begin(),temp_group.end());
        }
    }

    std::unordered_map<int,Vertex> vertices;
    vector<int> unsearched;
    std::unordered_set<int> searched;
    iterable_stack<int> sequential;

};

int main(){

    std::ifstream myfile("graphSCC.txt");
    Graph graph(myfile);

    // graph.Print_all();

    // print("========\n");
    
    graph.DFS_initial_direction();

    // print("=========\n");
    
    print("result of the first search: ")
    for(auto i:graph.sequential){
        cout << i << ", ";
    }
    print("\n");

    print("=========\n");

    graph.DFS_reverse_direction();

    return 0;

}

/* one of the possible output:

result of the first search:
10, 4, 3, 5, 6, 9, 8, 7, 2, 1, 0,

=========

NO.1 group of strongly connected conponents:
1, 2, 0,

NO.2 group of strongly connected conponents:
8, 9, 6, 7,

NO.3 group of strongly connected conponents:
3, 4, 5,

NO.4 group of strongly connected conponents:
10,
*/