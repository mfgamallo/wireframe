#include<unistd.h>
#include<X11/Xlib.h>
#include<X11/extensions/Xdbe.h>
#include"geo.h"
#include"xlib.h"

Display *display;
Window window;
XdbeBackBuffer back_buffer;
GC gc;
XEvent event;

// Torus
#define RADIUS 100
#define THICKNESS 40
#define PAR 50
#define MER 50

// Surface
#define WIDTH 300
#define HEIGHT 300
#define COLUMNS 60
#define ROWS 60
#define ORIGIN_X 0
#define ORIGIN_Y 0
#define SCALE_H 10
#define SCALE_L 50

void draw_sphere(double r, int npar, int nmer, double rot_y) {
  int nsegs = sphere_nsegs(npar, nmer);
  Segment *segs = sphere(r, npar, nmer);
  segs_rot_y(segs, nsegs, rot_y);
  segs_rot_x(segs, nsegs, 0.8);
  draw_segs(display, back_buffer, gc, segs, nsegs);
}

void draw_torus(double r, double t, int npar, int nmer, double rot_y) {
  int nsegs = torus_nsegs(npar, nmer);
  Segment *segs = torus(r, t, npar, nmer);
  segs_rot_y(segs, nsegs, rot_y);
  segs_rot_x(segs, nsegs, 0);
  draw_segs(display, back_buffer, gc, segs, nsegs);
}

void draw_surface(double width,
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
  draw_segs(display, back_buffer, gc, segs, nsegs);
}

int main() {
  display = XOpenDisplay(NULL);
  window = XCreateSimpleWindow(display, DefaultRootWindow(display), 10, 10, 500, 500, 1, BlackPixel(display, 0), WhitePixel(display, 0));
  XMapWindow(display, window);

  back_buffer = XdbeAllocateBackBufferName(display, window, XdbeBackground);
  
  XSelectInput(display, window, ExposureMask | KeyPressMask | StructureNotifyMask);

  gc = XCreateGC(display, window, 0, NULL);

  XSetForeground(display, gc, BlackPixel(display, 0));

  for(;;) {
    XNextEvent(display, &event);
    if (event.type == MapNotify)
      break;
  }

  double ty = 0;
  for(;;) {
    XSetForeground(display, gc, WhitePixel(display, 0));
    //draw_sphere(RADIUS, PAR, MER, ty);
    //draw_torus(RADIUS, THICKNESS, PAR, MER, ty);
    draw_surface(WIDTH, HEIGHT, COLUMNS, ROWS, ORIGIN_X, ORIGIN_Y, ty, SCALE_H, SCALE_L);
    ty = ty - 0.1;
    XSetForeground(display, gc, BlackPixel(display, 0));
    //draw_sphere(RADIUS, PAR, MER, ty);
    //draw_torus(RADIUS, THICKNESS, PAR, MER, ty);
    draw_surface(WIDTH, HEIGHT, COLUMNS, ROWS, ORIGIN_X, ORIGIN_Y, ty, SCALE_H, SCALE_L);

    XdbeSwapInfo swap_info;
    swap_info.swap_window = window;
    swap_info.swap_action = XdbeBackground;
    XdbeSwapBuffers(display, &swap_info, 1);
    
    XFlush(display);
    usleep(20000);
  }

  XdbeDeallocateBackBufferName(display, back_buffer);
  XDestroyWindow(display, window);
  XCloseDisplay(display);
  return 0;
}
