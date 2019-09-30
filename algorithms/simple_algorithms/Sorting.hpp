//
//  Sorting.hpp
//  graph_algorithms
//
//  Created by apple on 2019/9/26.
//  Copyright © 2019 apple. All rights reserved.
//

#ifndef Sorting_hpp
#define Sorting_hpp

#include <stdio.h>
#include <vector>

using namespace std;


template <class T>
class Sort {
    
private:
    
    static void _mergeSortL(const vector<T>& x, vector<int>& link, int l, int r, int& start) {
        if(l == r) {
            start = l;
            return;
        }
        if(r - l == 1) {
            if(x[l] <= x[r]) {
                link[l] = r;
                start = l;
            } else {
                link[r] = l;
                start = r;
            }
            return;
        }
        int start1 = -1, start2 = -1;
        int mid = (l + r) >> 1;
        _mergeSortL(x, link, l, mid, start1);
        _mergeSortL(x, link, mid + 1, r, start2);
        
        int i = start1, j = start2, new_start = 0, k = 0, num = 0;
        while(i != -1 || j != -1) {
            if(i != -1 && (j == -1 || x[i] <= x[j])) {
                if(num == 0) {
                    new_start = i;
                    k = new_start;
                    num++;
                } else {
                    link[k] = i;
                    k = i;
                }
                i = link[i];
            } else if(j != -1 && (i == -1 || x[j] < x[i])) {
                if(num == 0) {
                    new_start = j;
                    k = new_start;
                    num++;
                } else {
                    link[k] = j;
                    k = j;
                }
                j = link[j];
            }
        }
        start = new_start;
    }
    
    
    static void _quickSort(vector<T>& x, int l, int r) {
        if(l >= r) return;
        int v = x[l], i = l, j = r;
        do {
            while(i <= r && x[i] <= v) i++;
            while(j >= l && x[j] > v) j--;
            if(i < j) {
                swap(x[i], x[j]);
            } else {
                break;
            }
        }while(true);
        x[l] = x[j]; x[j] = v;
        _quickSort(x, l, j - 1);
        _quickSort(x, j + 1, r);
    }
    
public:
    static void runMergeSortL(vector<T>& x) {
        vector<int> link(x.size(), -1);
        int start;
        _mergeSortL(x, link, 0, x.size() - 1, start);
        vector<T> y(x.size(), 0);
        int i = start, k = 0;
        while(i != -1) {
            y[k++] = x[i];
            i = link[i];
        }
        x = y;
    }
    
    static void runQuickSort(vector<T>& x) {
        _quickSort(x, 0, x.size() - 1);
    }
};

#endif /* Sorting_hpp */
