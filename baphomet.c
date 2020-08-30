/*
Alex Staley - CS410P - July 2020

  HOW TO SUMMON BAPHOMET WITH
  A PYTHAGORAS TREE FRACTAL
*/

#include  "FPToolkit.c"

void background(int swidth, int sheight, double colorBot[], double colorTop[]);
void buildHead(int swidth, int sheight);
void buildHorns(int swidth, int sheight, int depth);
void buildEyes(int swidth, int sheight, int depth);
void eyeballs(double eyeColors[], double eyeCoords[], double radius);
void pentagrams(double eyes[], double radius);
void markOfTheBeast(int swidth, int sheight, int depth);
void pyTree(double p1[], double p2[], int dep, int orient, int color[]);
void getMidCR(double lc[], double rc[], double * mc);
void getMidCL(double lc[], double rc[], double * mc);

const double piBy3 = M_PI / 3.0;
const double piBy5 = M_PI / 5.0;
const double piBy6 = M_PI / 6.0;

int main()
{
   // Screen dimensions swidth and sheight
   // determine the dimensions of Baphomet
   int swidth = 666;  //if not using repl, please
   int sheight = 666; //initialize to 666 x 666. -B
   double colorBot[3] = {0.44, 0.14, 0.05};
   double colorTop[3] = {0.03, 0.01, 0.01};

   // Set up display
   //G_choose_repl_display(); //enable if repling
   G_init_graphics(swidth,sheight);

   // Draw the background
   background(swidth, sheight, colorBot, colorTop);
   markOfTheBeast(swidth, sheight, 15);

   // Draw Baphomet
   buildHead(swidth, sheight);
   buildHorns(swidth, sheight, 12);
   buildEyes(swidth, sheight, 10);
   
   // Display and save image
   int key =  G_wait_key();
   G_save_to_bmp_file("baphomet.bmp");

   return 0;
}

// Fade the background colors
void background(int swidth, int sheight, double colorBot[], double colorTop[]) {
  double r, g, b, scaleFactor;
  double rShift, bShift, gShift;
  double bound = 0.5 * sheight;

  rShift = colorTop[0] - colorBot[0];
  gShift = colorTop[1] - colorBot[1];
  bShift = colorTop[2] - colorBot[2];

  for(double k=0; k<=sheight; ++k) {
    scaleFactor = k/bound;

    r = colorBot[0] + scaleFactor*rShift;
    g = colorBot[1] + scaleFactor*gShift;
    b = colorBot[2] + scaleFactor*bShift;

    G_rgb(r,g,b);
    G_line(0,k, swidth,k);
  }
}

// A circle and a triangle
void buildHead(int swidth, int sheight) {
  Gi_rgb(96, 36, 12);
  G_fill_circle(0.5*swidth, (5.0/24.0)*sheight, (11.0/60.0)*sheight);
  G_fill_triangle(0.25*swidth, (11.0/30.0)*sheight, 0.75*swidth, (11.0/30.0)*sheight, 0.5*swidth, -0.25*sheight); 
}

// Two mirrored fractal ibex horns
void buildHorns(int swidth, int sheight, int depth) {
  int colorHorns[6] = {39, 33, 33, 175, 4, 4};
  double p1[2], p2[2], p3[2], p4[2];

  // X-coordinates of horn roots
  p1[0] = 0.365 * swidth;
  p4[0] = 0.635 * swidth;
  p2[0] = p1[0] + 0.0625 * swidth;
  p3[0] = p4[0] - 0.0625 * swidth;

  // Y-coordinates of horn roots
  p1[1] = 0.35 * sheight;
  p4[1] = 0.35 * sheight;
  p2[1] = p1[1] + 0.025 * sheight;
  p3[1] = p4[1] + 0.025 * sheight;
  
  pyTree(p1, p2, depth, 'L', colorHorns);
  pyTree(p3, p4, depth, 'R', colorHorns);
}

// Windows into Hell
void buildEyes(int swidth, int sheight, int depth) {
  double colorEyes[9] = {0.92, 0.51, 0.26, 0.9, 0.09, 0.03, 0.96, 0.85, 0.39};
  double eyes[4];  //centers of eyes
  double radius = (1.0 / 15.0) * sheight; //eyeball radius

  // Calculate eye coordinates
  eyes[0] = 0.415 * swidth;
  eyes[1] = 0.19 * sheight;
  eyes[2] = 0.585 * swidth;
  eyes[3] = 0.19 * sheight;

  // Draw eyes
  eyeballs(colorEyes, eyes, radius);
  pentagrams(eyes, radius);
}

// Fiery shaded circles
void eyeballs(double eyeColors[], double eyeCoords[], double radius) {
  double r, g, b, shadeScale;  //eye color fading utils
  double inRadius = 0.3 * radius;  //iris radius
  double rShift = eyeColors[3] - eyeColors[0];   //shading factor r
  double gShift = eyeColors[4] - eyeColors[1]; //shading factor g
  double bShift = eyeColors[5] - eyeColors[2];  //shading factor b

  // Outer color fade (NOT APPROVED BY BAPHOMET)
  for (double i=inRadius; i<radius; ++i) {
    shadeScale = i / radius;
    r = eyeColors[0] + shadeScale * rShift;
    g = eyeColors[1] + shadeScale * gShift;
    b = eyeColors[2] + shadeScale * bShift;

    G_rgb(r, g, b);
    G_circle(eyeCoords[0], eyeCoords[1], i);
    G_circle(eyeCoords[2], eyeCoords[3], i);
  }

  // Inner color fade
  rShift = eyeColors[6] - eyeColors[3];
  gShift = eyeColors[7] - eyeColors[4];
  bShift = eyeColors[8] - eyeColors[5];
  for(double j=0; j<=inRadius; ++j) {
    shadeScale = j / inRadius;
    r = eyeColors[3] + shadeScale * rShift;
    g = eyeColors[4] + shadeScale * gShift;
    b = eyeColors[5] + shadeScale * bShift;

    G_rgb(r, g, b);
    G_circle(eyeCoords[0], eyeCoords[1], j);
    G_circle(eyeCoords[2], eyeCoords[3], j);
  }
}

// Inscribe pentagrams in eyes
void pentagrams(double eyes[], double r) {
  double xPentL[5], yPentL[5], xPentR[10], yPentR[10];
  double arm = 2 * r * sin(2*piBy5);   //long segment
  double leg = 2 * r * sin(piBy5);    //short segment
  double face = eyes[2] - eyes[0]; //distance between eyes
  
  // Calculate pentagram coordinates for left eye
  xPentL[0] = eyes[0];
  xPentL[1] = xPentL[0] + arm * cos(2*piBy5);
  xPentL[3] = xPentL[0] + leg * cos(piBy5);
  xPentL[2] = xPentL[3] - arm;
  xPentL[4] = xPentL[1] - leg;
  yPentL[0] = eyes[1] - r;
  yPentL[1] = yPentL[0] + arm * sin(2*piBy5);
  yPentL[3] = yPentL[0] + leg * sin(piBy5);
  yPentL[2] = yPentL[3];
  yPentL[4] = yPentL[1];

  // Right eye
  for (int i=0; i<6; ++i) {
    xPentR[i] = xPentL[i] + face;
    yPentR[i] = yPentL[i];
  }

  // Draw pentagrams
  G_rgb(0, 0, 0);
  G_polygon(xPentL, yPentL, 5);
  G_polygon(xPentR, yPentR, 5);
}

// 666
void markOfTheBeast(int swidth, int sheight, int depth) {
  double p1a[2], p1b[2], p2a[2], p2b[2], p3a[2], p3b[2];
  int colorMark[6] = {115, 8, 8, 0, 0, 0};

  // Six hundred
  p1a[0] = 0.3 * swidth + 0.05 * swidth;
  p1a[1] = 0.9 * sheight;
  p1b[0] = p1a[0] - 0.002 * swidth;
  p1b[1] = p1a[1] + 0.027 * sheight;

  // Sixty
  p2a[0] = 0.5 * swidth + 0.05 * swidth;
  p2a[1] = p1a[1];
  p2b[0] = p2a[0] - 0.002 * swidth;
  p2b[1] = p1b[1];

  // Six
  p3a[0] = 0.7 * swidth + 0.05 * swidth;
  p3a[1] = p1a[1];
  p3b[0] = p3a[0] - 0.002 * swidth;
  p3b[1] = p1b[1];

  pyTree(p1a, p1b, depth, 'L', colorMark);
  pyTree(p2a, p2b, depth, 'L', colorMark);
  pyTree(p3a, p3b, depth, 'L', colorMark);
}

// Build a Pythagoras tree fractal
void pyTree(double p1[], double p2[], int dep, int orient, int color[]) {
  if (dep == 0) {
    return;
  }

  double leg1, leg2;  //baseline components
  double rectX[4], rectY[4];    //rectangle
  double lc[2], rc[2], mc[2];    //triangle

  leg1 = 1.5 * (p2[0] - p1[0]);
  leg2 = 1.5 * (p2[1] - p1[1]);

  // Define rectangle
  rectX[0] = p1[0];
  rectX[1] = p1[0]-leg2;
  rectX[2] = p2[0]-leg2;
  rectX[3] = p2[0];
  rectY[0] = p1[1];
  rectY[1] = p1[1]+leg1;
  rectY[2] = p2[1]+leg1;
  rectY[3] = p2[1];

  // Draw rectangle
  Gi_rgb(color[0], color[1], color[2]);
  G_fill_polygon(rectX, rectY, 4);

  // Define triangle
  lc[0] = rectX[1];
  lc[1] = rectY[1];
  rc[0] = rectX[2];
  rc[1] = rectY[2];
  if (orient == 'L') {
    getMidCL(lc, rc, mc);
  }
  else getMidCR(lc, rc, mc);

  // Draw triangle
  Gi_rgb(color[3], color[4], color[5]);
  G_fill_triangle(lc[0], lc[1], mc[0], mc[1], rc[0], rc[1]);

  // Recursed
  pyTree(lc, mc, dep-1, orient, color);
  pyTree(mc, rc, dep-1, orient, color);
}

// Define third triangle vertex for right-curling horn
void getMidCR(double lc[], double rc[], double * mc) {
  double t[2];
  t[0] = 0.5 * (rc[0] - lc[0]);
  t[1] = 0.5 * (rc[1] - lc[1]);
  mc[0] = lc[0] + t[0] * cos(piBy3) - t[1] * sin(piBy3);
  mc[1] = lc[1] + t[1] * cos(piBy3) + t[0] * sin(piBy3);
}

// Define third triangle vertex for left-curling horn
void getMidCL(double lc[], double rc[], double * mc) {
  double t[2];
  t[0] = 0.5 * sqrt(3) * (rc[0] - lc[0]);
  t[1] = 0.5 * sqrt(3) * (rc[1] - lc[1]);
  mc[0] = lc[0] + t[0] * cos(piBy6) - t[1] * sin(piBy6);
  mc[1] = lc[1] + t[1] * cos(piBy6) + t[0] * sin(piBy6);
}
