#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>

using std::cout;
using std::endl;
using std::vector;
using std::string;

#define print(a) cout<<a<<endl;
#define be(v) v.begin(),v.end()

struct job{
    job() {};
    // job(const int job_weight_, const int job_length_): \
    //         job_weight(job_weight_), job_length(job_length_) {}
    job(const int job_weight_, const int job_length_){
        job_weight = job_weight_;
        job_length = job_length_;
        priority = job_weight-job_length;
    }
    int job_weight = 0;
    int job_length = 0;
    int priority = job_weight-job_length;
};

std::ostream& operator <<(std::ostream& os, const job j){
    os << "weight: " << j.job_weight << " , length: " << j.job_length << " , priority: " << j.priority << endl;
    return os;
};


struct cmp_job{
    bool operator()(std::shared_ptr<job> j1, std::shared_ptr<job> j2){
        if (j1->priority != j2->priority){
            return (j1->priority < j2->priority);
        }
        else{
            return (j1->job_weight < j2->job_weight);
        }
    }
};



int main(){

    std::priority_queue<std::shared_ptr<job>,vector<std::shared_ptr<job>>,cmp_job> job_queue;

    std::ifstream myfile("jobs.txt");
    if (myfile.is_open()){
        string line;
        std::getline(myfile,line);
        int number_of_jobs = std::stoi(line);
        for (int i=0; i<number_of_jobs; ++i){
            std::getline(myfile,line);
            std::stringstream stream(line);
            std::istream_iterator<string> Begin(stream),End;
            std::vector<string> vstring(Begin,End);

            std::shared_ptr<job> j_temp(new job(std::stoi(vstring[0]),std::stoi(vstring[1])));
            job_queue.push(j_temp);
        }
    }

    int64_t completion_sum = 0;
    int timer = 0;
    while(!job_queue.empty()){
        // print(*job_queue.top());
        timer += job_queue.top()->job_length;

        completion_sum += timer*job_queue.top()->job_weight;
        print(timer << ", "<<completion_sum);
        job_queue.pop();
    }

    print("sum of completion = " << completion_sum);

    return 0;

}

/* typical output:

...................
...................
509114, 69108670958
weight: 1 , length: 96 , priority: -95

509210, 69109180168
weight: 1 , length: 96 , priority: -95

509306, 69109689474
weight: 2 , length: 98 , priority: -96

509404, 69110708282
weight: 2 , length: 98 , priority: -96

509502, 69111727286
weight: 1 , length: 97 , priority: -96

509599, 69112236885
weight: 1 , length: 97 , priority: -96

509696, 69112746581
weight: 1 , length: 97 , priority: -96

509793, 69113256374
weight: 1 , length: 97 , priority: -96

509890, 69113766264
weight: 3 , length: 100 , priority: -97

509990, 69115296234
weight: 3 , length: 100 , priority: -97

510090, 69116826504
weight: 3 , length: 100 , priority: -97

510190, 69118357074
weight: 2 , length: 99 , priority: -97

510289, 69119377652
sum of completion = 69119377652

*/