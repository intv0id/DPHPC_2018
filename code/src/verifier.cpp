#include <stdio.h>
#include <iostream>
#include <graph.hpp>

#include <omp.h>

#include "tbb/task_scheduler_init.h"

#include "verifier.hpp"
#include "algorithms/kruskal.hpp"
#include "algorithms/boost_wrapper.hpp"

using namespace std;

Verifier::Verifier(list<mst_algorithm*> l, unsigned int m_threads) : algorithms(l){
    max_threads = m_threads;
}

void Verifier::check(list<Graph*> g_list)
{

    omp_set_dynamic(0);
    srand(42);

    for(mst_algorithm* mst_alg : algorithms){
        mst_algorithm &mst_algo = *mst_alg;

        for (Graph* g : g_list) {
            Graph &graph = *g;

            for (unsigned int thread_nb=1; thread_nb <= max_threads; thread_nb *= 2){
                omp_set_num_threads(thread_nb);
                tbb::task_scheduler_init tbb_scheduler(thread_nb);

                cout << "Algorithm: " << mst_algo.name.c_str();
                cout << "\t; Nodes number:" << graph.n;
                cout << "\t; Max threads: " << thread_nb;

		Graph graph_copy(graph);
                /* Perform the operation */
                l_edge_t mst = mst_algo.algorithm(graph_copy, thread_nb);

                bool passed = verify_one(*g, mst);

                if (passed)
                    cout << "\t; Correct result" << endl;
                else
                    cout << "\t; Failed" << endl;

            }
        }
    }
}

int Verifier::getSum(l_edge_t &mst){
    int c = 0;
    for(l_edge_it ite = mst.begin(); ite != mst.end(); ite++){
        c += (*ite)->weight;
    }   
	return c;
}

bool Verifier::isEveryNodeReachable(int n, l_edge_t &mst) {
    bool* reached = new bool[n];
    reached[0] = true;
    int new_node = 1;
    while (new_node > 0) {
        new_node = 0;
        for (l_edge_it ite = mst.begin(); ite != mst.end(); ite++) {
            int u = (*ite)->source;
            int v = (*ite)->target;
            if (reached[u] && !reached[v]) {
                new_node++;
                reached[v] = true;
            }
            else if (reached[v] && !reached[u]) {
                new_node++;
                reached[u] = true;
            }
        }
    }

    bool reachable = true;
    for (int i = 0; i < n; i++) {
        reachable = reachable && reached[i];
    }

    return reachable;
}

bool Verifier::verify_one(Graph &g, l_edge_t &solution) {
    bool verified = true;

    boost_wrapper s;
    l_edge_t other_solution = s.algorithm(g);

    // 1. Is it spanning?
    verified = verified && isEveryNodeReachable(g.n, solution);
    
    // 2. Is it minimum?
    verified = verified && (getSum(solution) == getSum(other_solution));

    return verified;
}
