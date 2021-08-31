
#include"include/vec.h"

typedef struct sphere {
  int index;        

  vec3 center;
  float radius;

  vec3 mat_ambient;    
  vec3 mat_diffuse;
  vec3 mat_specular;
  float mat_shineness;

  float reflectance;      
  float refr;
  float refractivity;
  
  struct sphere *next;
} Spheres;  

float intersect_plane(vec3 o, vec3 u, vec3 *hit);

float intersect_sphere(vec3 o, vec3 u, Spheres *sph, vec3 *hit);

void *intersect_scene(vec3, vec3, Spheres *, vec3 *, int *);

vec3 sphere_normal(vec3, Spheres *);

Spheres *add_sphere(Spheres *, vec3, float, vec3 , vec3, vec3, float, float, float, float, int);

struct plane {
	vec3 leftbottom;
	vec3 righttop;
	float reflectance;
	
	vec3 mat_ambient;
	vec3 mat_diffuse1;
	vec3 mat_diffuse2;
	vec3 mat_specular;
	float shineness;
};

float intersect_plane(vec3, vec3, vec3*);
const float precision = 0.00001;
const float t_precise = 0.001;

