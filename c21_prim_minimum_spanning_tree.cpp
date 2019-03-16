#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <random>

using std::cout;
using std::endl;
using std::vector;
using std::string;

#define print(a) cout<<a<<endl;
#define be(v) v.begin(),v.end()
#define NodePair std::pair<std::shared_ptr<Node>,int>



struct Node{
    Node (){}
    Node (const int self_num_): self_num(self_num_){}
    int self_num;
    vector<NodePair> neighbor;
};

struct cmp_pair{
    bool operator()(const NodePair next1,const NodePair next2){
        return (next1.second > next2.second);
    }
};

int main(){

    std::ifstream myfile("edges.txt");
    string line;
    std::unordered_map<int,std::shared_ptr<Node>> map_from_num_to_node;
    int number_of_nodes, number_of_edges;

    if (myfile.is_open()){
        std::getline(myfile,line);
        std::stringstream stream(line);
        stream >> number_of_nodes >> number_of_edges;
        // print("number_of_nodes = " << number_of_nodes << ", number_of_edges = " << number_of_edges);
        while (std::getline(myfile,line)){
            std::stringstream stream(line);
            int node1;
            int node2;
            int weight;
            stream >> node1 >> node2 >> weight;
            if (map_from_num_to_node.find(node1) == map_from_num_to_node.end()){
                std::shared_ptr<Node> node1_ptr(new Node(node1));
                map_from_num_to_node.insert({node1,node1_ptr});
            }
            if (map_from_num_to_node.find(node2) == map_from_num_to_node.end()){
                std::shared_ptr<Node> node2_ptr(new Node(node2));
                map_from_num_to_node.insert({node2,node2_ptr});
            }

            NodePair N1_to_N2 = std::make_pair(map_from_num_to_node[node2],weight);
            NodePair N2_to_N1 = std::make_pair(map_from_num_to_node[node1],weight);
            // could these two lines join the two lines code below? more efficient?

            map_from_num_to_node[node1]->neighbor.emplace_back(N1_to_N2);
            map_from_num_to_node[node2]->neighbor.emplace_back(N2_to_N1);
        }
    }

    long sum = 0;
    std::unordered_set<int> searched;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0,number_of_nodes-1);
    int current_num = dis(gen);

    searched.insert(current_num);
    std::priority_queue<NodePair,vector<NodePair>,cmp_pair> frontline;

    while (searched.size() < number_of_nodes){
        for (auto next:map_from_num_to_node[current_num]->neighbor){
            frontline.push(next);
        }

        int potential_next_num = frontline.top().first->self_num;

        while(searched.find(potential_next_num) != searched.end()){
            frontline.pop();
            potential_next_num = frontline.top().first->self_num;
        }
        int weight = frontline.top().second;
        current_num = potential_next_num;
        searched.insert(current_num);
        sum += weight;

    }

    print("total number of nodes = " << number_of_nodes);
    print("searched size = " << searched.size());
    print("sum = " << sum);
    // print("unsearched num = 0??? " << (number_of_nodes - searched.size() == 0))


    return 0;

}

/* typical output

total number of nodes = 500
searched size = 500
sum = -3612829

*/