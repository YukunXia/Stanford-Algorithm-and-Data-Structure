#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::cin;
using std::sort;
using std::endl;

void MergeSort(vector<int> &list){
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
        vector<int> left(list.begin(),list.begin()+mid);
        vector<int> right(list.begin()+mid,list.end());
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

    vector<int> list = {5,4,1,8,7,2,6,3};

    for (int i = 0; i < list.size(); ++i){
        cout << list[i] << " ";
    } // print the original list
    cout << endl;

    MergeSort(list);

    for (auto i : list){
        cout << i << " ";
    }// print the sorted list
    cout << endl;









    // for (int i = 0; i < list.size(); ++i){
    //     cout << list[i] << endl;
    // } // print the list

    return 0;

}