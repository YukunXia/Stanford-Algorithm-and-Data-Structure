#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>

using std::cout;
using std::cin;
using std::endl;
using std::vector;

template <typename T>
void QuickSort_Easyway(vector<T> &list){ // always create new vector, and use memory freely!!!

    int length = list.size();
    if (length == 2){
        std::sort(list.begin(),list.end());
    }
    else if (length > 2){
        vector<T> cache(length,0);
        int j=0,k=1;
        for (int i=1; i<length ; ++i){ // use the first element as pivot
            if (list[i] <= list.front()){
                cache.at(j) = list[i];
                ++j;
            }
            else{
                cache.at(length-k) = list[i];
                ++k;
            }
        }

        T pivot = list.front();

        vector<T> left(cache.begin(),cache.begin()+j);
        vector<T> right(cache.begin()+j+1,cache.end());
        QuickSort_Easyway<T>(left);
        QuickSort_Easyway<T>(right);

        std::copy(left.begin(),left.end(),list.begin());
        list.at(j) = pivot;
        std::copy(right.begin(),right.end(),list.begin()+j+1);
    }
}

template <typename T>
void QuickSort_InPlace(typename vector<T>::iterator begin, typename vector<T>::iterator end){ 
    // avoid occupying memory. Instead, prefer swap!!!

    int length = std::distance(begin,end);
    if (length == 2){
        std::sort(begin,end);
    }
    else if (length > 2){ // four partitions: pivot | not larger | larger | unknown
        T pivot = *begin; // i is the first index of 4th partition, [i,end)
        int j = 1; // j is the first index of 3rd partition
        for (int i=1; i < length ; ++i){ // 2nd partition: [1,j), or at first j = 1
            if (*(begin+i) <= pivot){
                if (i!=j){ // slower for ~1ms without this code block
                    std::iter_swap(begin+i,begin+j);
                }
                ++j;
            }
        }
        std::move(begin+1 , begin+j, begin); // move [begin+1,begin+j) to begin
        *(begin+j-1) = pivot;

        QuickSort_InPlace<T>(begin,begin+j-1);
        QuickSort_InPlace<T>(begin+j,end);
    }
}

int main(){

    // ---------------------- toy ---------------------- case ----------------------

    // vector<int> list = {6,1,10,2,3,0,4,2,5};
    // QuickSort_Easyway<int>(list);

    // cout << "Easy Way:" << endl;
    // for (auto i : list){
    //     cout << i << " ";
    // }
    // cout << endl;

    // list = {6,1,10,2,3,0,4,2,5};
    // QuickSort_InPlace<int>(list.begin(),list.end());

    // cout << "In Place:" << endl;
    // for (auto i : list){
    //     cout << i << " ";
    // }
    // cout << endl;

    // ---------------------- 100,000 ---------------------- numbers ----------------------

    clock_t  Begin, End;

    std::ifstream myfile("IntegerArray.txt");
    std::istream_iterator<int> begin(myfile),end;
    vector<int> number_list0(begin,end);
    vector<int> number_list1(number_list0.begin(),number_list0.end());

    Begin = clock();
    QuickSort_Easyway<int>(number_list0);
    End = clock();
    cout << float(End-Begin)/CLOCKS_PER_SEC << endl; // approx ~ 0.097s
    // cout << "Easy Way:" << endl;
    // for (auto i : number_list0){
    //     cout << i << " ";
    // }
    // cout << endl;

    Begin = clock();
    QuickSort_InPlace<int>(number_list1.begin(),number_list1.end());  
    End = clock();
    cout << float(End-Begin)/CLOCKS_PER_SEC << endl; // approx ~ 0.026s
    // cout << "In Place:" << endl;
    // for (auto i : number_list1){
    //     cout << i << " ";
    // }
    // cout << endl;

    return 0;

}