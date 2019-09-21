//
//  cut_vertex.cpp
//  graph_algorithms
//
//  Created by apple on 2019/9/21.
//  Copyright © 2019 apple. All rights reserved.
//

#include "cut_vertex.hpp"


cvs_result CutVertexSolver::solve(Graph* g) {
    dfn.clear();
    low.clear();
    ans.clear();
    dfn.resize(g -> getTotalNode());
    fill(dfn.begin(), dfn.end(), 0);
    low.resize(g -> getTotalNode());
    _dfnl(g, 1, 0);
    cvs_result result;
    result.cut_vertex_indices = ans;
    return result;
}


void CutVertexSolver::_dfnl(Graph* g, int dep, int cur_node) {
    dfn[cur_node] = low[cur_node] = dep;
    list<int> next_cache;
    g -> getNext(cur_node, next_cache);
    
    int n_des = 0;
    for(auto& i: next_cache) {
        if(dfn[i] == 0) {
            _dfnl(g, dep + 1, i);
            low[cur_node] = min(low[cur_node], low[i]);
            n_des++;
        } else {
            low[cur_node] = min(low[cur_node], dfn[i]);
        }
        if(low[i] >= dfn[cur_node] && dep > 1) {
            ans.insert(cur_node);
        }
    }
    if(dep == 1 && n_des >= 2) {    // special case for root node
        ans.insert(cur_node);
    }
}
