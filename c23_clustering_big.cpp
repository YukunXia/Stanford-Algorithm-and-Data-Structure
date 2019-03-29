#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <unordered_map>
#include <limits>
#include <sstream>
#include <string>
#include <cmath>

using std::cout;
using std::endl;
using std::cin;
using std::vector;
using std::string;

#define print(a) cout<<a<<endl;
#define be(v) v.begin(),v.end()

struct Node{
    Node (){}
    Node (const int num,const std::shared_ptr<vector<int>>& gene_): self_num(num),gene(gene_) {}
    int self_num;
    std::shared_ptr<vector<int>> gene;
    vector<std::shared_ptr<Node>> point_in;
    std::shared_ptr<Node> point_out;
};

std::bitset<24> line_to_gene(string binary_str){
    std::stringstream stream(binary_str);
    int binary_section;
    int count = 0;
    int gene = 0;
    while(stream >> binary_section){
        gene += binary_section*pow(2,count);
        ++count;
    }
    return std::bitset<24>(gene);
}

int main(){

    std::unordered_map<std::bitset<24>,int> gene_to_num;
    std::unordered_map<std::bitset<24>,std::bitset<24>> gene_to_head;
    std::vector<std::bitset<24>> num_to_gene;

    freopen("clustering_big.txt","r",stdin);
    int num_of_nodes = 0;
    int bits_of_nodes = 0;
    cin >> num_of_nodes >> bits_of_nodes;
    print(num_of_nodes << ", " << bits_of_nodes);
    

    string line;
    int count = 0;
    std::getline(std::cin,line);
    while (std::getline(std::cin,line)){
        std::bitset<24> gene = line_to_gene(line);
        if (gene_to_num.find(gene) != gene_to_num.end()){
            --num_of_nodes;
            continue;
        }
        gene_to_num[gene] = count;
        gene_to_head[gene] = gene;
        num_to_gene.emplace_back(gene);

        ++count;
    }
    print("removed identical gene, and now: " << num_of_nodes);

    for (auto i=num_to_gene.begin(); i<num_to_gene.end(); ++i){
        std::bitset<24> head_of_star_i = *i;
        while(gene_to_head[head_of_star_i] != head_of_star_i){
            head_of_star_i = gene_to_head[head_of_star_i];
            // print("up!");
        }
        for (int k=0; k<bits_of_nodes; k++){
            std::bitset<24> temp_gene = *i;
            temp_gene.flip(k);
            if (gene_to_num.find(temp_gene) != gene_to_num.end()){
                while(gene_to_head[temp_gene] != temp_gene){
                    temp_gene = gene_to_head[temp_gene];
                    // print("up!");
                }
                if (temp_gene != head_of_star_i){
                    gene_to_head[temp_gene] = head_of_star_i;
                    --num_of_nodes;
                }
            }
        }
    }
    print("removed gene distance=1, and now: " << num_of_nodes);

    for (auto i=num_to_gene.begin(); i<num_to_gene.end(); ++i){
        std::bitset<24> head_of_star_i = *i;
        while(gene_to_head[head_of_star_i] != head_of_star_i){
            head_of_star_i = gene_to_head[head_of_star_i];
            // print("up!");
        }
        for (int j=0; j<bits_of_nodes-1; ++j){
            for (int k=j+1; k<bits_of_nodes; ++k){
                std::bitset<24> temp_gene = *i;
                temp_gene.flip(j);
                temp_gene.flip(k);
                if (gene_to_num.find(temp_gene) != gene_to_num.end()){
                    std::bitset<24> head_of_temp = temp_gene;
                    while(gene_to_head[head_of_temp] != head_of_temp){
                        head_of_temp = gene_to_head[head_of_temp];
                        // print("up!");
                    }
                    if (head_of_temp != head_of_star_i){
                        gene_to_head[head_of_temp] = head_of_star_i;
                        --num_of_nodes;
                        // print(num_of_nodes);
                    }
                }
            }
        }
    }

    print("removed gene distance=2, and now: " << num_of_nodes);
}

/* typical outputs:

6118 (This program is so slow. Any method to accelerate it or any better solution?)

*/