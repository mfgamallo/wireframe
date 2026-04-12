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
