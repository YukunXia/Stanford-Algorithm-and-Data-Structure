#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::cin;
using std::sort;
using std::endl;

template <typename T>
int CountInversions(vector<T> &list){

    int counter = 0;

    if (list.size()  ==  1){
    }
    else if (list.size() ==  2){
        if (list.front() > list.back()){
            ++counter;
            sort(list.begin(),list.end());
        }
    }
    else{
        int mid = list.size()/2;
        vector<T> left(list.begin(),list.begin()+mid);
        vector<T> right(list.begin()+mid,list.end());
        counter += CountInversions(left);
        counter += CountInversions(right);

        // start merging
        int j = 0;
        int k = 0;
        for (int i = 0;i < list.size(); ++i){
            
            if (left[j] < right[k] && j < left.size()){
                list.at(i) = left[j];
                ++j;
            }
            else if(left[j] >= right[k] && k < right.size()){
                list.at(i) = right[k];
                ++k;
                counter += mid-j;
            }
            else if(j == left.size()){
                list.at(i) = right[k];
                ++k;
                counter += mid-j;
            }
            else if(k == right.size()){
                list.at(i) = left[j];
                ++j;
            }
        } 
    }
    return counter;
    
}


int main(){

    vector<int> list = {5,4,1,8,7,2,6,3};

    for (int i = 0; i < list.size(); ++i){
        cout << list[i] << " ";
    } // print the original list : 5,4,1,8,7,2,6,3
    cout << endl;

    int counter = 0;

    counter = CountInversions<int>(list);

    cout << "Number of inversions:" << counter << endl; // Number of inversions = 11

    return 0;

}