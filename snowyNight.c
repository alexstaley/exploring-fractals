/*
Alex Staley - CS410P - July 2020

  KOCH SNOWFLAKES FALLING
  ON SNOWCAPPED KOCH MOUNTAINS
  UNDER A TWILIGHT SKY
*/

#include  "FPToolkit.c"
const double piBy3 = M_PI / 3.0;

void sky(int swidth, int sheight, double colorBot[], double colorTop[]);
void mountains(int swidth, int sheight, double colorDark[], double colorLite[]);
void cornerFade(double colDark[], double colLite[], double xMax);
void kochFlake(double p1[], double p2[], double color[], int depth);
void kochCurve(double p1[], double p2[], double color[], int curr, int dep);

int main() {
  int swidth, sheight;
  int flakes, depth, orient;
  double p1[2], p2[2], p3[2];
  double skyColorTop[3], skyColorBot[3];
  double mtColorDark[3], mtColorLite[3];

  // Set up display environment
  srand(time(0));
  flakes = 60;
  depth = 6;
  orient = 0;
  swidth = 746;
  sheight = 746;
  //G_choose_repl_display();
  G_init_graphics (swidth,sheight);

  // Night sky
  skyColorBot[0] = 166.0 / 255.0;
  skyColorBot[1] = 145.0 / 255.0;
  skyColorBot[2] = 242.0 / 255.0;
  skyColorTop[0] = 22.0 / 255.0;
  skyColorTop[1] = 15.0 / 255.0;
  skyColorTop[2] = 48.0 / 255.0;
  sky(swidth, sheight, skyColorBot, skyColorTop);

  // Mountains
  mtColorDark[0] = 35;
  mtColorDark[1] = 25;
  mtColorDark[2] = 3;
  mtColorLite[0] = 68;
  mtColorLite[1] = 55;
  mtColorLite[2] = 55;
  mountains(swidth, sheight, mtColorDark, mtColorLite);

  // Moon
  Gi_rgb(245, 252, 193);
  G_fill_circle(swidth*0.8, sheight*0.8, sheight*0.1);
  G_rgb(0.6*skyColorTop[0], 0.6*skyColorTop[1], 0.6*skyColorTop[2]);
  G_fill_circle(swidth*0.81, sheight*0.81, sheight*0.09);

  // Snowflakes
  double white[3] = {255, 255, 255};
  G_rgb(1,1,1);
  for (int i=0; i<flakes; ++i) {
    p1[0] = rand() % (swidth-10);
    p1[1] = rand() % (sheight-10);
    orient = rand();
    p2[0] = p1[0] + (i%20)*cos(orient);
    p2[1] = p1[1] + (i%20)*sin(orient);
    kochFlake(p1, p2, white, depth);
  }

  // Display and save image
  G_wait_key();
  G_save_to_bmp_file("snowyNight.bmp");

  return 0;
}

// Fade the sky's colors from bottom to top
void sky(int swidth, int sheight, double colorBot[], double colorTop[]) {
  double r, g, b, sf;
  double rShift, bShift, gShift;
  double bound = sheight / 2.5;

  rShift = colorTop[0] - colorBot[0];
  gShift = colorTop[1] - colorBot[1];
  bShift = colorTop[2] - colorBot[2];

  for(double k=0; k<=sheight; ++k) {
    sf = k/bound;

    r = colorBot[0] + sf*rShift;
    g = colorBot[1] + sf*gShift;
    b = colorBot[2] + sf*bShift;

    G_rgb(r,g,b);
    G_line(0,k, swidth,k);
  }
}

// Snow-capped Koch mountains
void mountains(int swidth, int sheight, double colorDark[], double colorLite[]) {
  double mt1[2], mt2[2], mt3[2], mt4[2];
  double darkFade[3], liteFade[3];

  // Calculate starting coordinates
  mt1[0] = swidth*(-0.4);
  mt1[1] = sheight*(0.1);
  mt2[0] = swidth*(0.6);
  mt2[1] = sheight*(0.1);
  mt3[0] = swidth*(0.4);
  mt3[1] = sheight*(0.1);
  mt4[0] = swidth*(1.1);
  mt4[1] = sheight*(0.1);

  // Convert colors to fraction for fade
  for (int i=0; i<3; ++i) {
    darkFade[i] = colorDark[i] / 255.0;
    liteFade[i] = colorLite[i] / 255.0;
  }

  // Draw mountains
  kochFlake(mt1, mt2, colorLite, 8);
  kochFlake(mt3, mt4, colorDark, 8);

  // Draw shadow
  cornerFade(darkFade, liteFade, 0.3*swidth);
}

// Fade color dark->lite from the origin to (xMax, yMax)
void cornerFade(double colDark[], double colLite[], double xMax) {
  double r, g, b, y, sc;
  double rShift, bShift, gShift;

  rShift = colLite[0] - colDark[0];
  gShift = colLite[1] - colDark[1];
  bShift = colLite[2] - colDark[2];

  y = 0;
  for (double x=0; x<=xMax; ++x) {
    sc = x / xMax;

    r = colDark[0] + sc*rShift;
    g = colDark[1] + sc*gShift;
    b = colDark[2] + sc*bShift;

    G_rgb(r,g,b) ;
    G_line(x, 0, 0, y);
    ++y;
  }
}

// Draw a solid hexagonal snowflake using Koch's curve
void kochFlake(double p1[], double p2[], double color[], int depth) {
  double p3[2], p4[2], p5[2], p6[2], pt[2];

  // Calculate p3
  pt[0] = p2[0] - p1[0];
  pt[1] = p2[1] - p1[1];
  p3[0] = p1[0] + pt[0] * cos(-piBy3) - pt[1] * sin(-piBy3);
  p3[1] = p1[1] + pt[1] * cos(-piBy3) + pt[0] * sin(-piBy3);
  
  // Draw snowflake
  kochCurve(p1, p2, color, 0, depth);
  kochCurve(p2, p3, color, 0, depth);
  kochCurve(p3, p1, color, 0, depth);

  // Fill in center
  Gi_rgb(color[0], color[1], color[2]);
  G_fill_triangle(p1[0], p1[1], p2[0], p2[1], p3[0], p3[1]);
}

// Recursively draw Koch's curve to a given depth
void kochCurve(double p1[], double p2[], double color[], int curr, int dep) {
  if (curr == dep) return;
  double pa[2], pb[2], pc[2], pt[2];

  // Calculate key points
  pa[0] = p1[0] + (1.0/3.0) * (p2[0] - p1[0]);
  pa[1] = p1[1] + (1.0/3.0) * (p2[1] - p1[1]);
  pt[0] = pa[0] - p1[0];
  pt[1] = pa[1] - p1[1];
  pb[0] = pa[0] + pt[0] * cos(piBy3) - pt[1] * sin(piBy3);
  pb[1] = pa[1] + pt[1] * cos(piBy3) + pt[0] * sin(piBy3);
  pc[0] = p1[0] + (2.0/3.0) * (p2[0] - p1[0]);
  pc[1] = p1[1] + (2.0/3.0) * (p2[1] - p1[1]);

  // Reinforce base line
  // and draw triangle
  if (curr+1 == dep) {
    G_rgb(1, 1, 1);  // snowcaps
  }
  G_line(p1[0], p1[1], p2[0], p2[1]);
  // Retain color for triangle
  Gi_rgb(color[0], color[1], color[2]);
  G_fill_triangle(pa[0], pa[1], pb[0], pb[1], pc[0], pc[1]);

  // Recurse
  kochCurve(p1, pa, color, curr+1, dep);
  kochCurve(pa, pb, color, curr+1, dep);
  kochCurve(pb, pc, color, curr+1, dep);
  kochCurve(pc, p2, color, curr+1, dep);
}

