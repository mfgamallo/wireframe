#define SPHERE 1
#define TORUS 2
#define SURFACE 3

typedef struct sphere_state {
  double radius;
  int npar;
  int nmer;
  double rot_y;
} SphereState;

typedef struct torus_state {
  double radius;
  double thickness;
  int npar;
  int nmer;
  double rot_y;
} TorusState;

typedef struct surface_state {
  double width;
  double height;
  int columns;
  int rows;
  double origin_x;
  double origin_y;
  double phase;
  double scale_h;
  double scale_l;
} SurfaceState;

typedef struct aState {
  int type; // Either a sphere, a torus or a surface
  SphereState sphere;
  TorusState torus;
  SurfaceState surface;
} State;

/*
 * Returns a new State with default values
 */
State init_state();

/*
 * Returns a new State with the same values as the one provided
 */
State copy_state(State state);
