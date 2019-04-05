#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;

#define print(a) cout<<a<<endl;
#define be(v) v.begin(),v.end()

struct pair_hash
{
	template <class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2> &pair) const
	{
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};

int gene_alignment(string gene1, string gene2, int& p_gap, int& p_xy,\
                    unordered_map<std::pair<int,int>,int,pair_hash>& history){
                    //  use string::iterator for more efficient implementation
    int g1_len = gene1.length();
    int g2_len = gene2.length();
    if (g1_len*g2_len == 0){
        return std::max(g1_len,g2_len)*p_gap;
    }
    else{
        std::pair<int,int> pij1 = std::make_pair(g1_len,g2_len-1);
        std::pair<int,int> pi1j1 = std::make_pair(g1_len-1,g2_len-1);
        std::pair<int,int> pi1j = std::make_pair(g1_len-1,g2_len);
        if (history.find(pij1) == history.end()){
            history[pij1] = gene_alignment(gene1,gene2.substr(0,g2_len-1),p_gap,p_xy,history);
        }
        if (history.find(pi1j1) == history.end()){
            history[pi1j1] = gene_alignment(gene1.substr(0,g1_len-1),gene2.substr(0,g2_len-1),p_gap,p_xy,history);
        }
        if (history.find(pi1j) == history.end()){
            history[pi1j] = gene_alignment(gene1.substr(0,g1_len-1),gene2,p_gap,p_xy,history);
        }
        bool flag = (gene1.back() != gene2.back());
        return std::min({history[pij1]+p_gap, history[pi1j]+p_gap, history[pi1j1]+flag*p_xy});
    }
}

int main(){

    // string gene1 = "AGGGCT";
    // string gene2 = "AGGCA";
    // int p_gap = 3;
    // int p_xy = 2; 
    // // result = 5

    // string gene1 = "CG";
    // string gene2 = "CA";
    // int p_gap = 3;
    // int p_xy = 7;
    // // result = 6

    string gene1 = "CG";
    string gene2 = "CA";
    int p_gap = 3;
    int p_xy = 5;
    // result = 5

    unordered_map<std::pair<int,int>,int,pair_hash> history;
    print(gene_alignment(gene1,gene2,p_gap,p_xy,history));

    return 0;

}