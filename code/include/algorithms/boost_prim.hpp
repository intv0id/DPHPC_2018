#include "graph.hpp"
#include "common.hpp"
#include <vector>

#include <boost/graph/prim_min_spanning_tree.hpp>

typedef boost::graph_traits<Boost_Graph>::edge_descriptor Boost_Edge;
using namespace std;

class boost_prim : public mst_algorithm {
	public:
	boost_prim() : mst_algorithm("Boost_Prim") {}
	virtual l_edge_t algorithm(Graph &g, unsigned int n_threads = 1);
};
