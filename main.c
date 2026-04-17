#include<unistd.h>
#include<X11/Xlib.h>
#include<X11/extensions/Xdbe.h>
#include"geo.h"
#include"xlib.h"

#define SPHERE 1
#define TORUS 2
#define SURFACE 3

Display *display;
Window window;
XdbeBackBuffer back_buffer;
GC gc;
XEvent event;

// Sphere
double sphere_radius = 100;
int sphere_npar = 20;
int sphere_nmer = 10;
double sphere_rot_y = 0;

// Torus
double torus_radius = 100;
double torus_thickness = 40;
int torus_npar = 20;
int torus_nmer = 10;
double torus_rot_y = 0;

// Surface
double surface_width = 300;
double surface_height = 300;
int surface_columns = 60;
int surface_rows = 60;
double surface_origin_x = 0;
double surface_origin_y = 0;
double surface_phase = 0;
double surface_scale_h = 10;
double surface_scale_l = 50;

void draw(int state) {
  switch (state) {
  case SPHERE:
    draw_sphere(display, back_buffer, gc,
		sphere_radius, sphere_npar, sphere_nmer, sphere_rot_y);
    break;
  case TORUS:
    draw_torus(display, back_buffer, gc,
	       torus_radius, torus_thickness, torus_npar, torus_nmer, torus_rot_y);
    break;
  case SURFACE:
    draw_surface(display, back_buffer, gc,
		 surface_width, surface_height, surface_columns, surface_rows,
		 surface_origin_x, surface_origin_y, surface_phase, surface_scale_h, surface_scale_l);
    break;
  }
}

int main() {
  display = XOpenDisplay(NULL);
  window = XCreateSimpleWindow(display, DefaultRootWindow(display), 10, 10, 500, 500, 1, BlackPixel(display, 0), WhitePixel(display, 0));
  XMapWindow(display, window);

  back_buffer = XdbeAllocateBackBufferName(display, window, XdbeBackground);
  
  XSelectInput(display, window, ExposureMask | KeyPressMask | StructureNotifyMask);

  gc = XCreateGC(display, window, 0, NULL);

  XSetForeground(display, gc, BlackPixel(display, 0));

  /* for(;;) { */
  /*   XNextEvent(display, &event); */
  /*   if (event.type == MapNotify) */
  /*     break; */
  /* } */

  int state = SURFACE;
  double ty = 0;
  for(;;) {
    XSetForeground(display, gc, WhitePixel(display, 0));
    draw(state);

    ty = ty + 0.01;
    sphere_rot_y = ty;
    torus_rot_y = ty;
    surface_phase = ty;

    XSetForeground(display, gc, BlackPixel(display, 0));
    draw(state);

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
