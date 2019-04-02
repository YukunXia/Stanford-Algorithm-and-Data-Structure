#include <iostream>
#include <vector>
#include <unordered_map>

using std::cout;
using std::endl;
using std::vector;

#define print(a) cout<<a<<endl;
#define be(v) v.begin(),v.end()

struct pair_hash
{
	template <class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2> &pair) const
	{
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};

int knapsack(vector<std::pair<int,int>>::iterator begin,\
             vector<std::pair<int,int>>::iterator end,int max_val,\
             std::unordered_map<std::pair<int,int>,int,pair_hash>& history){

    if (end - begin == 0 || max_val <= 0){
        return 0;
    }
    else if (max_val < (end-1)->second){ // dont forget this case, otherwise the answer is wrong!
        return 0;
    }
    else{
        std::pair<int,int> p1 = std::make_pair(end-begin-1,max_val);
        std::pair<int,int> p2 = std::make_pair(end-begin-1,max_val-(end-1)->second);
        if (history.find(p1) == history.end()){
            history[p1] = knapsack(begin,end-1,max_val,history);
        }
        if (history.find(p2) == history.end()){
            history[p2] = knapsack(begin,end-1,max_val-(end-1)->second,history);
        }
        return std::max(history[p1],history[p2] + (end-1)->first);
    }
}

int main(){

    // vector<std::pair<int,int>> val_wt = {{60,10}, {100,20}, {120,30}}; // optimal => 220
    // int max_val = 50;

    vector<std::pair<int,int>> val_wt = {{3,4}, {2,3}, {4,2},{4,3}}; // optimal => 8
    int max_val = 6;


    std::unordered_map<std::pair<int,int>,int,pair_hash> history;
    print(knapsack(be(val_wt),max_val,history));

    return 0;

}