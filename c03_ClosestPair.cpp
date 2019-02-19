#include <iostream>
#include <cmath> // cmath is a wrapper of math.h for c++
#include <vector>

using std::vector;
using std::cout;
using std::endl;
using std::sort;

// define data structure
struct Point{
    Point(double x_, double y_): x{x_}, y{y_} {}
    double x;
    double y;
};

// define comparision operators
bool XCompare (Point &a, Point &b){
    return a.x < b.x;
}

bool YCompare (Point &a, Point &b){
    return a.y < b.y;
}

// core function
double ClosestPair(vector<Point>::iterator begin, vector<Point>::iterator end){
    
    int length = std::distance(begin,end);

    if (length == 2){
        return sqrt(pow(begin->x - (begin+1)->x,2) + pow(begin->y - (begin+1)->y,2));
    }
    else if (length == 3){
        return std::min({sqrt(pow(begin->x - (begin+1)->x,2) + pow(begin->y - (begin+1)->y,2)),\
        sqrt(pow(begin->x - (begin+2)->x,2) + pow(begin->y - (begin+2)->y,2)),\
        sqrt(pow((begin+2)->x - (begin+1)->x,2) + pow((begin+2)->y - (begin+1)->y,2))});
    }
    else if (length > 3){

        // recursive results and calculate delta (i.e. left_or_right_smallest)
        double left_smallest = ClosestPair(begin, begin+length/2);
        double right_smallest = ClosestPair(begin+length/2, end);
        double left_or_right_smallest = std::min(left_smallest,right_smallest);

        // pick out the points whose x-axis distances to the rightmost point in left set are within delta
        int i = length/2-2;
        if (i > 0){
            while (i > 0){
                if ((begin + i)->x - (begin + length/2 - 1)->x > left_or_right_smallest){
                    ++i;
                    break;
                }
                --i;
            }
        }
        else{
            if ((begin + length/2 - 1)->x - (begin + i)->x > left_or_right_smallest){
                ++i;
            }
        }

        int j = length/2;
        while (j < length){
            if ((begin + j)->x - (begin + length/2 - 1)->x > left_or_right_smallest){
                break;
            }
            ++j;
        }

        // sort these special boundary points according to y value
        vector<Point> two_delta(begin+i,begin+j);
        sort(two_delta.begin(),two_delta.end(),YCompare);

        // loop these points and compare distance between adjacent points to delta
        double distance_temp = 0;
        for (int k=0; k < two_delta.size(); ++k){
            for (int l=k+1; l < two_delta.size(); ++l){
                if (l-k > 7){
                    break;
                }
                distance_temp = sqrt(pow(two_delta.at(k).x - two_delta.at(l).x,2) + \
                pow(two_delta.at(k).y - two_delta.at(l).y,2));
                if (distance_temp < left_or_right_smallest){
                    left_or_right_smallest = distance_temp;
                }
            }
        }

        return left_or_right_smallest;
    }
    else {
        throw std::logic_error("Length is too small!!!");
    }
}

// brute force implement for validation
double brute_force_ClosestPair(vector<Point>& p_list){

    int length = p_list.size();

    if (length < 2){
        throw std::logic_error("Length is too small!!!");
    }
    
    vector<double> temp;

    for (auto i : p_list){
        for (auto j : p_list){
            if (i.x != j.x or i.y != j.y){
                temp.emplace_back(sqrt(pow(i.x - j.x,2) + pow(i.y - j.y,2)));
            }
        }
    }

    return *std::min_element(temp.begin(),temp.end());

}

int main(){

    // initialize random seed with actual date-time
    std::srand(std::time(NULL));

    vector<Point> p_list; // = {{1.03,1.05},{2.0,1.02},{3,1.01},{4,1.00}}; // a simple test case

    // generate 100 points. to make points have little chance to overlap, the modulo value is set to 1000.
    for (int point_num = 0; point_num < 30; ++point_num){
        int random_x = std::rand()%1000;
        int random_y = std::rand()%1000;
        p_list.emplace_back(Point(random_x,random_y));
    }

    // sort for preparation
    sort(p_list.begin(),p_list.end(),XCompare);

    // show results
    cout << ClosestPair(p_list.begin(),p_list.end()) << endl;
    cout << brute_force_ClosestPair(p_list) << endl;

    return 0;
}
