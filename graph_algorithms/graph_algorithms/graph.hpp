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

template <class NodeT, class EdgeT>
class Node {
public:
    NodeT data;
    list<int> next;
    list<EdgeT> edge_val;
    
    Node() {}
    
    Node(const NodeT& data, const list<int>& next, const list<EdgeT>& edge_val) {
        this -> data = data;
        this -> next = next;
        this -> edge_val = edge_val;
    }
    
    Node& operator= (const NodeT& node_value) const {
        data = node_value;
    }
    
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
template <class NodeT, class EdgeT>
class SparseGraph: public Graph {
    friend class GraphFactory;
private:
    vector<Node<NodeT, EdgeT>>* _nodes;
    bool _copied;
    bool _calculate_edge;
    bool _supress_warnings;
    bool _allow_duplicate;
    
    inline auto& _get(int u) {
        return _nodes -> at(u);
    }
    
    inline auto& _get_next(int u) {
        return (_nodes -> at(u)).next;
    }
    
    inline auto& _get_value(int u) {
        return (_nodes -> at(u)).edge_val;
    }
    
    SparseGraph(bool calculate_edge = false, bool suppress_warnings = false,
                bool allow_duplicate = false): Graph(){
        _is_sparse = true;
        _calculate_edge = calculate_edge;
        _supress_warnings = suppress_warnings;
        _allow_duplicate = allow_duplicate;
    }
    
    SparseGraph(int total_node, bool calculate_edge = false,
                bool suppress_warnings = false,
                bool allow_duplicate = true);
    
    SparseGraph(vector<Node<NodeT, EdgeT>>* nodes,
                bool calculate_edge = false,
                bool copy_list = false,
                bool suppress_warnings = false,
                bool allow_duplicate = false);
    
public:
    inline auto& operator[](int node_index_u) {
        return _nodes -> at(node_index_u);
    }
    
    void assignNode(int node_index_u, NodeT& new_value) {
        _nodes -> at(node_index_u) = new_value;
    }
    
    bool isConnect(int node_index_u, int node_index_v);
    int getInDegree(int node_index);
    int getOutDegree(int node_index);
    void addEdge(int node_index_u, int node_index_v, const EdgeT& value = EdgeT());
    void addEdgeFast(int node_index_u, int node_index_v, const EdgeT& value = EdgeT());
    ~SparseGraph() {
        if(_copied) {
            delete _nodes;
        }
    }
};

class GraphFactory {
public:
    static DenseGraph* createNewDense(int total_node,
                          int not_connected_const,
                          int calculate_edge = false,
                          bool suppress_warnings = false) {
        return new DenseGraph(total_node, not_connected_const,
                              calculate_edge, suppress_warnings);
    }
    
    // create a graph with adjacent matrix
    static DenseGraph* createDense(vector<vector<int>>* matrix,
                       int not_connected_const = 0,
                       int calculate_edge = false,
                       bool copy_matrix = false,
                       bool suppress_warnings = false) {
        return new DenseGraph(matrix, not_connected_const, calculate_edge,
                              copy_matrix, suppress_warnings);
    }
    
    template <class NodeT, class EdgeT>
    static SparseGraph<NodeT, EdgeT>* createSparse(vector<Node<NodeT, EdgeT>>* graphNodes,
                        bool allow_duplicate = true,
                        bool calculate_edge = false,
                        bool copy_list = false,
                        bool suppress_warnings = false) {
        return new SparseGraph<NodeT, EdgeT>(graphNodes, calculate_edge, copy_list, suppress_warnings,
                                      allow_duplicate);
    }
    
    // create a graph with adjacent list
    template <class NodeT, class EdgeT>
    static SparseGraph<NodeT, EdgeT>* createNewSparse(int total_node, bool allow_duplicate = true,
                                  bool calculate_edge = false,
                                  bool suppress_warnings = false) {
        return new SparseGraph<NodeT, EdgeT>(total_node, calculate_edge, suppress_warnings,
                                      allow_duplicate);
    }
    
};

/*
 ----------------------------------------------------------------------------------------------------------
 Here begins the implementation of the template class SparseGraph
 ----------------------------------------------------------------------------------------------------------
 */


template<class NodeT, class EdgeT>
SparseGraph<NodeT, EdgeT>::SparseGraph(int total_node, bool calculate_edge, bool suppress_warnings,
                                       bool allow_duplicate): SparseGraph<NodeT, EdgeT>(calculate_edge, suppress_warnings, allow_duplicate) {
    _total_node = total_node;
    _copied = true;
    _nodes = new vector<Node<NodeT, EdgeT>>(total_node);
    if(calculate_edge) {
        _total_edge = 0;
    }
}

template<class NodeT, class EdgeT>
SparseGraph<NodeT, EdgeT>::SparseGraph(vector<Node<NodeT, EdgeT>>* nodes,
                                       bool calculate_edge,
                                       bool copy_list,
                                       bool suppress_warnings,
                                       bool allow_duplicate): SparseGraph<NodeT, EdgeT>(calculate_edge, suppress_warnings, allow_duplicate) {
    _total_node = nodes -> size();
    _copied = copy_list;
    if(copy_list) {
        _nodes = new vector<Node<NodeT, EdgeT>>();
        *_nodes = *nodes;  // copy
    } else {
        _nodes = nodes;
    }
    if(calculate_edge) {
        _total_edge = 0;
        for(int i = 0; i < nodes -> size(); i++) {
            _total_edge += _get(i).next.size();
        }
    }
}

template<class NodeT, class EdgeT>
bool SparseGraph<NodeT, EdgeT>::isConnect(int node_index_u, int node_index_v){
    auto& next = _get_next(node_index_u);
    return find(next.begin(), next.end(), node_index_v) != next.end();
}

template<class NodeT, class EdgeT>
int SparseGraph<NodeT, EdgeT>::getInDegree(int node_index) {
    int ans = 0;
    for(int u = 0; u < _total_node; u++) {
        for(const int& v: _get_next(u)) {
            ans += (v == node_index);
        }
    }
    return ans;
}

template<class NodeT, class EdgeT>
int SparseGraph<NodeT, EdgeT>::getOutDegree(int node_index) {
    return _get_next(node_index).size();
}

template<class NodeT, class EdgeT>
void SparseGraph<NodeT, EdgeT>::addEdge(int node_index_u, int node_index_v, const EdgeT& value) {
    auto& next = _get_next(node_index_u);
    auto& values = _get_value(node_index_u);
    if(!_allow_duplicate) {
        if(find(next.begin(), next.end(), node_index_v) != next.end()) {
            if(!_supress_warnings) {
                cout << "WARNING: Duplicate edges where duplicate edge is not allowed, the operation of adding edge will be neglected." << endl;
            }
        } else {
            next.push_back(node_index_v);
            values.push_back(value);
            if(_calculate_edge) {
                _total_edge++;
            }
        }
    } else {
        next.push_back(node_index_v);
        values.push_back(value);
        if(_calculate_edge) {
            _total_edge++;
        }
    }
}

template<class NodeT, class EdgeT>
void SparseGraph<NodeT, EdgeT>::addEdgeFast(int node_index_u, int node_index_v, const EdgeT& value) {
    auto& next = _get_next(node_index_u);
    auto& values = _get_value(node_index_u);
    next.push_back(node_index_v);
    values.push_back(value);
    if(_calculate_edge) {
        _total_edge++;
    }
}

#endif /* graph_hpp */
