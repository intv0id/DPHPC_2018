#include <stdio.h>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <iostream>

#include "algorithms/boost_wrapper.hpp"
#include <boost/graph/kruskal_min_spanning_tree.hpp>
// #include <boost/graph/prim_minimum_spanning_tree.hpp>

#include <sys/time.h>
#include "algorithms/filter_kruskal.hpp"

using namespace std;

l_edge_t boost_wrapper::algorithm(Graph &g, unsigned int n_threads) {

    boost::property_map<Boost_Graph,boost::edge_weight_t>::type w = get(boost::edge_weight, g.boost_rep);
    vector<Boost_Edge> v;

    struct timeval t0, t1;
    gettimeofday(&t0, NULL);

	kruskal_minimum_spanning_tree(g.boost_rep, back_inserter(v));
    gettimeofday(&t1, NULL);

    std::cout << "BOOST: " << getTime(t1, t0) << std::endl;

	l_edge_t result;

	for (vector<Boost_Edge>::iterator it = v.begin(); it != v.end(); it++) {
        // result.push_back(g.edges[]
        // weight_kruskal_boost += w[*it];
    }

    return result;
}