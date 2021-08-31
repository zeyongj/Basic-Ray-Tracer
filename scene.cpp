// this provide functions to set up the scene
#include "sphere.h"
#include <stdio.h>

extern vec3 light1;
extern vec3 light1_intensity;
extern float light1_shadow;

extern vec3 global_ambient;
extern Spheres* scene;

extern vec3 background_clr;
extern float decay_a;
extern float decay_b;
extern float decay_c;
/*******************************************
 * set up the default scene 
 *******************************************/
void set_up_default_scene() {
  scene = NULL;
  // set background color
  background_clr.x = 0.5;
  background_clr.y = 0.5;
  background_clr.z = 0.5;

  global_ambient.x = 0.2;
  global_ambient.y = 0.2;
  global_ambient.z = 0.2;


  light1.x = -2.0;
  light1.y = 5.0;
  light1.z = 1.0;
  light1_intensity.x = 1.0;
  light1_intensity.y = 1.0;
  light1_intensity.z = 1.0;
  light1_shadow = 0.2;

  decay_a = 0.5;
  decay_b = 0.3;
  decay_c = 0.0;

  // sphere 1
  vec3 sphere1_ctr = vec3(1.1, 0, -2);
  float sphere1_rad = 0.5;
  vec3 sphere1_ambient = vec3(0.7, 0.7, 0.7);
  vec3 sphere1_diffuse = vec3(0.8, 0.1, 0.8);
  vec3 sphere1_specular = vec3(1.0, 1.0, 1.0);
  float sphere1_shineness = 10;
  float sphere1_reflectance = 0.4;
  float sphere1_refr = 0;
  float sphere1_refractivity = 1.3;

  scene = add_sphere(scene, sphere1_ctr, sphere1_rad, sphere1_ambient,
            sphere1_diffuse, sphere1_specular, sphere1_shineness,
		     sphere1_reflectance, sphere1_refr, sphere1_refractivity, 1);

  // sphere 2
  vec3 sphere2_ctr = vec3(-2.5, 0.0, -2.25);
  float sphere2_rad = 0.5;
  vec3 sphere2_ambient = vec3(0.8, 0.6, 0.6);
  vec3 sphere2_diffuse = vec3(0.0, 0.0, 0.25);
  vec3 sphere2_specular = vec3(1.0, 1.0, 1.0);
  float sphere2_shineness = 6;
  float sphere2_reflectance = 0.3;
  float sphere2_refr = 0;
  float sphere2_refractivity = 0.8;

  scene = add_sphere(scene, sphere2_ctr, sphere2_rad, sphere2_ambient,
            sphere2_diffuse, sphere2_specular, sphere2_shineness,
		     sphere2_reflectance, sphere2_refr, sphere2_refractivity, 2);

  // sphere 3
  vec3 sphere3_ctr = vec3(0.0, -0.2, -0.9);
  float sphere3_rad = 0.5;
  vec3 sphere3_ambient = vec3(0.6, 0.6, 0.6);
  vec3 sphere3_diffuse = vec3(0.1, 0.3, 0.5);
  vec3 sphere3_specular = vec3(1.0, 1.0, 1.0);
  float sphere3_shineness = 3;
  float sphere3_reflectance = 0.1;
  float sphere3_refr = 0.3;
  float sphere3_refractivity = 1.1;

  scene = add_sphere(scene, sphere3_ctr, sphere3_rad, sphere3_ambient,
            sphere3_diffuse, sphere3_specular, sphere3_shineness,
		     sphere3_reflectance, sphere3_refr, sphere3_refractivity, 3);

}
