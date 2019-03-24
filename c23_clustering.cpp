#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <unordered_map>
#include <limits>

using std::cout;
using std::endl;
using std::cin;
using std::vector;

#define print(a) cout<<a<<endl;
#define be(v) v.begin(),v.end()

struct Node{
    Node (){}
    Node (const int num): self_num(num) {}
    int self_num;
    vector<std::shared_ptr<Node>> point_in;
    std::shared_ptr<Node> point_out;
};

struct Edge{
    Edge () {}
    Edge (const int n1, const int n2, const int wei):node1(n1),node2(n2),weight(wei){}
    int node1;
    int node2;
    int weight;
};

struct edge_cmp{
    bool operator() (std::shared_ptr<Edge> e1, std::shared_ptr<Edge> e2){
        return e1->weight > e2->weight;
    }
};

struct Tree{
    void insert(const int num_for_insert,std::shared_ptr<Node> node_for_insert){
        node_archive.insert({num_for_insert,node_for_insert});
        ++node_quantity;
    }
    bool merge(const int node1_num, const int node2_num){
        int ancestor1 = ancestor_and_depth(node1_num).first;
        int ancestor2 = ancestor_and_depth(node2_num).first;
        int depth1 = ancestor_and_depth(node1_num).second;
        int depth2 = ancestor_and_depth(node2_num).second;
        if (ancestor1 != ancestor2){
            if (depth1 > depth2){
                node_archive[ancestor2]->point_out = node_archive[ancestor1];
                node_archive[ancestor1]->point_in.emplace_back(node_archive[ancestor2]);
            }
            else{
                node_archive[ancestor1]->point_out = node_archive[ancestor2];
                node_archive[ancestor2]->point_in.emplace_back(node_archive[ancestor1]);
            }
            return true;
        }
        else{
            return false;
        }
    }
    void print_all(){
        std::unordered_map<int,int> existing_group;
        for (auto n:node_archive){
            cout << n.first << ": ";
            if (existing_group.find(ancestor_and_depth(n.second->self_num).first) == existing_group.end()){
                existing_group.insert({ancestor_and_depth(n.second->self_num).first,1});
            }
            else{
                ++existing_group[ancestor_and_depth(n.second->self_num).first];
            }
            print(ancestor_and_depth(n.second->self_num).first);
        }
        for (auto i:existing_group){
            cout << "(" << i.first << "," << i.second << ") ";
        }
        cout << endl;
    }
    
    std::pair<int,int> ancestor_and_depth(int node_num){
        int depth;
        while (node_archive[node_num]->point_out){
            ++depth;
            node_num = node_archive[node_num]->point_out->self_num;
        }
        return std::make_pair(node_num,depth);
    }
    int max_spacing(){
        int cache_spacing = std::numeric_limits<int>::max();
        for (auto e:edge_archive){
            int node1_num = e->node1;
            int node2_num = e->node2;
            int ancestor1 = ancestor_and_depth(node1_num).first;
            int ancestor2 = ancestor_and_depth(node2_num).first;
            if (ancestor1 != ancestor2){
                if (cache_spacing > e->weight){
                    cache_spacing = e->weight;
                }
            }
        }
        return cache_spacing;
    }
    std::vector<std::shared_ptr<Edge>> edge_archive;
    std::unordered_map<int,std::shared_ptr<Node>> node_archive;
    int node_quantity = 0;
};

int main(){

    freopen("clustering1.txt","r",stdin);
    int num_of_nodes = 0;
    cin >> num_of_nodes;
    // print(num_of_nodes);


    std::unordered_set<int> node_searched;
    std::priority_queue<std::shared_ptr<Edge>,vector<std::shared_ptr<Edge>>,edge_cmp> edge_archive;
    Tree tree;


    int edge_node1,edge_node2,edge_cost;
    while (cin >> edge_node1 >> edge_node2 >> edge_cost){
        if (node_searched.find(edge_node1) == node_searched.end()){
            node_searched.insert(edge_node1);
            std::shared_ptr<Node> node_temp(new Node(edge_node1));
            tree.insert(edge_node1,node_temp);
        }
        if (node_searched.find(edge_node2) == node_searched.end()){
            node_searched.insert(edge_node2);
            std::shared_ptr<Node> node_temp(new Node(edge_node2));
            tree.insert(edge_node2,node_temp);
        }
        // print(edge_node1 << " " << edge_node2 << " "  << edge_cost);
        std::shared_ptr<Edge> edge_temp(new Edge(edge_node1,edge_node2,edge_cost));
        edge_archive.push(edge_temp);
        tree.edge_archive.emplace_back(edge_temp);
    }

    // print("\n");
    int count = tree.node_quantity;
    while(!edge_archive.empty() && count > 4){
        // print(edge_archive.top()->weight);

        if (tree.merge(edge_archive.top()->node1, edge_archive.top()->node2)){
            --count;
        }

        edge_archive.pop();
    }

    // print("\n");

    // tree.print_all();

    print("\n");

    print(tree.max_spacing());

    return 0;

}

/* typical outputs:

106

*/