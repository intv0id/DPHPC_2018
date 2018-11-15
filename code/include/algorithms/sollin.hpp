#include "graph.hpp"
#include "common.hpp"

#include <vector>

using namespace std;

class sollin : public mst_algorithm {
	public:
	virtual l_edge_t algorithm(Graph& g, unsigned int n_threads = 1);
	sollin() : mst_algorithm("Sollin"){}
};
