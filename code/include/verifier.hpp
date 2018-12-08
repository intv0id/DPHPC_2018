#include "graph.hpp"
#include "common.hpp"

#include <vector>

using namespace std;



class Verifier{
    unsigned int max_threads;
    const list<mst_algorithm*> algorithms;
public:
    Verifier(list<mst_algorithm*> l, unsigned int m_threads);
    void check(list<Graph*> g_list);
    bool isEveryNodeReachable(int n, l_edge_t &mst);
    int getSum(l_edge_t &solution);
    bool verify_one(Graph &g, l_edge_t &solution);
    ~Verifier(){}
};