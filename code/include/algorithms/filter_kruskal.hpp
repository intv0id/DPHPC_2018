#include "graph.hpp"
#include "common.hpp"
#include <vector>

using namespace std;

int find_pivot(vector<edge*> &edges);
vector<edge*> filter(vector<edge*> &edges, union_find *u_find);
pair<vector<edge*>, vector<edge*>> partition(vector<edge*> &edges, int pivot);
l_edge_t filter_kruskal_main(Graph &g, vector<edge*> &edges, union_find *u, unsigned long* old_size);

class filter_kruskal : public mst_algorithm {
	public:
	filter_kruskal() : mst_algorithm("Filter Kruskal") {}
	virtual l_edge_t algorithm(Graph &g);
};
