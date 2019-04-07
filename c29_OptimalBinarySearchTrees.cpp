#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits>

using std::cout;
using std::endl;
using std::vector;
using std::unordered_map;

#define print(a) cout<<a<<endl;
#define be(v) v.begin(),v.end()

struct pair_hash{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1,T2>& pair) const{
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

double optimal_BST(vector<double>::iterator begin, vector<double>::iterator end,\
                 const vector<double>::iterator& true_begin,
                 unordered_map<std::pair<int,int>,double,pair_hash>& history){
    if (begin >= end){
        return 0;
    }
    else if (begin == end-1){
        return *begin;
    }
    // for (auto ptr1=begin; ptr1<end; ++ptr1){
        // print("ptr1-begin: " << ptr1-begin);
    auto ptr1 = begin;
    for (auto ptr3=ptr1; ptr3<end; ++ptr3){
        // print("ptr3-begin: " << ptr3-begin);
        std::pair<int,int> pair_13 = std::make_pair(ptr1-true_begin,ptr3-true_begin+1);
        if (history.find(pair_13) == history.end()){
            double sum_pk = 0;
            double min_from_ptr1_to_ptr3 = std::numeric_limits<double>::max();
            for (auto ptr2=ptr1; ptr2<ptr3+1; ++ptr2){
                // print("ptr2-begin: " << ptr2-begin);
                sum_pk += *ptr2;
                std::pair<int,int> pair_12 = std::make_pair(ptr1-true_begin,ptr2-1-true_begin+1);
                if (history.find(pair_12) == history.end()){
                    history[pair_12] = optimal_BST(ptr1,ptr2-1+1,true_begin,history);
                    // print(pair_12.first << " - " << pair_12.second << " : " << history[pair_12]);
                }
                std::pair<int,int> pair_23 = std::make_pair(ptr2+1-true_begin,ptr3-true_begin+1);
                if (history.find(pair_23) == history.end()){
                    history[pair_23] = optimal_BST(ptr2+1,ptr3+1,true_begin,history);
                    // print(pair_23.first << " - " << pair_23.second << " : " << history[pair_23]);
                }
                double sum_of_left_and_right_subproblem = history[pair_12]+history[pair_23];
                min_from_ptr1_to_ptr3 = std::min(sum_of_left_and_right_subproblem,min_from_ptr1_to_ptr3);
            }
            history[pair_13] = min_from_ptr1_to_ptr3 + sum_pk;
            // print(pair_13.first << " - " << pair_13.second << " : " << history[pair_13]);
        }
    }
    // }
    std::pair<int,int> pair_be = std::make_pair(begin-true_begin,end-true_begin);
    return history[pair_be];
}


int main(){

    // vector<double> probability = {1};  // => optimal cost = 1
    // vector<double> probability = {0.5,0.5};  // => optimal cost = 1.5
    vector<double> probability = {0.25,0.2,0.05,0.2,0.3}; // => optimal cost = 2.1, https://www.radford.edu/~nokie/classes/360/dp-opt-bst.html

    auto begin = probability.begin();
    unordered_map<std::pair<int,int>,double,pair_hash> history;
    print(optimal_BST(be(probability),begin,history));

    return 0;

}