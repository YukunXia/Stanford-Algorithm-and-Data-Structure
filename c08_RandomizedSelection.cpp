#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>

using std::cout;
using std::endl;
using std::vector;

template <typename T>
int RandomizedSelection1(typename vector<T>::iterator begin, typename vector<T>::iterator end,\
                        int index){ // stable_partition
    int length = std::distance(begin,end);
    if (index > length || index <= 0){
        throw std::logic_error("Index Out Of Bound!!!");
    }
    T object_for_compare = *begin;
    if (length > 1){
        auto cutting = std::stable_partition(begin,end,[&](T em){return em < object_for_compare;});
        int cutting_pos =  std::distance(begin,cutting);
        if (cutting_pos == index-1){
            return *cutting;
        }
        else if (cutting_pos > index - 1){
            return RandomizedSelection1<T>(begin,cutting,index);
        }
        else{
            return RandomizedSelection1<T>(cutting+1,end,index-cutting_pos-1);
        }
    }
    else{
        return *begin;
    }
}

template <typename T>
int RandomizedSelection2(typename vector<T>::iterator begin, typename vector<T>::iterator end,\
                        int index){ // partition
    int length = std::distance(begin,end);
    if (index > length || index <= 0){
        throw std::logic_error("Index Out Of Bound!!!");
    }
    T object_for_compare = *begin;
    if (length > 1){
        auto cutting = std::partition(begin,end,[&](T em){return em < object_for_compare;});
        int cutting_pos =  std::distance(begin,cutting);
        if (cutting_pos == index-1){
            return *cutting;
        }
        else if (cutting_pos > index - 1){
            return RandomizedSelection2<T>(begin,cutting,index);
        }
        else{
            return RandomizedSelection2<T>(cutting+1,end,index-cutting_pos-1);
        }
    }
    else{
        return *begin;
    }
}

clock_t Begin,End;

int main(){

    std::ifstream myfile("IntegerArray.txt");
    std::istream_iterator<int> file_top(myfile),file_bottom;
    vector<int> number_list1(file_top,file_bottom);
    vector<int> number_list2(number_list1.begin(),number_list1.end());

    int N = 999;

    Begin = clock();
    cout << N << "th element is: " << RandomizedSelection1<int>(number_list1.begin(),number_list1.end(),N) << endl;
    End = clock(); // ~ 3.3ms
    cout << "Time for stable_partition:" << float(End - Begin)/CLOCKS_PER_SEC << endl;



    Begin = clock();
    cout << N << "th element is: " << RandomizedSelection2<int>(number_list2.begin(),number_list2.end(),N) << endl;
    End = clock(); // ~ 2.4ms
    cout << "Time for partition:" << float(End - Begin)/CLOCKS_PER_SEC << endl;

    // so stable_partition is slower than partition here!!!

    return 0;

}
