#include "mylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// here we check if the flag EXTERNC is set, if it is then
// we will tell the compiler to treat the code below a C code
// generating the library

#ifdef EXTERNC
extern "C" {
#endif

// estimate pi from the dart throwing method
double findPi(long nthrows=100*1000*1000){
  srand48((long)time(NULL));
  long count=0;
  for (int i=0; i<nthrows; ++i){
    double x=drand48();
    double y=drand48();
    if (x*x+y*y < 1) ++count;
  }
  return 4.0*count/nthrows;
}
 

// return the magnitude of a complex number
double magnitude(double re, double im){
  return sqrt(re*re+im*im);
}


int mandel_test(double c_re, double c_im, int NTRIALS){  
  // If a point is in the set, its magnitude will remain bounded by
  // 2.0 over iterations of z -> z^2 + C.  Stop the loop after a 
  // maximum of NTRIALS and consider this point to be in the set

  double z_re = c_re;
  double z_im = c_im;
  int counts = 1;
  while (magnitude(z_re,z_im)<=2.0  && counts<NTRIALS) {
    counts++;
    double re=z_re;  // careful! keep old values for calculation
    double im=z_im;
    // z -> z^2
    z_re = re*re-im*im;
    z_im = 2*re*im;
    // add c to z^2
    z_re = z_re + c_re;
    z_im = z_im + c_im;
  }
  return counts;
}


// explore the Mandelbrot set
// we pass the image buffer as a 1D array, and access the pixels
// using pointer arithmatic
// eg for an array a[n][m], n=nrow, m=mcolumns
// a[0] = a[0][0]
// a[m] = a[1][0]
// a[m+1] = a[1][1]
// ...
void mandel(double *img, double re1, double re2, double im1, double im2,
	    int nr, int ni, int NTRIALS){
  double dx=(re2-re1)/nr;
  double dy=(im2-im1)/ni;
  // loop over grid starting in lower left corner
  for (int j=0; j<ni; ++j){
    double im=im1+j*dy;
    for (int i=0; i<nr; ++i){
      double re=re1+i*dx;
      img[j*ni+i]=mandel_test(re,im,NTRIALS);
    }
  }
}


void scale_vec(int dim, std::vector<double>& v, double scale) {
    // scale a positive vector to have components between -scale/2 and scale/2
    for (int i = 0; i<dim; ++i) {
        v[i] = v[i] * scale - (scale/2);
    }
}

double vector_mag(int dim, std::vector<double>& v) {
    // calculate the magnitude of a vector
    double sum = 0;
    for (int j=0; j<dim; ++j){
        sum += v[j]*v[j];
    }
    double mag = std::pow(sum, 0.5);
    return mag;   

}

bool check_inside(int dim, std::vector<double>& v, double r) {
    // check if vector is inside a dim dimensional sphere centered at the origin
    std::vector<double> temp(v); // create a deep copy of the vector
    double mag = vector_mag(dim, temp);
    return mag <= r;
}

// do mc integration of a dim dimensional sphere with radius r with N total points, n_axis points along each axis
// return the relative error for plotting in python
double pseudo_mc(int dim, long int N, double r) {
    std::vector<double> pseudo_pos(dim,0.0); // position vector for pseudo random distribution method
    double box_length = 2.*r; // length of box to surround the 5d sphere
    double box_volume = std::pow(box_length, 1.*dim); // volume of 5d box
    long int N_inside = 0; // count of points that land inside sphere
    double std_dev = 0.0; // standard deviation

    TRandom3 *tr3 = new TRandom3(0);; // trandom pseudo rng

    for (int i=0; i<N; i++) {
        tr3->RndmArray(dim,pseudo_pos.data()); // generate a random vector
        scale_vec(dim, pseudo_pos, box_length); // scale the vector by the box length
        if (check_inside(dim, pseudo_pos, r)) {
            // std::cout<<"Inside"<<std::endl;
            N_inside++;
        }
    }
    double p = (1.*N_inside)/N;
    double volume = p * box_volume; // set input parameter to the volume

    // calculate relative error
    if (N_inside < 1) {
        std_dev = 1.0;
    } else {
        std_dev = std::sqrt(1.*(N_inside - N * p * p)/(N-1));
        std_dev = (1/p) * (std_dev / std::sqrt(N)); // relative error
    }

    // // package results
    // static double results[2];
    // results[0] = volume;
    // results[1] = std_dev;
    return std_dev;
}
  

#ifdef EXTERNC
}
#endif




/*
int main(){
  findPi();
  return 0;
} 
*/
