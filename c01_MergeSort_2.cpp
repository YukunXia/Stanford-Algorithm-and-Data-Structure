#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::cin;
using std::sort;
using std::endl;

template <typename T>
void MergeSort(typename vector<T>::iterator begin, typename vector<T>::iterator end){

    int dis = std::distance(begin,end);

    if (dis == 0){
        return;
        //throw std::logic_error("Empty array!!!");
    }
    else if (dis == 1){
        return;
    }
    else if (dis ==  2){
        sort(begin,end);
    }
    else{

        int mid = dis/2;
        
        MergeSort<T>(begin,begin+mid);
        MergeSort<T>(begin+mid,end);

        vector<T> left(begin,begin+mid);
        vector<T> right(begin+mid,end);

        // start merging
        int j = 0;
        int k = 0;
        for (int i = 0;i < dis; ++i){
            
            if (left[j] < right[k] && j < mid){
                *(begin+i) = left[j];
                ++j;
            }
            else if(left[j] >= right[k] && k < (dis - mid)){
                *(begin+i) = right[k];
                ++k;
            }
            else if(j == mid){
                *(begin+i) = right[k];
                ++k;
            }
            else if(k == dis - mid){
                *(begin+i) = left[j];
                ++j;
            }
        } 
    }
}


int main(){

    vector<double> list = {5.1,4.1,1.1,8.1,7.1,2.1,6.1,3.1};

    for (int i = 0; i < list.size(); ++i){
        cout << list[i] << " ";
    } // print the original list : 5,4,1,8,7,2,6,3
    cout << endl;

    MergeSort<double>(list.begin(),list.end());

    for (auto i : list){
        cout << i << " ";
    }// print the sorted list : 1 2 3 4 5 6 7 8
    cout << endl;

    return 0;

}
