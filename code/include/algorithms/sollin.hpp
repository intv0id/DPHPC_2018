#include "graph.hpp"
#include "common.hpp"

#include <vector>

using namespace std;

class sollin : public mst_algorithm {
	public:
	virtual l_edge_t algorithm(Graph& g);
	sollin() : mst_algorithm("Sollin"){}
};
