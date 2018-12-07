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
    
    cout << n_nodes << ", " << n_edges << endl;

    cout << "Init time: " << getTime(t1, t0) << ", " << getTime(t2, t1) << endl;

    return filter_kruskal_main(g, edges, u_find, old_size, n_nodes, n_edges);
}

double getTime(struct timeval end, struct timeval start) {
    return ((end.tv_sec  - start.tv_sec) * 1000000u + 
         end.tv_usec - start.tv_usec) / 1.e6;
}

l_edge_t filter_kruskal_main(Graph &g, vector<edge*> &edges, union_find *u, unsigned long* old_size, int n_nodes, int n_edges) {

    if (edges.size() < 5000 || (*old_size) == edges.size() ) {
        struct timeval t0, t1;
        gettimeofday(&t0, NULL);
        tbb::parallel_sort(edges.begin(), edges.end(), compare);
        gettimeofday(&t1, NULL);
        // cout << "Sort " << edges.size() << ", " << getTime(t1, t0) << endl;
        return kruskal_main(edges, u);
    }
    else {
        
        int old = *old_size;

        (*old_size) = edges.size();

        struct timeval t0, t1, t2, t3, t4, t5, t6;

        gettimeofday(&t0, NULL);

        int pivot = find_pivot(edges, n_nodes, n_edges);
        gettimeofday(&t1, NULL);

        auto couple = partition(edges, pivot);
        gettimeofday(&t2, NULL);

        l_edge_t partial_solution = filter_kruskal_main(g, couple.first, u, old_size, n_nodes, n_edges);
        gettimeofday(&t3, NULL);

        auto e_plus = filter(couple.second, u);
        gettimeofday(&t4, NULL);

        l_edge_t other_solution = filter_kruskal_main(g, e_plus, u, old_size, n_nodes, n_edges);
        gettimeofday(&t5, NULL);

        partial_solution.splice(partial_solution.end(), other_solution);
        gettimeofday(&t6, NULL);

        if (old == 0) {
            cout << "Find pivot: " << getTime(t1, t0) << endl;
            cout << "Partition: " << getTime(t2, t1) << endl;
            cout << "Appel recursif: " << getTime(t3, t2) << endl;
            cout << "Filter: " << getTime(t4, t3) << endl;
            cout << "Appel recursif 2: " << getTime(t5, t4) << endl;
            cout << "Merge: " << getTime(t6, t5) << endl;
        }

        return partial_solution;
    }
}

int getMedian(vector<int> &values) {

    tbb::parallel_sort(values.begin(), values.end());

    int n = values.size();

    if (n % 2 == 0) {
        int m = n / 2;
        return (values[m - 1] + values[m]) / 2;
    }
    else {
        return values[(n - 1) / 2];
    }

}

int find_pivot(vector<edge*> &edges, int n_nodes, int n_edges) {

    int n = edges.size();
    int n_samples = 128;

    vector<int> values(n_samples);

    #pragma omp parallel
    {
        int n_threads = omp_get_num_threads();
        int ID = omp_get_thread_num();
        int per_thread = n_samples / n_threads;
        int offset = per_thread * ID;

        for (int i = 0; i < per_thread; i++) {
            values[offset + i] = edges[rand() % n]->weight;
        }

    }

    tbb::parallel_sort(values.begin(), values.end());

    // return getMedian(values);
    int index = n_samples * n_nodes / n_edges;
    index = min(n_samples, index);
    return getMedian(values);
}

vector<edge*> filter(vector<edge*> &edges, union_find *u_find) {

    vector<edge*> filtered (edges.size());

    auto it = copy_if (pstl::execution::par,
    // auto it = copy_if (
            edges.begin(), edges.end(), filtered.begin(),
            [u_find](edge* e) {
            return u_find->find(e->source) != u_find->find(e->target);
            });

    filtered.resize(distance(filtered.begin(), it));

    return filtered;
}

pair<vector<edge*>, vector<edge*>> partition(vector<edge*> &edges, int pivot) {

    vector<edge*> e_minus = vector<edge*>(edges.size());
    vector<edge*> e_plus = vector<edge*>(edges.size());

    struct timeval t0, t1;

    gettimeofday(&t0, NULL);

    auto pair = partition_copy(pstl::execution::par,
                    edges.begin(), edges.end(),
                    e_minus.begin(), e_plus.begin(),
                    [pivot](edge* e) {return e->weight <= pivot;});

    gettimeofday(&t1, NULL);

    cout << "PARTITION_COPY: " << getTime(t1, t0) << endl;

    e_minus.resize(distance(e_minus.begin(), pair.first));
    e_plus.resize(distance(e_plus.begin(), pair.second));

    return make_pair(e_minus, e_plus);
}

pair<vector<edge*>, vector<edge*>> old_partition(vector<edge*> &edges, int pivot) {
    
    vector<edge*> e_minus = vector<edge*>(edges.size());
    vector<edge*> e_plus = vector<edge*>(edges.size());

    auto it_minus = copy_if (pstl::execution::par,
            edges.begin(), edges.end(), e_minus.begin(),
            [pivot](edge* e) {return e->weight < pivot;});

    e_minus.resize(distance(e_minus.begin(), it_minus));

    auto it_plus = copy_if (pstl::execution::par,
            edges.begin(), edges.end(), e_plus.begin(),
            [pivot](edge* e) {return e->weight >= pivot;});
    
    e_plus.resize(distance(e_plus.begin(), it_plus));

    return make_pair(e_minus, e_plus);

}
