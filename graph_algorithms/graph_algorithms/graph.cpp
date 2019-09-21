//
//  graph.cpp
//  graph_algorithms
//
//  Created by apple on 2019/9/19.
//  Copyright © 2019 apple. All rights reserved.
//

#include "graph.hpp"


// The implementation of sparse graph is in hpp file because sparse graph is a template class


DenseGraph::DenseGraph(int total_node,
           int not_connected_const,
           bool calculate_edge,
           bool suppress_warnings): DenseGraph() {
    _dynamic_mode = true;
    _suppress_warnings = suppress_warnings;
    _total_node = total_node;
    _not_connected = not_connected_const;
    // In dynamic mode, the edge will be added within the track of the class. There won't be a high cost to get total edges.
    if(calculate_edge) {
        _total_edge = 0;
    }
    _matrix = new vector<vector<int>>();
    _matrix_is_copied = true;  // In dynamic mode, the memory will be in the charge of us.
}

DenseGraph::DenseGraph(vector<vector<int>>* matrix,
           int not_connected_const,
           bool calculate_edge,
           bool copy_matrix,
           bool suppress_warnings): DenseGraph() {
    _dynamic_mode = false;
    _suppress_warnings = suppress_warnings;
    _total_node = matrix -> size();
    _not_connected = not_connected_const;
    if(calculate_edge) {
        _total_edge = 0;
        for(int i = 0; i < _total_node; i++) {
            for(int j = 0; j < _total_node; j++) {
                if(mat(i, j) != _not_connected) _total_edge++;
            }
        }
    }
    if(copy_matrix) {
        _matrix_is_copied = true;
        _matrix = new vector<vector<int>>();
        *(_matrix) = *(matrix);   // to make a copy
    } else {
        _matrix_is_copied = false;
        _matrix = matrix;
    }
}

void DenseGraph::addEdge(int node_index_u, int node_index_v, int value) {
    int old_value = mat(node_index_u, node_index_v);
    
    if(!_suppress_warnings) {
        if(!_matrix_is_copied) {
            cout << "WARNING: In static mode when the matrix is not copied. The add edge operation will affect the input adjacent matrix! [Use suppress_warnings = true to disable this warning.]" << endl;
        }
        if(value == _not_connected) {
            cout << "WARNING: The value of the edge is the same as the const not_connected. Thus the edge will not be added logically." << endl;
        }
        if(old_value != _not_connected) {
            cout << "WARNING: The value of the edge is not \"not_connected\" before adding edge. Plus, the adjacent matrix doesn't support duplicate edges. The value of the edge will be refreshed." << endl;
        }
    }
    
    mat(node_index_u, node_index_v) = value;
    
    if(_total_edge != INVALID && old_value == _not_connected) {
        _total_edge++;
    }
}

void DenseGraph::addEdgeFast(int node_index_u, int node_index_v, int value) {
    if(_total_edge != INVALID && mat(node_index_u, node_index_v) == _not_connected) {
        _total_edge++;
    }
    mat(node_index_u, node_index_v) = value;
}

int DenseGraph::getInDegree(int node_index) {
    int sum = 0;
    for(int i = 0; i < _total_node; i++) {
        if(mat(i, node_index) != _not_connected) sum++;
    }
    return sum;
}

int DenseGraph::getOutDegree(int node_index) {
    int sum = 0;
    for(int i = 0; i < _total_node; i++) {
        if(mat(node_index, i) != _not_connected) sum++;
    }
    return sum;
}

