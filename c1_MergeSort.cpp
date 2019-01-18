#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::cin;
using std::sort;
using std::endl;

template <class T>
void MergeSort(vector<T> &list){
    // cout << "start func:" << endl;

    if (list.size()  ==  1){
        // cout << "case 0:" << endl;
    }
    else if (list.size() ==  2){
        // cout << "case 1:" << endl;
        sort(list.begin(),list.end());
    }
    else{
        // cout << "case 2:" << endl;
        int mid = list.size()/2;
        vector<T> left(list.begin(),list.begin()+mid);
        vector<T> right(list.begin()+mid,list.end());
        MergeSort(left);
        MergeSort(right);

        // cout << "left" << endl;
        // for (auto i : left){
        //     cout << i << " ";
        // }
        // cout << endl;
        // cout << "right" << endl;
        // for (auto i : right){
        //     cout << i << " ";
        // }
        // cout << endl;

        // start merging
        int j = 0;
        int k = 0;
        for (int i = 0;i < list.size(); ++i){
            
            // cout << "i: " << i << endl;
            // cout << "j: " << j << ", len(left): " << left.size() << ", left j:" << left[j] << endl;  
            // cout << "k: " << k << ", len(right): " << right.size() << ", right k:" << right[k] << endl;  

            if (left[j] < right[k] && j < left.size()){
                list.at(i) = left[j];
                ++j;
            }
            else if(left[j] >= right[k] && k < right.size()){
                list.at(i) = right[k];
                ++k;
            }
            else if(j == left.size()){
                list.at(i) = right[k];
                ++k;
            }
            else if(k == right.size()){
                list.at(i) = left[j];
                ++j;
            }
        } 
    }
    
}


int main(){

    vector<float> list = {5.1,4.1,1.1,8.1,7.1,2.1,6.1,3.1};

    for (int i = 0; i < list.size(); ++i){
        cout << list[i] << " ";
    } // print the original list : 5,4,1,8,7,2,6,3
    cout << endl;

    MergeSort<float>(list);

    for (auto i : list){
        cout << i << " ";
    }// print the sorted list : 1 2 3 4 5 6 7 8
    cout << endl;

    return 0;

}