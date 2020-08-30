/*
Alex Staley - CS410P - August 2020

  THE NAUTILITERATORUS:
  AN IFS FRACTAL DESIGN
*/

#include "FPToolkit.c"
void translate (double dx, double dy);
void scale (double sx, double sy);
void rotate (double degrees);
double getLength(double a, double b);

double x[1] = {0};
double y[1] = {0};
int n = 1;

int main() 
{
  // Set up display environment
  //G_choose_repl_display(); //for repl
  int swidth = 746; int sheight = 746;
  G_init_graphics(swidth, sheight);
  G_rgb(0.067, 0.071, 0.067);
  G_clear();
  srand48(time(0));

  double momL = getLength(99, 25);
  double momA = atan2(99, 25);

  double mainL = getLength(83, 25);
  double mainA = atan2(83, 25);
  double mainD = mainA - momA;

  double leftL = getLength(33, 35);
  double leftA = atan2(35, -33);
  double leftD = leftA - momA;

  double rightL = getLength(30, 14);
  double rightA = atan2(14, 30) - M_PI/6.0;
  double rightD = momA - rightA;

  double r, factor, xCoord, yCoord;
  double red, grn, blu;
  int j = 0;

  while (j < 250000) {
    r = drand48();

    if(r < 0.75) { //1st child
      factor = mainL / momL;
      translate(-0.5, 0);
      rotate(mainD);
      scale(factor, factor);
      translate(0.5, 4.0/25.0);
    }
    else if(r < 0.95) { //2nd child
      factor = leftL / momL;
      translate(-0.5, 0);
      rotate(leftD);
      scale(factor, factor);
      translate(0.5, 4.0/25.0);
    }
    else { //3rd child
      factor = rightL / momL;
      translate(-0.5, 0);
      rotate(rightD);
      scale(factor, 0.25);
      translate(0.5, 1.0/25.0);
    }

    // "Every time" adjustments
    rotate(M_PI/4.0);
    translate(0.94, 0.02);
    xCoord = x[0]*swidth*0.66;
    yCoord = y[0]*sheight*0.48;
    red = 0.25*y[0] + 0.11/(x[0]+0.5);
    grn = 0.41/y[0] + 0.26*x[0];
    blu = 0.4/(y[0]+0.3) + 0.11/(x[0]+0.3);
    G_rgb(red, grn, blu);
    G_point(xCoord, yCoord);
    ++j;
  }

  // Display and save image
  G_wait_key();
  G_save_to_bmp_file("nautiliteratorus.bmp");
}

// Return the magnitude of the vector
double getLength(double a, double b) {
  return sqrt(a*a + b*b);
}

// Shift a point dx and dy
void translate (double dx, double dy) {
  for (int i=0; i<n; ++i) {
    x[i] = x[i] + dx;
    y[i] = y[i] + dy;
  }
}

// Scale a point by sx and sy
void scale (double sx, double sy) {
  for (int i=0; i<n; ++i) {
    x[i] = sx * x[i];
    y[i] = sy * y[i];
  }
}

// Rotate a point r radians about (0,0)
void rotate (double r) {
  double t;
  double c = cos(r);
  double s = sin(r);

  for (int i=0; i<n; ++i) {
     t = c*x[i] - s*y[i];
     y[i] = s*x[i] + c*y[i];
     x[i] = t;
  }
}
