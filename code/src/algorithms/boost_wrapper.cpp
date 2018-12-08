#include <stdio.h>
#include <algorithm>
#include <map>
#include <cstdlib>

#include "algorithms/boost_wrapper.hpp"
#include <boost/graph/kruskal_min_spanning_tree.hpp>
// #include <boost/graph/prim_minimum_spanning_tree.hpp>

using namespace std;

l_edge_t boost_wrapper::algorithm(Graph &g, unsigned int n_threads) {

    boost::property_map<Boost_Graph,boost::edge_weight_t>::type w = get(boost::edge_weight, g.boost_rep);
    vector<Boost_Edge> v;

	kruskal_minimum_spanning_tree(g.boost_rep, back_inserter(v));

	l_edge_t result;

	for (vector<Boost_Edge>::iterator it = v.begin(); it != v.end(); it++) {
        // result.push_back(g.edges[]
        // weight_kruskal_boost += w[*it];
    }

    return result;
}
