#include <iostream>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <stack>
#include <random>
#include <set>

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
            /* for txt format like: 
            1 2 3 4
            2 4 5
            3 2 6
            */
            // while(std::getline(myfile,line)){
            //     std::stringstream stream(line);
            //     std::istream_iterator<string> begin(stream),end;
            //     vector<string> vstring(begin,end);

            //     vector<int> tail;
            //     int num;

            //     std::transform(vstring.begin()+1,vstring.end(),\
            //                    std::back_inserter(tail),\
            //                    [](const string& str){return std::stoi(str);});
            //     num = std::stoi(*vstring.begin());
            //     // print(num);

            //     Vertex v(num,tail);
            //     vertices[num] = v;
            //     unsearched.emplace_back(num);
            // }

            //=========================================================================================

            /* for txt format like:
            1 2
            1 3
            1 4
            2 4
            2 5
            3 2
            3 6
            */
        //    int count = 0;
            while(std::getline(myfile,line)){
                // ++count;
                std::stringstream stream(line);
                std::istream_iterator<string> Begin(stream),End;
                vector<string> vstring(Begin,End);

                int num = std::stoi(vstring.front());
                int next = std::stoi(vstring.back());
                if (vertices.find(num) == vertices.end()){
                    Vertex v;
                    vertices[num] = v;
                }
                if (vertices.find(next) == vertices.end()){
                    Vertex v;
                    vertices[next] = v;
                }
                vertices[num].tail.insert(next);

                // if(count > 800000){
                //     break;
                // }
            }
        }

        for (auto &m:vertices){
            unsearched.emplace_back(m.first);
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

    void Print_all(bool t){
        if (t == true){
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
        }
        else{
            std::sort(unsearched.begin(),unsearched.end());
        }

        print("unsearched: \n");
        for(auto i:unsearched){
            cout << i << ", ";
            cout << vertices[i].tail.size() << endl;
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

        // int count = 0;
        while (searched.size() < vertices.size()){
            // if(count%300 == 0){
            //     cout << "initial direction count: ";
            //     print(count);
            // }
            // ++count;
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
        std::set<int> temp_group;
        searched.erase(searched.begin(),searched.end());
        // int count = 0;
        while(sequential.size()>0){
            // if(count%10000 == 0){
            //     cout << "reverse direction count: ";
            //     print(count);
            // }
            // ++count;
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
                            temp_group.insert(temp_stack.top());
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
                        temp_group.insert(temp_stack.top());
                        temp_stack.pop();
                    }
                }
            }

            max5.emplace_back(temp_group.size());

            // ++count;
            // cout << "NO." << count << " group of strongly connected conponents:\n";
            // for(auto i:temp_group){
            //     cout << i << ", ";
            // }
            // print("\n");
            temp_group.clear();
            // temp_group.erase(temp_group.begin(),temp_group.end());
        }

        std::sort(max5.begin(),max5.end());
        for (auto i=max5.end()-1;i>max5.end()-6;--i){
            cout << *i << ", ";
        }
        print("");
    }

    std::unordered_map<int,Vertex> vertices;
    vector<int> unsearched;
    std::unordered_set<int> searched;
    iterable_stack<int> sequential;
    vector<int> max5;
};

int main(){

    std::ifstream myfile("SCC_final.txt"); //https://s3-us-west-1.amazonaws.com/prod-edx/Algo1/Files/SCC.txt
    Graph graph(myfile);

    // graph.Print_all(false);

    print("========\n");
    
    graph.DFS_initial_direction();

    print("=========\n");
    
    // print("result of the first search: ")
    // for(auto i:graph.sequential){
    //     cout << i << ", ";
    // }
    // print("\n");

    // print("=========\n");

    graph.DFS_reverse_direction();

}

/* 
Programming Assignment 4
5/5 points (graded)
Download the following text file (right click and select "Save As..."): SCC.txt

The file contains the edges of a directed graph. Vertices are labeled as positive integers from 1 to 875714. Every row indicates an edge, the vertex label in first column is the tail and the vertex label in second column is the head (recall the graph is directed, and the edges are directed from the first column vertex to the second column vertex). So for example, the  row looks liks : "2 47646". This just means that the vertex with label 2 has an outgoing edge to the vertex with label 47646

Your task is to code up the algorithm from the video lectures for computing strongly connected components (SCCs), and to run this algorithm on the given graph.

Enter the sizes of the 5 largest SCCs in the given graph using the fields below, in decreasing order of sizes. So if your algorithm computes the sizes of the five largest SCCs to be 500, 400, 300, 200 and 100, enter 500 in the first field, 400 in the second, 300 in the third, and so on. If your algorithm finds less than 5 SCCs, then enter 0 for the remaining fields. Thus, if your algorithm computes only 3 SCCs whose sizes are 400, 300, and 100, then you enter 400, 300, and 100 in the first, second, and third fields, respectively, and 0 in the remaining 2 fields.

WARNING: This is the most challenging programming assignment of the course. Because of the size of the graph you may have to manage memory carefully. The best way to do this depends on your programming language and environment, and we strongly suggest that you exchange tips for doing this on the discussion forums.

top 5 SCC:
434821, 968, 459, 313, 211,
*/
