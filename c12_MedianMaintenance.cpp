#include <iostream>
#include <vector>
#include <queue>

using std::cout;
using std::endl;
using std::vector;

#define print(a) cout<<(a)<<endl;

struct Median_Heap{

    void push(int t){
        ++count;
        if (count > 2){
            if (t < max_heap.top()){
                max_heap.push(t);
            }
            else{
                min_heap.push(t);
            }
            if (min_heap.size() > max_heap.size()+1){
                max_heap.push(min_heap.top());
                min_heap.pop();
            }
            else if (max_heap.size() > min_heap.size()){
                min_heap.push(max_heap.top());
                max_heap.pop();
            }
            if (count%2 == 1){
                cout << min_heap.top() << endl;
            }
            else{
                cout << 0.5*float(min_heap.top()+max_heap.top()) << endl;
            }
        }
        else if(count == 2){
            if (t > min_heap.top()){
                max_heap.push(min_heap.top());
                min_heap.pop();
                min_heap.push(t);
            }
            else{
                max_heap.push(t);
            }
            cout << 0.5*float(min_heap.top()+max_heap.top()) << endl;
        }
        else{
            min_heap.push(t);
            cout << t << endl;
        }
        
    }

    void clear(){
        std::priority_queue<int,vector<int>,std::greater<int>> min_h;
        std::priority_queue<int,vector<int>,std::less<int>> max_h;
        min_heap = min_h;
        max_heap = max_h;
    }

    int count = 0;
    std::priority_queue<int,vector<int>,std::greater<int>> min_heap;
    std::priority_queue<int,vector<int>,std::less<int>> max_heap;
};

int main(){

    Median_Heap median_heap;

    for(auto i:{1,2,3,4}){
        median_heap.push(i);
    }

    print("\n");
    median_heap.clear();

    for(auto i:{10,11,0,-1,-2,12,5,0,3}){
        median_heap.push(i);
    }

    return 0;

}

/* output

1
1.5
2
2.5


10
10.5
10
5
0
5
5
2.5
3

*/