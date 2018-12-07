#include "graph.hpp"
#include "common.hpp"
#include <vector>

using namespace std;

l_edge_t filter_kruskal_main(Graph &g, vector<edge*> &edges, union_find *u, unsigned long* old_size, int n_nodes, int n_edges);

double getTime(struct timeval end, struct timeval start);

int getMedian(vector<int> &values);

int find_pivot(vector<edge*> &edges, int n_nodes, int n_edges);

vector<edge*> filter(vector<edge*> &edges, union_find *u_find);

pair<vector<edge*>, vector<edge*>> partition(vector<edge*> &edges, int pivot);

class filter_kruskal : public mst_algorithm {
	public:
	filter_kruskal() : mst_algorithm("Filter_Kruskal") {}
	virtual l_edge_t algorithm(Graph &g, unsigned int n_threads = 1);
};
