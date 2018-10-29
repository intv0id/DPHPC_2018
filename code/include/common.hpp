#include "graph.hpp"
#include <list>
using namespace std;

class union_find {
	public:
        vector<int> parents;
        int size;
        int numTrees;
        union_find(int n);
        int find(int x);
        bool unite(int x, int y);
};

void print_edge_list(l_edge_t);
