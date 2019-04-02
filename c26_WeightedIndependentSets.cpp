#include <iostream>
#include <vector>
#include <unordered_map>

using std::cout;
using std::endl;
using std::vector;

#define print(a) cout<<a<<endl;
#define be(v) v.begin(),v.end()

int wis(vector<int>::iterator begin, vector<int>::iterator end,\
        std::unordered_map<int,int>& history){
    if (end-begin == 0){
        return 0;
    }
    else if (end-begin == 1){
        return *begin;
    }
    else{
        if (history.find(end-1-begin) == history.end()){
            history[end-1-begin] = wis(begin,end-1,history);
        }
        if (history.find(end-2-begin) == history.end()){
            history[end-2-begin] = wis(begin,end-2,history);
        }
        return std::max(history[end-1-begin],history[end-2-begin] + *(end-1));
    }
}

int main(){

    vector<int> path_graph;
    for (auto i:{1,4,5,4}){
        path_graph.emplace_back(i);
    }

    std::unordered_map<int,int> history;
    print(wis(be(path_graph),history));

    return 0;

}