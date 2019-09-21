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
#include "cut_vertex.hpp"

using namespace std;


// in degree and out degree of sparse
void test_s_0() {
    SparseGraph<int, int>* g = GraphFactory::createNewSparse<int, int>(8, true);  // allow duplicate
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
    cout << "----------------------" << endl;
    delete g;
}

void test_s_1() {
    
    auto n0 = Node<string, int>("node0", list<int>{2, 3}, list<int>{1, 2});
    auto n1 = Node<string, int>("node1", list<int>{3, 4}, list<int>{2, 2});
    auto n2 = Node<string, int>("node2", list<int>{1}, list<int>{2});
    auto n3 = Node<string, int>("node3", list<int>{4}, list<int>{2});
    auto n4 = Node<string, int>("node4", list<int>{2, 0}, list<int>{2, 2});
    vector<Node<string, int>> nodes{n0, n1, n2, n3, n4};
    
    auto* g = GraphFactory::createSparse(&nodes, false);  // not allow duplicate
    vector<int> anso = {2, 2, 1, 1, 2};
    vector<int> ansi = {1, 1, 2, 2, 2};
    
    vector<int> vi, vo;
    for(int i = 0; i < g -> getTotalNode(); i++) {
        vi.push_back(g -> getInDegree(i));
        vo.push_back(g -> getOutDegree(i));
    }
    
    if(vo != anso) {
        cout << "getOutDegree() error" << endl;
    } else if(vi != ansi) {
        cout << "getInDegree() error" << endl;
    } else {
        cout << "success for part1." << endl;
    }
    
    g -> addEdge(2, 3);
    
    cout << "Warning is expected:";
    g -> addEdge(1, 4);  // duplicate happens
    
    anso[2] = 2;
    ansi[3] = 3;
    
    vi.clear(); vo.clear();
    for(int i = 0; i < g -> getTotalNode(); i++) {
        vi.push_back(g -> getInDegree(i));
        vo.push_back(g -> getOutDegree(i));
    }
    if(vo != anso) {
        cout << "getOutDegree() error" << endl;
    } else if(vi != ansi) {
        cout << "getInDegree() error" << endl;
    } else {
        cout << "success for part2." << endl;
    }
    
    if(n2.next != list<int>{1}) {
        cout << "node is modified when 'copy' is false" << endl;
    }
    
    cout << "----------------------" << endl;
    delete g;
}


void test_cut_vertex() {
    auto* g = GraphFactory::createNewSparse<int, int>(10);
    vector<pair<int, int>> e{{1, 2}, {2, 3}, {3, 4}, {3, 5},
        {3, 6}, {6, 7}, {7, 8}, {7, 9}, {9, 10}, {1, 6}, {6, 9},
        {6, 10}, {7, 10}
    };
    for(const auto& p: e) {
        g -> addEdge(p.first - 1, p.second - 1);  // vertices indices are from 1
        g -> addEdge(p.second - 1, p.first - 1);
    }
    CutVertexSolver solver;
    cvs_result res = solver.solve(g);
    cout << "The cut vertices are: " << endl;
    for(auto& r: res.cut_vertex_indices) {
        cout << r + 1 << endl;
    }
    delete g;
}

int main(int argc, const char * argv[]) {
//    test_s_0();
//    test_s_1();
    test_cut_vertex();
    
    return 0;
}
