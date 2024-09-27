#include <iostream>
#include <vector>
#include <string>
#include <complex>
#include <math.h>

using namespace std;


int mandelbrot(int max_iterations, double z_re, double z_im, double c_re, double c_im) {
    int new_re = z_re * z_re - c_re;
    int new_im = z_im * z_im - c_im;
    
    if((new_re * new_re + new_im * new_im) > 4) {
        return -1;
    }
    if (max_iterations > 0) {
        return mandelbrot(max_iterations, new_re, new_im, c_re, c_im);
    }
    return max_iterations;
}


int main() {
    double c_re = -0.7;
    double c_im = 0.27015;
    int w = 500;
    int h = 300;
    int max_iterations = 350;

    for(int x = 0; x < w; x++) {
        for(int y = 0; y < h; y++) {
            double z_re = (x/w)*3;
            double z_im = (x/h)*3;
            int is_valid = max_iterations - (max_iterations, z_re, z_im, c_re, c_im);
        }
    }
}





/*
    1
-2 ------- 1
    -1

z = z_n-1^2 + c

*/


