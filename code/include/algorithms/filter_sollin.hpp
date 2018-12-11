#include "graph.hpp"
#include "common.hpp"

#include <vector>

using namespace std;

class filter_sollin : public mst_algorithm{
	public:
	virtual l_edge_t algorithm(Graph& g, unsigned int n_threads = 1);
	filter_sollin():mst_algorithm("filter_sollin"){}
	l_edge_t main_func(Graph&,
			unsigned int,
			vector<component_FAL_vector*>&,
			const vector<v_edge_it>,
			const vector<v_edge_it>,
			union_find*,
			compVertexFALvec,
			compWeight);
	l_edge_t base_func(Graph&, 
			unsigned int,
			vector<component_FAL_vector*>&,
			vector<vector<edge*>::iterator>,
			const vector<vector<edge*>::iterator>,
			union_find*,
			compVertexFALvec,
			compWeight);
};
