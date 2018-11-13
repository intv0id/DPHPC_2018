#include "graph.hpp"
#include "common.hpp"

#include <vector>

using namespace std;

class parallel_sollin_AL : public mst_algorithm{
	public:
	virtual l_edge_t algorithm(Graph& g);
	parallel_sollin_AL() :mst_algorithm("parallel_sollin_AL"){}
};

class parallel_sollin_EL : public mst_algorithm{
	public :
	virtual l_edge_t algorithm(Graph& g);
	parallel_sollin_EL() :mst_algorithm("parallel_sollin_EL"){}
};
