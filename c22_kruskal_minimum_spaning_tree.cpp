#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <unordered_set>

using std::cout;
using std::endl;
using std::vector;
using std::string;

#define print(a) cout<<a<<endl;
#define be(v) v.begin(),v.end()
#define NodePair std::pair<std::shared_ptr<Node>,int>

struct Edge{
    Edge(){}
    Edge(const int node1_, const int node2_,const int weight_):node1(node1_),node2(node2_),weight(weight_){}
    int node1;
    int node2;
    int weight;
};

struct Edge_cmp{
    bool operator()(const Edge& e1, const Edge& e2){
        return e1.weight > e2.weight;
    }
};

struct Group{
    Group(){}
    Group(const int node_num_){
        leader = node_num_;
        NodeList.emplace_back(node_num_);
    }
    void MergeGroup(const std::shared_ptr<Group> g2){
        std::copy(be(g2->NodeList),std::back_inserter(NodeList));
    }
    int leader;
    vector<int> NodeList;
};

struct AllNodes{
    void Insert(int node_num){
        std::shared_ptr<Group> temp_group(new Group(node_num));
        Node_to_Group.insert({node_num,temp_group});
        all_groups.insert(temp_group);
    }
    bool Merge_Groups(std::shared_ptr<Group> g1, std::shared_ptr<Group> g2){
        if (g1->leader != g2->leader){
            g1->MergeGroup(g2);
            for (auto n:g2->NodeList){
                Node_to_Group[n] = g1;
            }
            all_groups.erase(all_groups.find(g2));
            return true;
        }
        else{
            return false;
        }
    }
    std::unordered_set<std::shared_ptr<Group>> all_groups;
    std::unordered_map<int,std::shared_ptr<Group>> Node_to_Group;
};

int main(){

    std::ifstream myfile("edges.txt");
    string line;
    std::getline(myfile,line);
    std::stringstream stream(line);
    int num_nodes, num_edges;
    stream >> num_nodes >> num_edges;
    std::unordered_set<int> node_searched;
    std::priority_queue<Edge,vector<Edge>,Edge_cmp> edge_queue;
    AllNodes allnodes;

    while(std::getline(myfile,line)){
        std::stringstream stream(line);
        int node1, node2, weight;
        stream >> node1 >> node2 >> weight;
        Edge n1_and_n2(node1,node2,weight);
        edge_queue.push(n1_and_n2);
        if (node_searched.find(node1) == node_searched.end()){
            node_searched.insert(node1);
            allnodes.Insert(node1);
        }
        if (node_searched.find(node2) == node_searched.end()){
            node_searched.insert(node2);
            allnodes.Insert(node2);
        }
    }



    long sum = 0;
    int refused = 0;
    while(!edge_queue.empty()){
        Edge edge_focus = edge_queue.top();
        print('\n'<<edge_focus.node1<<", "<<edge_focus.node2);
        print("weight : "<<edge_focus.weight);
        std::shared_ptr<Group>& g1 = allnodes.Node_to_Group[edge_focus.node1];
        std::shared_ptr<Group>& g2 = allnodes.Node_to_Group[edge_focus.node2];
        if (allnodes.Merge_Groups(g1,g2)){
            sum += edge_focus.weight;
            print("yes!");
        }
        edge_queue.pop();
    }
    print("\nsum = " << sum);

    return 0;

}