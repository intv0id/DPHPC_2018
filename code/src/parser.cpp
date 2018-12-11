#include <iostream>
#include <mpi.h>

#include "parser.hpp"
#include "timer.hpp"
#include "verifier.hpp"
#include "lsb_timer.hpp"

#include "algorithms/parallel_sollin.hpp"
#include "algorithms/sollin.hpp"
#include "algorithms/kruskal.hpp"
#include "algorithms/filter_kruskal.hpp"
#include "algorithms/prim.hpp"
#include "algorithms/boost_wrapper.hpp"

using namespace std;

parser::parser(int * argc, char ** argv[], int MPI_rank) {
    int nb_nodes;
    int nb_neighbors;
    bool verify = false;

    if (*argc <= 1) goto syntaxerror;
    for (int i = 1; i < *argc; i++) {
        string arg = (*argv)[i];
        if (arg == "--help") {
            print_help(MPI_rank);
            return;
        } else if (arg == "--lsalg") {
            print_algos();
            return;
        } else if (arg == "--verify") {
            verify = true;
        } else if (arg == "--algorithm") {
            while (i + 1 < *argc && parse_algonames((*argv)[i+1])) {i++;}
        } else if (arg == "--USA-graph") {
            if (i + 2 < *argc) {
                selected_graphs.push_back(new Graph((*argv)[i+1], (*argv)[i+2]));
                i+=2;
            } else goto syntaxerror;
        } else if (arg == "--Erdos-Renyi-graph") {
            if (i + 1 < *argc && (nb_nodes = atoi((*argv)[i+1])) > 0) {
                selected_graphs.push_back(new Graph(nb_nodes, (float) edgePerVertex / nb_nodes, minWeight, maxWeight));
                i++;
            } else goto syntaxerror;
        } else if (arg == "--PA-graph") {
            if (i + 2 < *argc && (nb_nodes = atoi((*argv)[i+1])) > 0 && (nb_neighbors = atoi((*argv)[i+2])) > 0) {
                selected_graphs.push_back(new Graph(nb_nodes, nb_neighbors, minWeight, maxWeight));
                i+=2;
            } else goto syntaxerror;
        } else if (arg == "--max-threads") {
            if (i + 1 < *argc && (max_threads = atoi((*argv)[i+1])) > 0) {
                i++;
            } else goto syntaxerror;
        } else if (arg == "--runs") {
            if (i + 1 < *argc && (runs = atoi((*argv)[i+1])) > 0) {
                i++;
            } else goto syntaxerror;
        } else if (arg == "--lsb-filename") {
            if (i + 1 < *argc) {
                lsb_name = (*argv)[i+1];
                i++;
            } else goto syntaxerror;
        } else {
            cout << "Unknown argument: " << arg << endl;
        }
    }
    if ( selected_graphs.size() == 0 || selected_algorithms.size() == 0 ) goto syntaxerror;

    // Parsing success : start measuring
    if (verify)
        check_correctness();
    else
        compute();
    for(auto g : selected_graphs){
	    delete g;
    }
    return;

    syntaxerror:
        print_help(MPI_rank, true);
        return;
}


void parser::print_help(int MPI_rank, bool syntax_error) {
    if (MPI_rank  == 0) {

        if (syntax_error) cout << endl <<"Syntax error."<< endl << endl;

        cout << endl << "USAGE: " << "./bin/exec [args] | --lsalg | --help" << endl;
        cout << endl;

        cout << "Mandatory args" << endl;
        cout << "--algorithm [Algorithm name 1] [Algorithm name 2] ..." << endl;
        cout << "--USA-graph [USA graph name] [USA graph type (d|t)] | --Erdos-Renyi-graph [size (nodes)] | --PA-graph [size (nodes)] [neighbors number]" << endl;
        cout << endl;

        cout << "Optionnal args" << endl;
        cout << "--verify (check for correctness instead of timing)" << endl;
        cout << "--max-threads [Maximum number of thread (preferably a power of 2)]" << endl <<"DEFAULT=1" << endl;
        cout << "--runs [Number of measurement for each parameters set]" << endl <<"DEFAULT=5" << endl;
        cout << "--lsb-filename [LSB filename]" << endl << "DEFAULT=\"measure\"" << endl;
        cout << endl ;
    }
}

bool parser::parse_algonames(string algoname) {
    if (algoname == "Sollin") {
        selected_algorithms.push_back(new sollin());
    } else if (algoname == "ParallelSollinEL") {
        selected_algorithms.push_back(new parallel_sollin_EL());
    } else if (algoname == "ParallelSollinAL") {
        selected_algorithms.push_back(new parallel_sollin_AL());
    } else if (algoname == "ParallelSollinFAL") {
        selected_algorithms.push_back(new parallel_sollin_FAL());
    } else if (algoname == "Kruskal") {
        selected_algorithms.push_back(new kruskal());
    } else if (algoname == "FilterKruskal") {
        selected_algorithms.push_back(new filter_kruskal());
    } else if (algoname == "BoostKruskal") {
        selected_algorithms.push_back(new boost_wrapper());
    } else {
        return false;
    }
    return true;
}

void parser::print_algos(){
    cout << endl << "Available algorithms:" << endl << endl;
    cout << "Sollin" << endl;
    cout << "ParallelSollinEL" << endl;
    cout << "ParallelSollinAL" << endl;
    cout << "Kruskal" << endl;
    cout << "FilterKruskal" << endl;
    cout << "BoostKruskal" << endl;
}

void parser::compute(){
    // Create timer
    LsbTimer t(selected_algorithms, lsb_name, max_threads, runs);

    // Time
    t.clock(selected_graphs);
}

void parser::check_correctness(){
    Verifier v(selected_algorithms, max_threads);
    v.check(selected_graphs);
}
