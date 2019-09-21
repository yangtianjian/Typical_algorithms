//
//  main.cpp
//  graph_algorithms
//
//  Created by apple on 2019/9/19.
//  Copyright © 2019 apple. All rights reserved.
//

#include <iostream>
#include <vector>
#include <map>
#include <utility>

#include "graph.hpp"

using namespace std;


// in degree and out degree of sparse
void test_s_0() {
    SparseGraph<int, int>* g = GraphFactory::createNewSparse<int, int>(8, false);  // allow duplicate
    g -> addEdge(0, 3);
    g -> addEdge(1, 4);
    g -> addEdge(3, 4);
    g -> addEdge(4, 7);
    g -> addEdge(4, 4);
    g -> addEdge(0, 3);
    vector<int> vo, vi;
    for(int i = 0; i < g -> getTotalNode(); i++) {
        vo.push_back(g -> getOutDegree(i));
        vi.push_back(g -> getInDegree(i));
    }
    vector<int> anso{2, 1, 0, 1, 2, 0, 0, 0};
    vector<int> ansi{0, 0, 0, 2, 3, 0, 0, 1};
    if(vo != anso) {
        cout << "getOutDegree() error" << endl;
    } else if(vi != ansi) {
        cout << "getInDegree() error" << endl;
    } else {
        cout << "success." << endl;
    }
    delete g;
}

int main(int argc, const char * argv[]) {
    test_s_0();
    return 0;
}
