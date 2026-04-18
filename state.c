#include"state.h"

State init_state() {

  State state;

  // Sphere
  state.sphere.radius = 100;
  state.sphere.npar = 20;
  state.sphere.nmer = 10;
  state.sphere.rot_y = 0;
  
  // Torus
  state.torus.radius = 100;
  state.torus.thickness = 40;
  state.torus.npar = 20;
  state.torus.nmer = 10;
  state.torus.rot_y = 0;
  
  // Surface
  state.surface.width = 300;
  state.surface.height = 300;
  state.surface.columns = 10;
  state.surface.rows = 10;
  state.surface.origin_x = 0;
  state.surface.origin_y = 0;
  state.surface.phase = 0;
  state.surface.scale_h = 10;
  state.surface.scale_l = 50;

  state.type = SPHERE;
  
  return state;
}

State copy_state(State state) {

  State new_state;

  // Sphere
  new_state.sphere.radius = state.sphere.radius;
  new_state.sphere.npar = state.sphere.npar;
  new_state.sphere.nmer = state.sphere.nmer;
  new_state.sphere.rot_y = state.sphere.rot_y;
  
  // Torus
  new_state.torus.radius = state.torus.radius;
  new_state.torus.thickness = state.torus.thickness;
  new_state.torus.npar = state.torus.npar;
  new_state.torus.nmer = state.torus.nmer;
  new_state.torus.rot_y = state.torus.rot_y;
  
  // Surface
  new_state.surface.width = state.surface.width;
  new_state.surface.height = state.surface.height;
  new_state.surface.columns = state.surface.columns;
  new_state.surface.rows = state.surface.rows;
  new_state.surface.origin_x = state.surface.origin_x;
  new_state.surface.origin_y = state.surface.origin_y;
  new_state.surface.phase = state.surface.phase;
  new_state.surface.scale_h = state.surface.scale_h;
  new_state.surface.scale_l = state.surface.scale_l;

  new_state.type = state.type;
  
  return new_state;
}
