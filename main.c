#include<stdio.h>
#include<unistd.h>
#include<X11/Xlib.h>
#include<X11/extensions/Xdbe.h>
#include"geo.h"
#include"xlib.h"
#include"state.h"

Display *display;
Window window;
XdbeBackBuffer back_buffer;
GC gc;
XEvent event;

void draw(State state) {
  switch (state.type) {
  case SPHERE:
    draw_sphere(display,
		back_buffer,
		gc,
		state.sphere.radius,
		state.sphere.npar,
		state.sphere.nmer,
		state.sphere.rot_y);
    break;
  case TORUS:
    draw_torus(display,
	       back_buffer,
	       gc,
	       state.torus.radius,
	       state.torus.thickness,
	       state.torus.npar,
	       state.torus.nmer,
	       state.torus.rot_y);
    break;
  case SURFACE:
    draw_surface(display,
		 back_buffer,
		 gc,
		 state.surface.width,
		 state.surface.height,
		 state.surface.columns,
		 state.surface.rows,
		 state.surface.origin_x,
		 state.surface.origin_y,
		 state.surface.phase,
		 state.surface.scale_h,
		 state.surface.scale_l);
    break;
  }
}

void alter(State *state, int keycode) {
  if (keycode == 0x18) { // q
    state->type = SPHERE;
  } else if (keycode == 0x19) { // w
    state->type = TORUS;
  } else if (keycode == 0x1a) { // e
    state->type = SURFACE;
  } else if (keycode == 0x26) { // a
    switch (state->type) {
    case SPHERE:
      state->sphere.npar += 1;
      break;
    case TORUS:
      state->torus.npar += 1;
      break;
    case SURFACE:
      state->surface.rows += 1;
      break;
    }
  } else if (keycode == 0x34) { // z
    switch (state->type) {
    case SPHERE:
      state->sphere.npar -= 1;
      break;
    case TORUS:
      state->torus.npar -= 1;
      break;
    case SURFACE:
      state->surface.rows -= 1;
      break;
    }
  } else if (keycode == 0x27) { // s
    switch (state->type) {
    case SPHERE:
      state->sphere.nmer += 1;
      break;
    case TORUS:
      state->torus.nmer += 1;
      break;
    case SURFACE:
      state->surface.columns += 1;
      break;
    }
  } else if (keycode == 0x35) { // x
    switch (state->type) {
    case SPHERE:
      state->sphere.nmer -= 1;
      break;
    case TORUS:
      state->torus.nmer -= 1;
      break;
    case SURFACE:
      state->surface.columns -= 1;
      break;
    }
  }
}

Bool has_key_event(Display *display, XEvent *event, XPointer arg) {
    if (event->type == KeyPress) {
        printf("KeyPress: %x\n", event->xkey.keycode);
        return True;
    }
    return False;
}

int main() {
  display = XOpenDisplay(NULL);
  window = XCreateSimpleWindow(display, DefaultRootWindow(display), 10, 10, 500, 500, 1, BlackPixel(display, 0), WhitePixel(display, 0));
  XMapWindow(display, window);

  back_buffer = XdbeAllocateBackBufferName(display, window, XdbeBackground);
  
  XSelectInput(display, window, ExposureMask | KeyPressMask | StructureNotifyMask);

  gc = XCreateGC(display, window, 0, NULL);

  XSetForeground(display, gc, BlackPixel(display, 0));

  State state = init_state();

  double ty = 0;

  for(;;) {
    XSetForeground(display, gc, WhitePixel(display, 0));
    draw(state);

    if (XCheckIfEvent(display, &event, has_key_event, (XPointer)NULL)) {
      if (event.xkey.keycode == 0x09) {
	break;
      } else {
	alter(&state, event.xkey.keycode);
      }
    }

    ty = ty + 0.01;
    state.sphere.rot_y = ty;
    state.torus.rot_y = ty;
    state.surface.phase = - ty * 10;

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
