#include <stdio.h>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <stdlib.h> 
#include <iostream>

#include "algorithms/filter_kruskal.hpp"
#include "algorithms/kruskal.hpp"
#include "tbb/parallel_sort.h"
#include "common.hpp"

#include "pstl/execution"
#include "pstl/algorithm"

#include <sys/time.h>
#include "omp.h"

using namespace std;

l_edge_t filter_kruskal::algorithm(Graph &g, unsigned int n_threads) {

    struct timeval t0, t1, t2;

    // srand(42);
    l_edge_t result;

    gettimeofday(&t0, NULL);

    // vector<edge*> edges {g.unique_edges.begin(), g.unique_edges.end()};
    vector<edge*> edges = g.unique_edges_vector;
    gettimeofday(&t1, NULL);

    union_find* u_find = new union_find(g.n);
    gettimeofday(&t2, NULL);

    unsigned long x = 0;
    unsigned long* old_size = &x;

    int n_nodes = g.n;
    int n_edges = g.n_edges;

    #ifdef DEBUG
    cout << n_nodes << ", " << n_edges << endl;
    cout << "Init time: " << getTime(t1, t0) << ", " << getTime(t2, t1) << endl;
    #endif

    auto result_mst = filter_kruskal_main(g, edges, u_find, old_size, n_nodes, n_edges, edges.begin(), edges.end());
    delete u_find;
    return result_mst;
}

double getTime(struct timeval end, struct timeval start) {
    return ((end.tv_sec  - start.tv_sec) * 1000000u + 
         end.tv_usec - start.tv_usec) / 1.e6;
}

l_edge_t filter_kruskal_main(Graph &g, vector<edge*> &edges, union_find *u, unsigned long* old_size,
                             int n_nodes, int n_edges, vector<edge*>::iterator start, vector<edge*>::iterator end) {
    // start = edges.begin();
    // end = edges.end();

    int size = end - start;
    int old = (*old_size);

    if (size < 5000 || (*old_size) == size) {
        tbb::parallel_sort(start, end, compare);
        return kruskal_main(start, end, u);
    }
    else {
        
        int old = *old_size;

        (*old_size) = size;

        struct timeval t0, t1, t2, t3, t4, t5, t6;

        gettimeofday(&t0, NULL);

        int pivot = find_pivot(start, end, n_nodes, n_edges);
        gettimeofday(&t1, NULL);

        auto middle = partition(start, end, pivot);
        gettimeofday(&t2, NULL);

        l_edge_t partial_solution = filter_kruskal_main(g, edges, u, old_size, n_nodes, n_edges, start, middle);
        gettimeofday(&t3, NULL);

        if (end - middle > 0) {

            auto new_end = filter(middle, end, u);
            gettimeofday(&t4, NULL);

            if (new_end - middle > 0) {

                l_edge_t other_solution = filter_kruskal_main(g, edges, u, old_size, n_nodes, n_edges, middle, new_end);
                gettimeofday(&t5, NULL);

                partial_solution.splice(partial_solution.end(), other_solution);
                gettimeofday(&t6, NULL);

                #ifdef DEBUG
                if (old == 0) {

                    cout << "Find pivot: " << getTime(t1, t0) << endl;
                    cout << "Partition: " << getTime(t2, t1) << endl;
                    cout << "Appel recursif: " << getTime(t3, t2) << endl;
                    cout << "Filter: " << getTime(t4, t3) << endl;
                    cout << "Appel recursif 2: " << getTime(t5, t4) << endl;
                    cout << "Merge: " << getTime(t6, t5) << endl;
                }
                #endif
            }
        }

        return partial_solution;
    }
}

int find_pivot(vector<edge*>::iterator start, vector<edge*>::iterator end, int n_nodes, int n_edges) {

    int n = end - start;
    int n_samples = 512;

    vector<int> values(n_samples);

    #pragma omp parallel
    {
        int n_threads = omp_get_num_threads();
        int ID = omp_get_thread_num();
        int per_thread = n_samples / n_threads;
        int offset = per_thread * ID;

        for (int i = 0; i < per_thread; i++) {
            values[offset + i] = (*(start + (rand() % n)))->weight;
        }

    }

    tbb::parallel_sort(values.begin(), values.end());

    return values[n_samples / 2];
}

vector<edge*>::iterator filter(vector<edge*>::iterator start, vector<edge*>::iterator end, union_find *u_find) {

    return partition(pstl::execution::par,
                     start, end,
                     [u_find](edge* e) {
                     return u_find->find(e->source) != u_find->find(e->target);
                     });
}

vector<edge*>::iterator partition(vector<edge*>::iterator start, vector<edge*>::iterator end, int pivot) {

    return partition(pstl::execution::par,
                     start, end,
                     [pivot](edge* e) {return e->weight <= pivot;});
}
