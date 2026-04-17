
/*
 * Draws an array of segments on the screen
 * segs: the array of segments
 * n: length of the array
 */
void draw_segs(Display *display, Window window, GC gc, Segment *segs, int n);

/*
 * Draws a wireframe sphere
 * r: radius
 * npar: number of parallels
 * nmer: number of meridians
 * rot_y: rotation around the y axis
 */
void draw_sphere(Display *display, Window window, GC gc, double r, int npar, int nmer, double rot_y);

/*
 * Draws a wireframe torus
 * r: radius of the torus
 * t: thickness of the tube
 * npar: number of parallels
 * nmer: number of meridians
 * rot_y: rotation around the y axis
 */
void draw_torus(Display *display, Window window, GC gc, double r, double t, int npar, int nmer, double rot_y);

/*
 * Draws a surface
 * width
 * height
 * columns
 * rows
 * origin_x: X of the origin of the ripple
 * origin_y: Y of the origin of the ripple
 * phase: phase of the ripple
 * scale_h: height scale of the ripple
 * scale_l: length scale of the ripple
 */
void draw_surface(Display *display,
		  Window window,
		  GC gc,
		  double width,
		  double height,
		  int columns,
		  int rows,
		  double origin_x,
		  double origin_y,
		  double phase,
		  double scale_h,
		  double scale_l);
