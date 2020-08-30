/*
Alex Staley - CS410P - July 2020

  AN L-SYSTEM TREE GUARDING
  A WINDSWEPT DESERT ROAD
*/

#include  "FPToolkit.c"
const double piBy2 = M_PI / 2.0;
const double piBy3 = M_PI / 3.0;
const double piBy4 = M_PI / 4.0;
const double piBy6 = M_PI / 6.0;
const double piBy8 = M_PI / 8.0;

#define MAX_SIZE 1000000

void stringWrapper();
void stackBuilder();
void stringBuilder(int curr, int depth);
void stringInterpreter(int start[2], double length, double angle, double gangle);
void autoFit(int swidth, int sheight, double angle, double gangle, double * placement);
void pushState();
void popState();

// Production: nonterminal -> rule
typedef struct {
  char nonterminal;
  char rule[100];
} Production;

// Stacks to track current state
typedef struct {
  double x[MAX_SIZE]; //x location
  double y[MAX_SIZE]; //y location
  double a[MAX_SIZE]; //heading (radians)
  int xI;
  int yI;
  int aI;
} Stack;

// Defined globally:
Stack stack;
Production prods[10]; //array of <=10 productions
int numProds = 0;     //number of prods defined
char axiom[2] = {'A', '\0'};       //starting string
char derivation[MAX_SIZE] = {'\0'}; //derived string
double heading = 0; //current angle from zero in radians
double here[2];     //current (x, y) position


int main() {
  int start[2];
  double placement[3];
  double gangle = piBy2; //DEFINE GANGLE HERE

  int depth;
  double length;

  // Set up
  srand(time(0));
  int key = 0;
  int swidth = 746; int sheight = 746;
  double horizon = sheight * 0.618;
  double road = swidth * 0.618;
  //G_choose_repl_display(); //for repl
  G_init_graphics (swidth,sheight);

  // Background
  Gi_rgb(217, 191, 119);
  G_clear();
  Gi_rgb(178, 235, 242);
  for (double i=horizon; i<=sheight; ++i) {
    G_line(0, i, swidth, i);
  }

  // Road
  Gi_rgb(18, 13, 2);
  for (double r=road; r<swidth*1.618; ++r) {
    if (r > swidth*0.99) Gi_rgb(245, 201, 13);
    if (r > swidth * 1.03) Gi_rgb(18, 13, 2);
    if (r > swidth*1.06) Gi_rgb(245, 201, 13);
    if (r > swidth * 1.10) Gi_rgb(18, 13, 2);
    G_line(road + r*0.01, horizon, r, 0);
  }
  G_line(road, 0, road, horizon);
  G_line(road, 0, road+1, horizon);
  G_line(road, 0, road+2, horizon);
  G_line(road, 0, road+3, horizon);

  int xSand, ySand;
  Gi_rgb(217, 191, 119);
  for (int i=0; i<700; ++i) {
    xSand = rand() % ((int)(swidth*1.618)) + swidth*0.6;
    ySand = rand() % ((int)horizon);
    G_point(xSand, ySand);
  }

  // Derive and build string
  stringWrapper();
  stackBuilder();

  // Determine line dimensions
  autoFit(swidth, sheight, piBy8, gangle, placement);
  start[0] = placement[0] - (swidth / 9.0); //scoot for road
  start[1] = placement[1];
  length = placement[2];

  // Draw tree
  Gi_rgb(43, 88, 12);
  stringInterpreter(start, length, piBy8, gangle);

  // Draw shadow
  length = length * 0.6;
  gangle = piBy2 + piBy6;
  Gi_rgb(6, 13, 2);
  stringInterpreter(start, length, piBy8, gangle);

  // Display and save image
  key = G_wait_key();
  G_save_to_bmp_file("desertLife.bmp");

  return 0;
}

// Populate the global prods[] array.
// DEFINE PRODUCTIONS HERE!
void stringWrapper(/*int numProds, Production prods[]*/) {
  // Define nonterminals and
  // associated derivations,
  // then increment numProds
  prods[0].nonterminal = 'A';
  strcpy(prods[0].rule, "B-[[A]+A]+B[+BA]-A");
  ++numProds;

  prods[1].nonterminal = 'B';
  strcpy(prods[1].rule, "BB");
  ++numProds;

  // Build string
  stringBuilder(0, 6);
}

void stackBuilder() {
  stack.x[0] = '\0';
  stack.y[0] = '\0';
  stack.a[0] = '\0';
  stack.xI = -1;
  stack.yI = -1;
  stack.aI = -1;
}

void pushState() {
  if (stack.xI < MAX_SIZE-1) {
    stack.xI += 1;
    stack.x[stack.xI] = here[0];
  }
  if (stack.yI < MAX_SIZE-1) {
    stack.yI += 1;
    stack.y[stack.yI] = here[1];
  }
  if (stack.aI < MAX_SIZE-1) {
    stack.aI += 1;
    stack.a[stack.aI] = heading;
  }
}

void popState() {
  if (stack.xI >=0) {
    here[0] = stack.x[stack.xI];
    stack.xI -=1;
  }
  if (stack.yI >= 0) {
    here[1] = stack.y[stack.yI];
    stack.yI -= 1;
  }
  if (stack.aI >= 0) {
    heading = stack.a[stack.aI];
    stack.aI -= 1;
  }
}

// Determine the length and starting point
// to ideally fit the screen, given its dimensions
void autoFit(int swidth, int sheight, double angle, double gangle, double * placement) {
  double xMin=0; double yMin=0;
  double xMax=0; double yMax=0;
  double deltaX=0; double deltaY=0;
  double newDeltaX = 0.9*swidth;  //this,
  double newDeltaY = 0.9*sheight; //XOR this
  double there[2];  //destination point container

  int i = 0;
  heading = gangle;
  here[0] = 0;
  here[1] = 0;

  // Loop over derived string
  while (derivation[i] != '\0') {
    if (derivation[i] == '[') { //push state
      pushState();
    }
    else if (derivation[i] == ']') { //pop state
      popState();
    }
    else if (derivation[i] == '-') { //turn clockwise
      heading -= angle;
    }
    else if (derivation[i] == '+') { //turn counterclockwise
      heading += angle;
    }
    else if ((derivation[i] >= 'A' && derivation[i] <='Z')
        || derivation[i] == 'f') { //move forward
      there[0] = here[0] + cos(heading);
      there[1] = here[1] + sin(heading);
      here[0] = there[0];
      here[1] = there[1];

      // Check current location against current extrema
      if (here[0] < xMin) xMin = here[0];
      if (here[0] > xMax) xMax = here[0];
      if (here[1] < yMin) yMin = here[1];
      if (here[1] > yMax) yMax = here[1];
    }

    ++i;
  }

  // Calculate scaling factors
  deltaX = xMax - xMin;
  deltaY = yMax - yMin;
  if (deltaY > deltaX) {
    newDeltaX = deltaX * (newDeltaY / deltaY);
  }
  else {
    newDeltaY = deltaY * (newDeltaX / deltaX);
  }

  // Calculate starting point and length,
  // accounting for negative min values
  placement[0] = 0.5 * (swidth - newDeltaX);  //startX
  if (xMin < 0) placement[0] -= (xMin * (newDeltaX/deltaX));
  placement[1] = 0.5 * (sheight - newDeltaY); //startY
  if (yMin < 0) placement[1] -= (yMin * (newDeltaY/deltaY));
  placement[2] = newDeltaX / deltaX;          //length
}

void stringInterpreter(int start[2], double length, double angle, double gangle) {
  heading = gangle;
  here[0] = start[0];
  here[1] = start[1];
  double there[2];
  int i = 0;

  // Loop over derived string
  while (derivation[i] != '\0') {
    if (derivation[i] == '[') { //push state
      pushState();
    }
    else if (derivation[i] == ']') { //pop state
      popState();
    }
    else if (derivation[i] == '-') { //turn clockwise
      heading -= angle;
    }
    else if (derivation[i] == '+') { //turn counterclockwise
      heading += angle;
    }
    else if ((derivation[i] >= 'A' && derivation[i] <='Z')
        || derivation[i] == 'f') { //move forward
      there[0] = here[0] + length * cos(heading);
      there[1] = here[1] + length * sin(heading);
      G_line(here[0], here[1], there[0], there[1]);
      here[0] = there[0];
      here[1] = there[1];
    }

    ++i;
  }
}

// Derive a string to the given depth
// MUST DEFINE PRODS W/ stringWrapper() BEFORE CALLING!
void stringBuilder(int curr, int depth) {
  if (derivation[0] == '\0') {
    // Start with global axiom
    strcpy(derivation, axiom);
  }

  if (curr == depth) return;

  int sI=0;  int pI=0; //starting string, prod list indices
  int derived=0; //flag for nonterminal found this iteration
  char let[2];  let[1] = '\0'; //current letter as a string
  char temp[MAX_SIZE]; //temporary derived string container

  // Loop over existing string
  while (derivation[sI] != '\0') {
    let[0] = derivation[sI];
    // Loop over all possible productions
    while (pI < numProds && derived == 0) {
      // If a nonterminal is found,
      // apply the appropriate rule
      if (derivation[sI] == prods[pI].nonterminal) {
        strcat(temp, prods[pI].rule);
        derived = 1;
      }
      ++pI;
    }
    // If the character is a
    // terminal, copy it to temp
    if (derived == 0) strcat(temp, let);

    // Increment/reset loop indices
    ++sI;
    pI = 0;
    derived = 0;
  }

  // Copy derived string and recurse
  strcpy(derivation, temp);
  stringBuilder(curr+1, depth);
}

