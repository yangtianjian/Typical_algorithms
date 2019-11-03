//
//  fft.hpp
//  graph_algorithms
//
//  Created by apple on 2019/10/7.
//  Copyright © 2019 apple. All rights reserved.
//

#ifndef fft_hpp
#define fft_hpp

#include <stdio.h>
#include <vector>
#include <complex>
#include <iostream>

#define PI 3.1415926536

using namespace std;

const auto I = complex<double>{0.0, 1.0};

class FFT {
    
private:
    /*
        Naive implementation of covolution with fft_naive. Acting as a comparator of the optimized one.
     */
    static void _convolve_naive(const vector<double>& v1, const vector<double>& v2,
                                   vector<double>& res, string mode = "FULL") {
        const vector<double>* x;  // x is the longer one.
        const vector<double>* k;
        int n1 = (int) v1.size(), n2 = (int) v2.size();  // n1 is the larger one
        if(n1 > n2) {
            x = &v1;
            k = &v2;
        } else {
            x = &v2;
            k = &v1;
            swap(n1, n2);
        }
        
        // c[i] = a[k] * b[i - k]
        int end = 0, start = 0;
        if(mode == "FULL") { end = n1 + n2 - 2; start = 0;}
        else if(mode == "SAME") { end = n1 - 1; start = 0;}
        else { end = n1 - 1; start = n2 - 1;}
        
        for(int i = start; i <= end; i++) {
            double sum = 0.0;
            for(int j = 0; j < n2; j++) {
                if(i - j >= 0 && i - j < n1) sum += (x -> at(i - j)) * (k -> at(j));
            }
            res[i - start] = sum;
        }
    }
    
    /**
        First implementation of fft. This is a simple but not a good implementation because it needs extra memory.
        n must be the power of 2, so does v.size()
     */
    static void _fft(const vector<double>& v, int n, vector<complex<double>>& res) {
        if(n == 1) {
            res[0] = v[0];
            return;
        }
        vector<double> ve(n >> 1), vo(n >> 1);
        vector<complex<double>> re(n >> 1), ro(n >> 1);
        
        for(int k = 0; k < (n >> 1); k++) {
            ve[k] = v[k << 1];
            vo[k] = v[(k << 1) + 1];
        }
        
        _fft(ve, n >> 1, re);
        _fft(vo, n >> 1, ro);
        
        complex<double> wn(cos(2 * PI / n), sin(2 * PI / n)), w(1.0, 0.0);
        for(int k = 0; k < (n >> 1); k++) {
            res[k] = re[k] + w * ro[k];
            res[k + (n >> 1)] = re[k] - w * ro[k];
            w = w * wn;
        }
    }
    
    /**
        This is  a part of recursive version of IFFT.
     */
    static void _ifft_(const vector<complex<double>>& v, int n, vector<complex<double>>& res) {
        if(n == 1) {
            res[0] = v[0];
            return;
        }
        vector<complex<double>> ve(n >> 1), vo(n >> 1);
        vector<complex<double>> re(n >> 1), ro(n >> 1);
        
        for(int k = 0; k < (n >> 1); k++) {
            ve[k] = v[k << 1];
            vo[k] = v[(k << 1) + 1];
        }
        
        _ifft_(ve, n >> 1, re);
        _ifft_(vo, n >> 1, ro);
        
        complex<double> wn(cos(2 * PI / n), sin(-2 * PI / n)), w(1.0, 0.0);
        for(int k = 0; k < (n >> 1); k++) {
            res[k] = (re[k] + w * ro[k]);
            res[k + (n >> 1)] = (re[k] - w * ro[k]);
            w = w * wn;
        }
    }
    
    static void _ifft(const vector<complex<double>>& v, int n, vector<complex<double>>& res) {
        _ifft_(v, n, res);
        for(int i = 0; i < n; i++) res[i] = res[i] / (double) n;
    }
    
    static void _convolve(const vector<double>& v1, const vector<double>& v2,
                          vector<complex<double>>& res, string mode = "FULL") {
        int n1 = (int) v1.size(), n2 = (int) v2.size();
        int nc1 = 1 << (int) ceil(log2((double) n1));   // pad the vectors to the power of 2
        int nc2 = 1 << (int) ceil(log2((double) n2));
        int nc = max(nc1, nc2) * 2;  // 2n points fft
        
        vector<double> v1c(nc, 0.0), v2c(nc, 0.0);
        copy(v1.begin(), v1.end(), v1c.begin());
        copy(v2.begin(), v2.end(), v2c.begin());
        
        vector<complex<double>> z1(nc), z2(nc);
        _fft(v1c, nc, z1);
        _fft(v2c, nc, z2);
        
        vector<complex<double>> z(nc);
        for(int i = 0; i < nc; i++) {
            z[i] = z1[i] * z2[i];
        }
        
        vector<complex<double>> tempres(nc);
        _ifft(z, nc, tempres);
        
        int start = 0, end = 0;
        if(mode == "FULL") { end = n1 + n2 - 2; start = 0;}
        else if(mode == "SAME") { end = n1 - 1; start = 0;}
        else { end = n1 - 1; start = n2 - 1;}
        
        for(int i = start; i <= end; i++) res[i - start] = tempres[i];
    }
    
    // This function produces real number after convolution
    static void _convolve(const vector<double>& v1, const vector<double>& v2,
                          vector<double>& res, string mode = "FULL") {
        vector<complex<double>> tempres(res.size());
        _convolve(v1, v2, tempres, mode);
        for(int i = 0; i < res.size(); i++) res[i] = tempres[i].real();
    }
    
    
public:
    
    static void run_fft();
    
    static void run_ifft();
    
    static void run_test_convolve_naive() {
        vector<double> v1{1.0, 2.0, 3.0}, v2{1.0, 2.0};
        vector<double> v3{0.0, 0.0, 0.0};
        FFT::_convolve_naive(v1, v2, v3, "SAME");
        for(int i = 0; i < v3.size(); i++) {
            cout << v3[i] << " ";
        }
        cout << endl;
    }
    
    static void run_test_fft() {
        vector<double> v{1.0, 2.0, 3.0, 4.0};
        vector<complex<double>> res(4);
        FFT::_fft(v, 4, res);
        for(int i = 0; i < res.size(); i++) {
            cout << res[i] << " ";
        }
        cout << endl;
    }
 
    static void run_test_convolve() {
        vector<double> v1{1.0, 2.0, 3.0}, v2{1.0, 2.0};
        vector<double> v3{0.0, 0.0, 0.0 };
        FFT::_convolve(v1, v2, v3, "SAME");
        for(int i = 0; i < v3.size(); i++) {
            cout << v3[i] << " ";
        }
        cout << endl;
    }
    
    static void run_test_all() {
        FFT::run_test_fft();
        FFT::run_test_convolve_naive();
        FFT::run_test_convolve();
    }
};

#endif /* fft_hpp */
