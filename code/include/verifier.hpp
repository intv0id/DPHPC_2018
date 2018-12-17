#include "graph.hpp"
#include "common.hpp"

#include <vector>

using namespace std;



class Verifier{
    vector<unsigned int> threads;
    const list<mst_algorithm*> algorithms;
public:
    Verifier(list<mst_algorithm*> l, vector<unsigned int> threads_vec);
    void check(list<Graph*> g_list);
    bool isEveryNodeReachable(int n, l_edge_t &mst);
    int getSum(l_edge_t &solution);
    bool verify_one(Graph &g, l_edge_t &solution);
    ~Verifier(){}
};