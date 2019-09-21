//
//  graph.hpp
//  graph_algorithms
//
//  Created by apple on 2019/9/19.
//  Copyright © 2019 apple. All rights reserved.
//

#ifndef graph_hpp
#define graph_hpp

#include <vector>
#include <iostream>
#include <list>

using namespace std;

template <class NodeT>
class Node {
public:
    NodeT data;
    list<int> next;
};

class Graph {
protected:
    int INVALID = -INT_MAX;
protected:
    bool _is_sparse;
    int _total_node;
    int _total_edge;
    
protected:
    Graph() {
        _total_node = INVALID;
        _total_edge = INVALID;
    }
    
public:
    int getTotalNode() {
        if(_total_node == INVALID) {
            throw "Total node number is not calculated.";
        }
        return _total_node;
    }
    
    int getTotalEdge() {
        if(_total_node == INVALID) {
            throw "Total node number is not calculated.";
        }
        return _total_edge;
    }
    
    virtual int getInDegree(int node_index) = 0;
    virtual int getOutDegree(int node_index) = 0;
    virtual bool isConnect(int node_index_u, int node_index_v) = 0;
    virtual void addEdge(int node_index_u, int node_index_v, int value = 1) = 0;
    
};

// For dense graph, we use adjacent matrix to store it
class DenseGraph: public Graph {
    friend class GraphFactory;
    
private:
    int _not_connected;
    vector<vector<int>>* _matrix;
    bool _matrix_is_copied;
    bool _dynamic_mode;
    bool _suppress_warnings;
    
    inline int& mat(int i, int j) {
        return (_matrix -> at(i))[j];
    }

private:
    
    DenseGraph(): Graph() {
        _is_sparse = false;
    }
    
    /*
     Create a dynamic graph.
     */
    DenseGraph(int total_node,
               int not_connected_const = 0,
               bool calculate_edge = false,
               bool suppress_warnings = false);
    /*
     Create a static graph, given the adjacent matrix.
     copy_matrix: If it is true, matrix will be copied. Therefore, the input matrix will be safe if the caller's matrix is out of its scope, but it takes time to make a copy.
     suppress_warnings: If it is true, the warnings printed in console will turn off
     */
    DenseGraph(vector<vector<int>>* matrix,
               int not_connected_const = 0,
               bool calculate_edge = false,
               bool copy_matrix = false,
               bool suppress_warnings = false);
public:
    bool isConnect(int node_index_u, int node_index_v) {
        return mat(node_index_u, node_index_v);
    }
    
    int getInDegree(int node_index);
    int getOutDegree(int node_index);
    void addEdge(int node_index_u, int node_index_v, int value = 1);
    void addEdgeFast(int node_index_u, int node_index_v, int value = 1);
    
    ~DenseGraph() {
        if(_matrix_is_copied) {
            _matrix -> clear();
            delete _matrix;
        }
    }

};


// For the sparse ones, adjacent list is a good choice
template <class NodeT>
class SparseGraph: public Graph {
private:
    vector<Node<NodeT>>* _nodes;
    bool _copied;
    bool _calculate_edge;
    bool _supress_warnings;
    bool _ensure_no_duplicate;
    
    inline Node<NodeT>& _get(int u) {
        return _nodes -> at(u);
    }
    
    inline auto& _get_next(int u) {
        return (_nodes -> at(u)).next;
    }
    
    SparseGraph(bool calculate_edge = false, bool suppress_warnings = false,
                bool ensure_no_duplicate = false): Graph(){
        _is_sparse = true;
        _calculate_edge = calculate_edge;
        _supress_warnings = suppress_warnings;
        _ensure_no_duplicate = ensure_no_duplicate;
    }
    
    SparseGraph(int total_node, bool calculate_edge = false,
                bool suppress_warnings = false,
                bool ensure_no_duplicate = false);
    
    SparseGraph(vector<Node<NodeT>>* nodes,
                bool calculate_edge = false,
                bool copy_list = false,
                bool suppress_warnings = false,
                bool ensure_no_duplicate = false);
    
public:
    inline Node<NodeT>& operator[](int node_index_u) {
        return _nodes -> at(node_index_u);
    }
    
    bool isConnect(int node_index_u, int node_index_v);
    int getInDegree(int node_index);
    int getOutDegree(int node_index);
    void addEdge(int node_index_u, int node_index_v);
    void addEdgeFast(int node_index_u, int node_index_v);
    ~SparseGraph() {
        if(_copied) {
            delete _nodes;
        }
    }
};

class GraphFactory {
public:
    static Graph* createNewDense(int total_node,
                          int not_connected_const,
                          int calculate_edge = false,
                          bool suppress_warnings = false) {
        return new DenseGraph(total_node, not_connected_const,
                              calculate_edge, suppress_warnings);
    }
    
    // create a graph with adjacent matrix
    static Graph* createDense(vector<vector<int>>* matrix,
                       int not_connected_const = 0,
                       int calculate_edge = false,
                       bool copy_matrix = false,
                       bool suppress_warnings = false) {
        return new DenseGraph(matrix, not_connected_const, calculate_edge,
                              copy_matrix, suppress_warnings);
    }
    
    template <class NodeT>
    static Graph* createSparse(vector<Node<NodeT>>* graphNodes,
                        bool calculate_edge = false,
                        bool copy_list = false,
                        bool suppress_warnings = false,
                        bool ensure_no_duplicate = false) {
        return new SparseGraph<NodeT>(graphNodes, calculate_edge, copy_list, suppress_warnings,
                                      ensure_no_duplicate);
    }
    
    // create a graph with adjacent list
    template <class NodeT>
    static Graph* createNewSparse(int total_node, bool calculate_edge = false,
                           bool suppress_warnings = false,
                           bool ensure_no_duplicate = false) {
        return new SparseGraph<NodeT>(total_node, calculate_edge, suppress_warnings,
                                      ensure_no_duplicate);
    }
    
};

#endif /* graph_hpp */
