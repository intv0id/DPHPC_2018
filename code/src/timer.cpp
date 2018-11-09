#include <iostream>
#include <fstream>

#include <omp.h>

#include "graph.hpp"
#include "timer.hpp"

using namespace std;

Timer::Timer(string name) 
{
	o.open(name);
}

void Timer::printF(int n){
	o << n << endl;
}

void Timer::time(l_edge_t fonction(Graph&), Graph& g){
		double time1 = omp_get_wtime();
		l_edge_t mst = fonction(g);
		double time2 = omp_get_wtime();
		
		o << time2 - time1 << " ";

		int weight(0);
		for(l_edge_it it = mst.begin(); it != mst.end(); it++){
			weight += (*it)->weight;
		}
		cout << "Weight " << weight << endl;

};
