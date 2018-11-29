#include "graph.hpp"
#include "common.hpp"
#include <vector>

using namespace std;

bool compare(const edge* a, const edge* b);

l_edge_t kruskal_main(vector<edge*> &edges, union_find* u_find);

class kruskal : public mst_algorithm {
	public:
	kruskal() : mst_algorithm("Sequential_Kruskal") {}
	virtual l_edge_t algorithm(Graph &g, unsigned int n_threads = 1);
};
