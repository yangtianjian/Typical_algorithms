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
    stk = stack<pair<int, int>>();
    dfn.resize(g -> getTotalNode());
    fill(dfn.begin(), dfn.end(), 0);
    low.resize(g -> getTotalNode());
    _dfnl(g, 1, 0, -1);
    cvs_result result;
    result.cut_vertex_indices = ans;
    return result;
}


void CutVertexSolver::_dfnl(Graph* g, int dep, int cur_node, int prev_node) {
    dfn[cur_node] = low[cur_node] = dep;
    list<int> next_cache;
    g -> getNext(cur_node, next_cache);
    
    int n_des = 0;
    pair<int, int> u0;
    for(auto& i: next_cache) {
        if(i != prev_node && dfn[i] < dfn[cur_node]) {
            stk.push({cur_node, i});
        }
        if(dfn[i] == 0) {
            _dfnl(g, dep + 1, i, cur_node);
            if(low[i] >= dfn[cur_node]) {
                cout << "new biconnect component" << endl;
                do {
                    u0 = stk.top(); stk.pop();
                    cout << u0.first + 1 << ", " << u0.second + 1  << endl;
                }while((u0.first == cur_node && u0.second == i) || (u0.first == i && u0.second == cur_node));
            }
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
