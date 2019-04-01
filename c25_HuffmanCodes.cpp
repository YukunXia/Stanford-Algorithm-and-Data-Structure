#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <string>

using std::cout;
using std::endl;
using std::vector;
using std::string;

#define print(a) cout<<a<<endl;
#define be(v) v.begin(),v.end()

struct Node{
    Node (const int& weight_): weight(weight_) {}
    Node (){}
    vector<char> self_name;
    int weight;
    std::shared_ptr<Node> left_child;
    std::shared_ptr<Node> right_child;
};

struct Node_cmp{
    bool operator() (const std::shared_ptr<Node>& n1, const std::shared_ptr<Node>& n2){
        return n1->weight > n2->weight;
    }
};

struct Tree{
    void insert(const char& name, const int& weight){
        std::shared_ptr<Node> node_to_insert(new Node(weight));
        node_to_insert->self_name.emplace_back(name);
        node_list.push(node_to_insert);
        node_vec.emplace_back(node_to_insert);
        ++number_of_node;
    }
    std::shared_ptr<Node> combine(std::shared_ptr<Node>& n1, std::shared_ptr<Node>& n2){
        std::shared_ptr<Node> n_new(new Node(n1->weight + n2->weight));
        // print(n1->weight << " | " << n2->weight);
        if (n1->weight < n2->weight){
            // print("n1 < n2");
            n_new->left_child = n1;
            n_new->right_child = n2;
            std::copy(be(n1->self_name),std::back_inserter(n_new->self_name));
            std::copy(be(n2->self_name),std::back_inserter(n_new->self_name));
        }
        else{
            // print("n1 >= n2");
            n_new->left_child = n2;
            n_new->right_child = n1;
            std::copy(be(n2->self_name),std::back_inserter(n_new->self_name));
            std::copy(be(n1->self_name),std::back_inserter(n_new->self_name));
        }
        return n_new;
    }
    void build_huffman(){
        while (number_of_node >= 2){
            std::shared_ptr<Node> n1 = node_list.top();
            node_list.pop();
            std::shared_ptr<Node> n2 = node_list.top();
            node_list.pop();
            std::shared_ptr<Node> n_new = combine(n1,n2);
            node_list.push(n_new);

            node_vec.emplace_back(n_new);
            --number_of_node;
        }
    }
    void print_all(std::shared_ptr<Node> node_temp, string code = ""){

        if (node_temp->left_child && node_temp->left_child){
            code += "0";
            print_all(node_temp->left_child, code);
            code.pop_back();

            code += "1";
            print_all(node_temp->right_child, code);
            code.pop_back();
        }
        else if(node_temp->left_child && !node_temp->left_child){
            code += "0";
            print_all(node_temp->left_child, code);
            code.pop_back();
        }
        else if(!node_temp->left_child && node_temp->left_child){
            code += "1";
            print_all(node_temp->right_child, code);
            code.pop_back();
        }
        else{
            print(node_temp->self_name[0] << " - " << code);
        }
    }
    int number_of_node = 0;
    vector<std::shared_ptr<Node>> node_vec;
    std::priority_queue<std::shared_ptr<Node>,vector<std::shared_ptr<Node>>,Node_cmp> node_list;
};

int main(){

    std::map<char,int> MAP({{'A',3},{'B',2},{'C',6},{'D',8},{'E',2},{'F',6}});
    // std::map<char,int> MAP({{'A',4},{'B',2},{'E',3}});

    Tree tree;
    for (auto i:MAP){
        tree.insert(i.first,i.second);
    }

    tree.build_huffman();

    tree.print_all(tree.node_vec.back());

    return 0;
}

/* typical output

C - 00
F - 01
A - 100
E - 1010
B - 1011
D - 11

*/