#include <stdio.h>
#include <iostream>
#include <graph.hpp>
#include <omp.h>
#include "tbb/task_scheduler_init.h"

#include "verifier.hpp"
#include "algorithms/boost_kruskal.hpp"
#include "algorithms/filter_kruskal.hpp"

using namespace std;

Verifier::Verifier(list<mst_algorithm*> l, vector<unsigned int> threads_vec) : algorithms(l){
    threads = threads_vec;
}

void Verifier::check(list<Graph*> g_list)
{

    omp_set_dynamic(0);
    srand(42);

    for(mst_algorithm* mst_alg : algorithms){
        mst_algorithm &mst_algo = *mst_alg;

        for (Graph* g : g_list) {
            Graph &graph = *g;

            for (unsigned int thread_nb: threads){
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

    for (l_edge_it ite = mst.begin(); ite != mst.end(); ite++) {
        int u = (*ite)->source;
        int v = (*ite)->target;
        if (u == 0 || u == 11 || v == 0 || v == 11) {
            // cout << "Edge: " << u << " -> " << v << endl;
        }
        // cout << "Edge: " << u << " -> " << v  << " : " << (*ite)->weight << endl;
    }


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

    for (int i = 0; i < n; i++) {
        if (!reached[i]) {
            cout << "Vertex " << i << " is not reachable" << endl;
            return false;
        }
    }

    return true;
}

bool Verifier::verify_one(Graph &g, l_edge_t &solution) {

    // boost_kruskal s;
    filter_kruskal s;
    l_edge_t other_solution = s.algorithm(g);

    // 1. Is it spanning?
    
    bool valid = true;

    cout << endl;

    if (! isEveryNodeReachable(g.n, solution)) {
        cout << "Error: not connected" << endl; 
        valid = false;
    }

    // 2. Is it minimum?
    if (getSum(solution) != getSum(other_solution)) {
        cout << "Error: different sum weights" << endl;
        cout << getSum(solution) << " compared to " << getSum(other_solution) << endl;
        valid = false;
    }

    return valid;
}
