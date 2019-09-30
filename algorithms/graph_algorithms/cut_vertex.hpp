//
//  cut_vertex.hpp
//  graph_algorithms
//
//  Created by apple on 2019/9/21.
//  Copyright © 2019 apple. All rights reserved.
//

#ifndef cut_vertex_hpp
#define cut_vertex_hpp

#include <stdio.h>
#include <set>
#include <stack>

#include "graph.hpp"

using namespace std;

class cvs_result {
public:
    set<int> cut_vertex_indices;
};

class CutVertexSolver {

private:
    vector<int> dfn;
    vector<int> low;
    set<int> ans;
    stack<pair<int, int>> stk;
    
    void _dfnl(Graph* g, int dep, int cur_node, int prev_node);
    
public:
    
    cvs_result solve(Graph* g);
};

#endif /* cut_vertex_hpp */
