#include <iostream>
#include <vector>
// #include <memory>
#include <limits>
#include <iomanip>
#include <cmath>
#include <string>

using std::cout;
using std::endl;
using std::vector;
using std::string;

#define print(a) cout<<a<<endl;
#define be(v) v.begin(),v.end()

struct Node{

    Node(){};
    Node(const int value_): value(value_) {}

    std::shared_ptr<Node> parent;
    std::shared_ptr<Node> left_child;
    std::shared_ptr<Node> right_child;

    int value = std::numeric_limits<int>::min();

};

struct BinarySearchTree{
    void insert(int coming_number, std::shared_ptr<Node>& focus_node){
        if (focus_node->value != std::numeric_limits<int>::min()) {
            if (coming_number >= focus_node->value){
                if(!focus_node->right_child){
                    focus_node->right_child = std::make_shared<Node>();
                }
                insert(coming_number,focus_node->right_child);
                focus_node->right_child->parent = focus_node;
            }
            else {
                if (!focus_node->left_child){
                    focus_node->left_child = std::make_shared<Node>();
                }
                insert(coming_number,focus_node->left_child);
                focus_node->left_child->parent = focus_node;
            }
        }
        else {
            focus_node->value = coming_number;
        }
    }
    void print_all(std::shared_ptr<Node>& p, int indent=0){
        if(p) {
            if(p->left_child) print_all(p->left_child, indent+4);
            if(p->right_child) print_all(p->right_child, indent+4);
            if (indent) {
                std::cout << std::setw(indent) << ' ';
            }
            cout<< p->value << "\n ";
        }
    }
    int min(std::shared_ptr<Node>& p){
        if(p->left_child){
            return min(p->left_child);
        }
        return p->value;
    }
    int max(std::shared_ptr<Node>& p){
        if(p->right_child){
            return max(p->right_child);
        }
        return p->value;
    }
    std::shared_ptr<Node> max(std::shared_ptr<Node>& p,bool return_pointer){
        if(p->right_child){
            return max(p->right_child,return_pointer = true);
        }
        return p;
    }
    int pred(std::shared_ptr<Node>& p){
        if (p->left_child){
            return max(p->left_child);
        }
        else{
            std::shared_ptr<Node> q = p;
            while(q->parent){
                if (q->parent->value < p->value){
                    return q->parent->value;
                }
                q = q->parent;
            }
            return std::numeric_limits<int>::min();
        }
    }
    int succ(std::shared_ptr<Node>& p){
        if (p->right_child){
            return min(p->right_child);
        }
        else{
            std::shared_ptr<Node> q = p;
            while(q->parent){
                if (q->parent->value > p->value){
                    return q->parent->value;
                }
                q = q->parent;
            }
            return std::numeric_limits<int>::min();
        }
    }
    void iter(std::shared_ptr<Node>& p){
        if (p->left_child){
            iter(p->left_child);
        }
        cout << p->value << ", ";
        if (p->right_child){
            iter(p->right_child);
        }
    }
    void del(std::shared_ptr<Node>& p){
        if (!p->left_child && !p->right_child){
            p.reset();
        }
        else if(!p->left_child && p->right_child){
            // print("I have right child, but no left child");
            if (p->parent->left_child == p){
                // print(p->value << " is " << p->parent->value << "'s left child");
                std::shared_ptr<Node>* q_parent = &p->parent;
                std::shared_ptr<Node>* q_child = &p->right_child;
                p.reset();
                (*q_parent)->left_child = *q_child;
                (*q_child)->parent = (*q_parent);
            }
            else if (p->parent->right_child == p){
                // print("i'm" << p->parent->value << "'s right child");
                std::shared_ptr<Node>* q_parent = &p->parent;
                std::shared_ptr<Node>* q_child = &p->right_child;
                p.reset();
                (*q_parent)->right_child = *q_child;
                (*q_child)->parent = (*q_parent);
            }
            else{
                print("Error! I'm not anyone's child!");
            }
        }
        else if(p->left_child && !p->right_child){
            // print("I have left child, but no right child");
            if (p->parent->left_child == p){
                // print("i'm" << p->parent->value << "'s left child");
                std::shared_ptr<Node>* q_parent = &p->parent;
                std::shared_ptr<Node>* q_child = &p->left_child;
                p.reset();
                (*q_parent)->left_child = *q_child;
                (*q_child)->parent = (*q_parent);
            }
            else if (p->parent->right_child == p){
                // print("i'm" << p->parent->value << "'s right child");
                std::shared_ptr<Node>* q_parent = &p->parent;
                std::shared_ptr<Node>* q_child = &p->left_child;
                p.reset();
                (*q_parent)->right_child = *q_child;
                (*q_child)->parent = (*q_parent);
            }
            else{
                print("Error! I'm not anyone's child!");
            }
        }
        else{
            std::shared_ptr<Node> q = max(p->left_child,true);
            p->value = q->value;
            del(q);
        }
    }
    int size(std::shared_ptr<Node>& p){
        int sum = 1;
        if (p->left_child){
            sum += size(p->left_child);
        }
        if (p->right_child){
            sum += size(p->right_child);
        }
        if (!p->left_child && !p->right_child){
            return 1;
        }
        return sum;
    }
    int rank(std::shared_ptr<Node>& p,int order){
        int a;
        if (!p->left_child){
            a = 0;   
        }
        else{
            a = size(p->left_child);   
        }

        // print("a: " << a);
        if (a == order-1){
            return p->value;
        }
        else if (a >= order){
            return rank(p->left_child,order);
        }
        else{
            return rank(p->right_child,order-a-1);
        }
    }
    int depth(std::shared_ptr<Node>& p){
        int left = 0;
        int right = 0;
        if (p->left_child){
            left = depth(p->left_child);
        }
        if (p->right_child){
            right = depth(p->right_child);
        }
        return std::max(left,right)+1;
    }

    BinarySearchTree(const int first_value){
        std::shared_ptr<Node> temp_node(new Node(first_value));
        root_node = std::move(temp_node);
    }
    BinarySearchTree(){
        root_node = NULL;
        print("please give root node a value");
    }

    std::shared_ptr<Node> root_node;
    int count = 0;

};


int main(){

    BinarySearchTree bst(1);

    for (int i: {-1,11,-2,3,10,-5,7,-4}){
        bst.insert(i,bst.root_node);
    }

    print("max depth = "<<bst.depth(bst.root_node));

    bst.print_all(bst.root_node);
    print('\n');

    print("min = "<<bst.min(bst.root_node));
    print("max = "<<bst.max(bst.root_node));

    print('\n');
    print("delete "<<bst.root_node->value);
    bst.del(bst.root_node);
    print("new root is: " << bst.root_node->value);
    print('\n');

    print("start to iterate: ")
    bst.iter(bst.root_node);
    print("\n");

    print("total number of node is " << bst.size(bst.root_node));
    print('\n')

    print("predcessor of "<< bst.root_node->right_child->left_child->right_child->left_child->value << \
            " is "<<bst.pred(bst.root_node->right_child->left_child->right_child->left_child));//->left_child->right_child));
    print("successor of 11 is "<<bst.succ(bst.root_node->right_child));

    print("\n");
    for (int i=1 ; i <= bst.size(bst.root_node); ++i){
        cout << "rank:" << i << " is equal to " << bst.rank(bst.root_node,i) << endl;
    }

    return 0;

}

/* example output:
max depth = 5
                -4
             -5
         -2
     -1
                 7
             10
         3
     11
 1


min = -5
max = 11


delete 1
new root is: -1


start to iterate:
-5, -4, -2, -1, 3, 7, 10, 11,

total number of node is 8


predcessor of 7 is 3
successor of 11 is -2147483648


rank:1 is equal to -5
rank:2 is equal to -4
rank:3 is equal to -2
rank:4 is equal to -1
rank:5 is equal to 3
rank:6 is equal to 7
rank:7 is equal to 10
rank:8 is equal to 11
*/