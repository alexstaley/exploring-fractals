/*
Alex Staley - CS410P - August 2020

  THE GHOST OF THE MANDELBROT SET
  (IT'S IMAGINARY, BUT ALSO REAL)
*/

#include <stdio.h>
#include <math.h>
#include <complex.h>
#include "FPToolkit.c"

#define SWIDTH 746
#define SHEIGHT 746

complex double mapComplex(double x, double y);
complex double iterate(complex double c, int reps);
void plotPoint(double x, double y, complex double z, double depth);

int main()
{
  double x,y,depth;
  complex double c,z;

  // Set up display environment
  //G_choose_repl_display(); //for repl
  G_init_graphics(SWIDTH, SHEIGHT);

  int startDepth = 2;
  int endDepth = 45;
  double range = endDepth - startDepth;

  // Process each point on the screen
  // for every integer depth in the range
  for (int d=startDepth; d<=endDepth; ++d) {
    for (x=0; x<=SWIDTH; ++x) {
      for (y=0; y<=SHEIGHT; ++y) {
        c = mapComplex(x, y);
        z = iterate(c, d);
        depth = (d - startDepth) / range;
        plotPoint(x, y, z, depth);
      }
    }
  }

  // Display and save image
  G_wait_key();
  G_save_to_bmp_file("mandelbrot.bmp");
  return 0;
}

// Map (x,y) to the complex plane
complex double mapComplex(double x, double y) {
  double cx = 3*x/SWIDTH - 2.15;
  double cy = 3*y/SHEIGHT - 1.5;
  return cx + cy*I;
}

// Iterate z = z^2 + c
complex double iterate(complex double c, int reps) {
  complex double z = 0;
  for (int k=0; k<reps; ++k) {
    z = z*z + c;
  }    
  return z;
}

// Plot (x,y) based on divergence of z
void plotPoint(double x, double y, complex double z, double depth) {
  if (cabs(z) < 1000) {
    G_rgb(depth,depth,depth);
    G_point(y, x);
  }
}
