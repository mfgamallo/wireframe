#include<X11/Xlib.h>
#include"geo.h"

#define Z_FACTOR 500

double project_x(Point p) {
  return 250 + (p.x / (1 + p.z / Z_FACTOR));
}

double project_y(Point p) {
  return 250 + (p.y / (1 + p.z / Z_FACTOR));
}

void draw_segs(Display *display, Window window, GC gc, Segment *segs, int n) {
  for (int i = 0; i < n; i++) {
    double x1 = project_x(segs[i].p1);
    double y1 = project_y(segs[i].p1);
    double x2 = project_x(segs[i].p2);
    double y2 = project_y(segs[i].p2);
    XDrawLine(display, window, gc, x1, y1, x2, y2);
  }
}

void draw_sphere(Display *display, Window window, GC gc, double r, int npar, int nmer, double rot_y) {
  int nsegs = sphere_nsegs(npar, nmer);
  Segment *segs = sphere(r, npar, nmer);
  segs_rot_y(segs, nsegs, rot_y);
  segs_rot_x(segs, nsegs, -0.8);
  draw_segs(display, window, gc, segs, nsegs);
}

void draw_torus(Display *display, Window window, GC gc, double r, double t, int npar, int nmer, double rot_y) {
  int nsegs = torus_nsegs(npar, nmer);
  Segment *segs = torus(r, t, npar, nmer);
  segs_rot_y(segs, nsegs, rot_y);
  segs_rot_x(segs, nsegs, -0.4);
  draw_segs(display, window, gc, segs, nsegs);
}

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
		  double scale_l) {
  int nsegs = surface_nsegs(columns, rows);
  Segment *segs = surface(width, height, columns, rows, origin_x, origin_y, phase, scale_h, scale_l);
  segs_rot_x(segs, nsegs, 0.8);
  draw_segs(display, window, gc, segs, nsegs);
}
