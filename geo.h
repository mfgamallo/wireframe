typedef struct point {
  double x;
  double y;
  double z;
} Point;

typedef struct segment {
  Point p1;
  Point p2;
} Segment;

/*
 * Rotates an array of segments arount the X axis
 * segs: pointer to the array of segments
 * n: number of segments
 * angle: angle
 */
void segs_rot_x(Segment *segs, int n, double angle);

/*
 * Rotates an array of segments arount the Y axis
 * segs: pointer to the array of segments
 * n: number of segments
 * angle: angle
 */
void segs_rot_y(Segment *segs, int n, double angle);

/*
 * Rotates an array of segments arount the Z axis
 * segs: pointer to the array of segments
 * n: number of segments
 * angle: angle
 */
void segs_rot_z(Segment *segs, int n, double angle);

/*
 * Returns a pointer to an array of segments representing
 * a sphere
 * r: radius
 * npar: number of parallels
 * nmer: number or meridians
 */
Segment *sphere(double r, int npar, int nmer);

/*
 * Returns a pointer to an array of segments representing
 * a torus
 * r: radius
 * t: thickness
 * npar: number of parallels
 * nmer: number or meridians
 */
Segment *torus(double r, double t, int npar, int nmer);

/*
 * Returns a pointer to an array of segments representing
 * a surface. The surface will have a wave starting from
 * origin_x, origin_y with phase and scaled
 * width
 * height
 * columns
 * rows
 * origin_x
 * origin_y
 * phase
 * scale_h: scale of the height of the wave
 * scale_l: scale of the length of the wave
 */
Segment *surface(double width,
		 double height,
		 int columns,
		 int rows,
		 double origin_x,
		 double origin_y,
		 double phase,
		 double scale_h,
		 double scale_l);

/*
 * Returns the number of segments in a sphere given
 * the number of parallels and meridians
 */
int sphere_nsegs(int npar, int nmer);

/*
 * Returns the number of segments in a torus given
 * the number of parallels and meridians
 */
int torus_nsegs(int npar, int nmer);

/*
 * Returns the number of segments in a surface given the
 * number of columns and rows
 */
int surface_nsegs(int columns, int rows);
