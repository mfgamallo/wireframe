#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"geo.h"

void copy_point(Point p1, Point *p2) {
  p2->x = p1.x;
  p2->y = p1.y;
  p2->z = p1.z;
}

void point_rot_x(Point *p, double angle) {
  double z = p->z * cos(angle) - p->y * sin(angle);
  double y = p->z * sin(angle) + p->y * cos(angle);
  p->z = z;
  p->y = y;
}

void point_rot_y(Point *p, double angle) {
  double x = p->x * cos(angle) - p->z * sin(angle);
  double z = p->x * sin(angle) + p->z * cos(angle);
  p->x = x;
  p->z = z;
}

void point_rot_z(Point *p, double angle) {
  double x = p->x * cos(angle) - p->y * sin(angle);
  double y = p->x * sin(angle) + p->y * cos(angle);
  p->x = x;
  p->y = y;
}

void point_move(Point *p, double x, double y, double z) {
  p->x = p->x + x;
  p->y = p->y + y;
  p->z = p->z + z;
}

void seg_rot_x(Segment *seg, double angle) {
  point_rot_x(&seg->p1, angle);
  point_rot_x(&seg->p2, angle);
}

void seg_rot_y(Segment *seg, double angle) {
  point_rot_y(&seg->p1, angle);
  point_rot_y(&seg->p2, angle);
}

void seg_rot_z(Segment *seg, double angle) {
  point_rot_z(&seg->p1, angle);
  point_rot_z(&seg->p2, angle);
}

void points_rot_x(Point *points, int n, double angle) {
  for (int i = 0; i < n; i++) {
    point_rot_x(&points[i], angle);
  }
}

void points_rot_y(Point *points, int n, double angle) {
  for (int i = 0; i < n; i++) {
    point_rot_y(&points[i], angle);
  }
}

void points_rot_z(Point *points, int n, double angle) {
  for (int i = 0; i < n; i++) {
    point_rot_z(&points[i], angle);
  }
}

void points_move(Point *points, int n, double x, double y, double z) {
  for (int i = 0; i < n; i++) {
    point_move(&points[i], x, y, z);
  }
}


void segs_rot_x(Segment *segs, int n, double angle) {
  for (int i = 0; i < n; i++) {
    seg_rot_x(&segs[i], angle);
  }
}

void segs_rot_y(Segment *segs, int n, double angle) {
  for (int i = 0; i < n; i++) {
    seg_rot_y(&segs[i], angle);
  }
}

void segs_rot_z(Segment *segs, int n, double angle) {
  for (int i = 0; i < n; i++) {
    seg_rot_z(&segs[i], angle);
  }
}

Point *circle(double r, int n) {
  Point *ps = (Point *)malloc(sizeof(Point) * n);

  for (int i = 0; i < n; i++) {
    double alpha = ((2 * M_PI) / n) * i + M_PI;
    Point p;
    p.x = 0; p.y = r; p.z = 0;
    point_rot_z(&p, alpha);
    copy_point(p, &ps[i]);
  }

  return ps;
}

Point *point_sphere(double r, int npar, int nmer) {
  Point *ps = (Point *)malloc(sizeof(Point) * npar * nmer);

  for (int j = 0; j < npar; j++) {
    double alpha = ((2 * M_PI) / npar) * j + M_PI;
    Point p;
    p.x = 0; p.y = r; p.z = 0;
    point_rot_z(&p, alpha);
    Point *ring = circle(p.x, nmer);
    points_rot_x(ring, nmer, M_PI_2);
    for (int m = 0; m < nmer; m++) {
      int i = nmer * j + m;
      ps[i].x = ring[m].x;
      ps[i].y = p.y + ring[m].y;
      ps[i].z = ring[m].z;
    }
    free(ring);
  }

  return ps;
}

Point *point_torus(double r, double t, int npar, int nmer) {
  Point *ps = (Point *)malloc(sizeof(Point) * npar * nmer);

  for (int mer = 0; mer < nmer; mer++) {
    double alpha = ((2 * M_PI) / nmer) * mer;
    Point *ring = circle(t, npar);
    points_move(ring, npar, r, 0, 0);
    points_rot_y(ring, npar, alpha);
    for (int par = 0; par < npar; par++) {
      int i = mer * npar + par;
      copy_point(ring[par], &ps[i]);
    }
  }

  return ps;
}

Point *point_surface(double width,
		     double height,
		     int columns,
		     int rows,
		     double origin_x,
		     double origin_y,
		     double phase, // phase of the wave
		     double scale_h, // scale of the height of the wave
		     double scale_l // scale of the length of the wave
		     ) {
  Point *ps = (Point *)malloc(sizeof(Point) * columns * rows);

  double delta_x = width / columns;
  double delta_y = height / rows;
  for (int j = 0; j < rows; j++) {
    for (int i = 0; i < columns; i++) {
      int index = j * columns + i;
      ps[index].x = (-width/2) + delta_x * i;
      ps[index].y = (-height/2) + delta_y * j;
      // Calculates Z using the distance from the origin and the phase
      double distance = sqrt((pow(ps[index].x - origin_x, 2) + pow(ps[index].y - origin_y, 2)) / scale_l);
      ps[index].z = sin(distance + phase) * scale_h;
    }
  }

  return ps;
}

Segment *point_circle_to_segments(Point *circle, int n) {
  Segment *segs = (Segment *)malloc(sizeof(Segment) * n);

  for (int i = 0; i < n-1; i++) {
    copy_point(circle[i], &segs[i].p1);
    copy_point(circle[i+1], &segs[i].p2);
  }
  copy_point(circle[n-1], &segs[n-1].p1);
  copy_point(circle[0], &segs[n-1].p2);

  return segs;
}

Segment *point_sphere_to_segments(Point *sphere, int npar, int nmer) {
  Segment *segs = (Segment *)malloc(sizeof(Segment) * npar * nmer * 2 - nmer);

  int p = 0;
  for (int i = 0; i < npar-1; i++) {
    Segment *circle_segments = point_circle_to_segments(&sphere[i*nmer], nmer);
    for (int j = 0; j < nmer; j++) {
      segs[p++] = circle_segments[j];
      copy_point(sphere[i*nmer + j], &segs[p].p1);
      copy_point(sphere[i*nmer + nmer + j], &segs[p++].p2);
    }
  }
  Segment *circle_segments = point_circle_to_segments(&sphere[npar*nmer - nmer], nmer);
  for (int j = 0; j < nmer; j++)
    segs[p++] = circle_segments[j];

  return segs;
}

Segment *point_torus_to_segments(Point *torus, int npar, int nmer) {
  Segment *segs = (Segment *)malloc(sizeof(Segment) * npar * nmer * 2);

  int p = 0;
  for (int mer = 0; mer < nmer-1; mer++) {
    Segment *torus_segments = point_circle_to_segments(&torus[npar * mer], npar);
    for (int par = 0; par < npar; par++) {
      segs[p++] = torus_segments[par];
      copy_point(torus[npar * mer + par], &segs[p].p1);
      copy_point(torus[npar * mer + npar + par], &segs[p++].p2);
    }
  }
  Segment *torus_segments = point_circle_to_segments(&torus[npar * nmer - npar], npar);
  for (int par = 0; par < npar; par++) {
    segs[p++] = torus_segments[par];
    copy_point(torus[npar * (nmer - 1) + par], &segs[p].p1);
    copy_point(torus[par], &segs[p++].p2);
  }

  return segs;
}

Segment *point_surface_to_segments(Point *surface, int columns, int rows) {
  Segment *segs = (Segment *)malloc(sizeof(Segment) * (columns -1) * rows * 2);

  int p = 0;
  for (int row = 0; row < rows - 1; row++) {
    for (int column = 0; column < columns - 1; column++) {
      copy_point(surface[columns * row + column], &segs[p].p1);
      copy_point(surface[columns * row + column + 1], &segs[p++].p2);
      copy_point(surface[columns * row + column], &segs[p].p1);
      copy_point(surface[columns * (row + 1) + column], &segs[p++].p2);
    }
    copy_point(surface[columns * row + (columns - 1)], &segs[p].p1);
    copy_point(surface[columns * (row + 1) + (columns - 1)], &segs[p++].p2);
  }
  for (int column = 0; column < columns - 1; column++) {
    copy_point(surface[columns * (rows - 1) + column], &segs[p].p1);
    copy_point(surface[columns * (rows - 1) + column + 1], &segs[p++].p2);
  }

  return segs;
}

Segment *sphere(double r, int npar, int nmer) {
  Point *ps = point_sphere(r, npar, nmer);
  Segment *segs = point_sphere_to_segments(ps, npar, nmer);
  return segs;
}

Segment *torus(double r, double t, int npar, int nmer) {
  Point *ps = point_torus(r, t, npar, nmer);
  Segment *segs = point_torus_to_segments(ps, npar, nmer);
  return segs;
}

Segment *surface(double width,
		 double height,
		 int columns,
		 int rows,
		 double origin_x,
		 double origin_y,
		 double phase,
		 double scale_h,
		 double scale_l) {
  Point *ps = point_surface(width, height, columns, rows, origin_x, origin_y, phase, scale_h, scale_l);
  Segment *segs = point_surface_to_segments(ps, columns, rows);
  return segs;
}

int sphere_nsegs(int npar, int nmer) {
  return npar * nmer * 2 - nmer;
}

int torus_nsegs(int npar, int nmer) {
  return npar * nmer * 2;
}

int surface_nsegs(int columns, int rows) {
  return (columns -1) * rows * 2;
}
