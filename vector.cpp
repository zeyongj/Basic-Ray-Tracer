#include "vector.h"
#include <math.h>

float vec_len(Vector u) {
  return sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}


float vec_dot(Vector p, Vector q) {
  return p.x * q.x + p.y * q.y + p.z * q.z;
}


Vector vec_plus(Vector p, Vector q) {
  Vector rc;
  rc.x = p.x + q.x;
  rc.y = p.y + q.y;
  rc.z = p.z + q.z;

  return rc;
}


Vector vec_minus(Vector p, Vector q) {
  Vector rc;
  rc.x = p.x - q.x;
  rc.y = p.y - q.y;
  rc.z = p.z - q.z;

  return rc;
}


Vector get_vec(Point q, Point p) {
  Vector rc;
  rc.x = p.x - q.x;
  rc.y = p.y - q.y;
  rc.z = p.z - q.z;

  return rc;
}

 
Point get_point(Point p, Vector q) {
  Point rc;
  rc.x = p.x + q.x;
  rc.y = p.y + q.y;
  rc.z = p.z + q.z;

  return rc;
}


void normalize(Vector *u) {
  float p;

  p = vec_len(*u);
  (*u).x = (*u).x / p;
  (*u).y = (*u).y / p;
  (*u).z = (*u).z / p;
}

Vector vec_scale(Vector p, float scale) {
  Vector ret;

  ret.x = scale * p.x;
  ret.y = scale * p.y;
  ret.z = scale * p.z;

  return ret;
}

RGB_float clr_add(RGB_float p, RGB_float q) {
  RGB_float ret;

  ret.r = p.r + q.r;
  ret.g = p.g + q.g;
  ret.b = p.b + q.b;

  return ret;
}

RGB_float clr_scale(RGB_float p, float s) {
  RGB_float ret;

  ret.r = s * p.r;
  ret.g = s * p.g;
  ret.b = s * p.b;

  return ret;
}

Vector vec_prod(Vector v1, Vector v2) {
	Vector v;
	v.x = v1.x*v2.x;
	v.y = v1.y*v2.y;
	v.z = v1.z*v2.z;
	return v;
}
