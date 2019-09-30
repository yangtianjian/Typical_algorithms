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
#include <fstream>
#include <cstdlib>

#include "graph.hpp"
#include "cut_vertex.hpp"
#include "Sorting.hpp"

#define PATH "/Users/apple/XcodeProject/Typical_algorithms/result_and_thesis/"

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


void test_cut_vertex(vector<pair<int, int>>& e) {
    auto* g = GraphFactory::createNewSparse<int, int>(10);
    
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

void batch_cut_vertex() {
    vector<pair<int, int>> e1{{1, 2}, {2, 3}, {3, 4}, {3, 5},
        {3, 6}, {6, 7}, {7, 8}, {7, 9}, {9, 10}, {1, 6}, {6, 9},
        {6, 10}, {7, 10}
    };
    test_cut_vertex(e1);
    
//    vector<pair<int, int>> e2{{2, 5}, {2, 4}, {3, 4}, {3, 6}, {6, 2}, {2, 1}, {2, 3}};
//    test_cut_vertex(e2);
}

void get_random(int n, vector<int>& x) {
    for(int i = 0; i < n; i++) {
        x[i] = rand();
    }
}

void get_reverse(int n, vector<int>& a) {
    get_random(n, a);
    sort(a.begin(), a.end(), greater<int>());
}


void test_mergesort() {
    vector<int> x;
    
    ofstream out1(string(PATH) + string("mergesort_result_random.tsv"));
    ofstream out2(string(PATH) + string("mergesort_result_reverse.tsv"));
    
    out1 << "N" << "\t" << "Time(ms)" << endl;
    out2 << "N" << "\t" << "Time(ms)" << endl;
    
    for(int n = 100; n <= 1000; n += 100) {
        out1 << n << "\t";
        x = vector<int>(n, 0);
        get_random(n, x);
        clock_t t0 = clock();
        Sort<int>::runMergeSortL(x);
        clock_t t1 = clock();
        out1 << (t1 - t0) / (double)CLOCKS_PER_SEC * 1000.0<< endl;
    }
    
    for(int n = 100; n <= 1000; n += 100) {
        out2 << n << "\t";
        x = vector<int>(n, 0);
        get_reverse(n, x);
        clock_t t0 = clock();
        Sort<int>::runMergeSortL(x);
        clock_t t1 = clock();
        out2 << (t1 - t0) / (double)CLOCKS_PER_SEC * 1000.0<< endl;
    }
    
    out1.close();
    out2.close();
}

void test_quicksort() {
    vector<int> x;
    
    ofstream out1(string(PATH) + string("quicksort_result_random.tsv"));
    ofstream out2(string(PATH) + string("quicksort_result_reverse.tsv"));
    
    out1 << "N" << "\t" << "Time(ms)" << endl;
    out2 << "N" << "\t" << "Time(ms)" << endl;
    
    for(int n = 100; n <= 1000; n += 100) {
        out1 << n << "\t";
        x = vector<int>(n, 0);
        get_random(n, x);
        clock_t t0 = clock();
        Sort<int>::runQuickSort(x);
        clock_t t1 = clock();
        out1 << (t1 - t0) / (double)CLOCKS_PER_SEC * 1000.0<< endl;
    }
    
    for(int n = 100; n <= 1000; n += 100) {
        out2 << n << "\t";
        x = vector<int>(n, 0);
        get_reverse(n, x);
        clock_t t0 = clock();
        Sort<int>::runQuickSort(x);
        clock_t t1 = clock();
        out2 << (t1 - t0) / (double)CLOCKS_PER_SEC * 1000.0<< endl;
    }
    
    out1.close();
    out2.close();
}



int main(int argc, const char * argv[]) {
//    test_s_0();
//    test_s_1();
//    batch_cut_vertex();
    
//  Sort<int>::runMergeSortL(x);
    
    test_mergesort();
    test_quicksort();
    
    return 0;
}
