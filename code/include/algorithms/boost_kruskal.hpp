#include "graph.hpp"
#include "common.hpp"
#include <vector>

#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::graph_traits<Boost_Graph>::edge_descriptor Boost_Edge;
using namespace std;

class boost_kruskal : public mst_algorithm {
	public:
	boost_kruskal() : mst_algorithm("Boost_Kruskal") {}
	virtual l_edge_t algorithm(Graph &g, unsigned int n_threads = 1);
};
