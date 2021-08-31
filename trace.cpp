#include <stdio.h>
#include <math.h>
#include "global.h"
#include "sphere.h"
//
// Global variables
//
extern int win_width;
extern int win_height;

extern GLfloat frame[WIN_HEIGHT][WIN_WIDTH][3];  

extern float image_width;
extern float image_height;

extern vec3 eye_pos;
extern float image_plane;
extern vec3 background_clr;
extern vec3 null_clr;

extern Spheres *scene;

extern vec3 light1;
extern vec3 light1_intensity;
extern float light1_shadow;

extern vec3 global_ambient;

extern float decay_a;
extern float decay_b;
extern float decay_c;

extern int shadow_on;
extern int reflect_on;
extern int board_on;
extern int refract_on;
extern int diffuse_reflection_on;
extern int antialias_on;


extern struct plane pl;
vec3 rotateX(float theta, vec3 v) {
	vec3 rv = vec3(0,0,0);
	rv.x = v.x;
	rv.y = cos(theta)*v.y - sin(theta)*v.z;
	rv.z = sin(theta)*v.y + cos(theta)*v.z;
	return rv;
}
vec3 rotateY(float theta, vec3 v) {
	vec3 rv = vec3(0,0,0);
	rv.y = v.y;
	rv.x = cos(theta)*v.x + sin(theta)*v.z;
	rv.z = -1*sin(theta)*v.x + cos(theta)*v.z;
	return rv;
}
vec3 rotateZ(float theta, vec3 v) {
	vec3 rv = vec3(0,0,0);
	rv.z = v.z;
	rv.x = cos(theta)*v.x - sin(theta)*v.y;
	rv.y = sin(theta)*v.x + cos(theta)*v.y;
	return rv;
}
//float precision = 0.00001;
float max_float(float a, float b) {
	if(a<=b) return b;
	return a;
}

vec3 phong(vec3 eye, vec3 ray, vec3 surf_norm, void *sph, vec3 hit, int isplane) {

	vec3 color=vec3(0,0,0);
	vec3 raynormal = normalize(ray);
	float dist = dot(ray, ray);
	vec3 diffuse;
	vec3 specular;
	vec3 reflect_vector = 2*dot(raynormal, surf_norm)*surf_norm - raynormal;

	if(isplane==1) {
		struct plane* pln= (struct plane*)sph;
		color += global_ambient * pln->mat_ambient;
		
		if(hit.x>=0) {
			if((int(hit.x)+int(hit.z))%2==0) diffuse = max_float(dot(raynormal,surf_norm),0)*light1_intensity*pln->mat_diffuse1;
			else diffuse = max_float(dot(raynormal,surf_norm),0)*light1_intensity*pln->mat_diffuse2;
		}
		else {
			if((int(hit.x)-1+int(hit.z))%2==0) diffuse = max_float(dot(raynormal,surf_norm),0)*light1_intensity*pln->mat_diffuse1;
			else diffuse = max_float(dot(raynormal,surf_norm),0)*light1_intensity*pln->mat_diffuse2;
		}
		specular = pow(max_float(dot(reflect_vector,eye),0), pln->shineness)*light1_intensity*pln->mat_specular;
	}
	else {
		Spheres *sphr = (Spheres *)sph;
		color += global_ambient * sphr->mat_ambient;

		diffuse = max_float(dot(raynormal, surf_norm),0)*light1_intensity*sphr->mat_diffuse;

		specular = pow(max_float(dot(reflect_vector, eye),0),sphr->mat_shineness)*light1_intensity * sphr->mat_specular;

	}

	color += 1.0 * (diffuse+specular)/(decay_c*dist+decay_b*sqrt(dist)+decay_a);
	return color;
}
//compute the shadow
vec3 get_shadow(vec3 eye, vec3 ray, vec3 surf_norm, void *sph, vec3 hit, int isplane) {

	vec3 raynormal = normalize(ray);
	float dist = dot(ray, ray);
	vec3 color=vec3(0,0,0);
	vec3 reflect_vector = 2*dot(raynormal, surf_norm)*surf_norm - raynormal;
	vec3 diffuse;
	vec3 specular;

	if(isplane==1) {
		struct plane *pln = (struct plane *)sph;
		if((int(hit.x) + int(hit.z))%2==0) diffuse=max_float(dot(raynormal,surf_norm),0)*light1_intensity*pln->mat_diffuse1;
		else  diffuse=max_float(dot(raynormal,surf_norm),0)*light1_intensity*pln->mat_diffuse2;
		specular = pow(max_float(dot(reflect_vector,eye),0), pln->shineness)*light1_intensity*pln->mat_specular;
	}
	else {
		Spheres *sphr= (Spheres *)sph;
		color += global_ambient * sphr->mat_ambient;
		diffuse = max_float(dot(raynormal, surf_norm),0)*light1_intensity*sphr->mat_diffuse;
		specular = pow(max_float(dot(reflect_vector, eye),0),sphr->mat_shineness)*light1_intensity * sphr->mat_specular;
	}
	
	color += 1.0 * light1_shadow *(diffuse+specular)/(decay_c*dist+decay_b*sqrt(dist)+decay_a);
	return color;

}

bool refract_light(vec3 normal, vec3 inlight, float ratio, vec3* outlight) {
	float root = 1 - pow(ratio, 2)*(1-pow(dot(normal, inlight),2)); 
	if(root<precision) {
		return false; 
	}
	*outlight =  (ratio*dot(normal,inlight) - sqrt(root)) * normal - ratio*inlight;
	return true;
}
// computation of the input point/dir and output point/dir of the refractor light
bool refraction(vec3 inpoint, vec3 invector, void* obj, int isplane, bool inobj, vec3* outlightvector) {

	if(isplane) return false;

	Spheres * sph = (Spheres *)obj;
	//intermediate light
	vec3 in_normal = sphere_normal(inpoint, sph);
	float ratio;
	if(!inobj) ratio = 1.0/sph->refractivity;
	else ratio = sph->refractivity;
	
	if(!refract_light(in_normal, invector, ratio, outlightvector)) return false;
	return true;
}


vec3 recursive_ray_trace(vec3 eye, vec3 ray, int num, bool inobj) {

	
	vec3 hit;
	int isplane;
	void *sph = intersect_scene(eye, ray, scene, &hit, &isplane);

	vec3 color = null_clr;
	if(sph==NULL) { 
		return background_clr;
	}

	vec3 lightvec = light1 - hit;
	vec3 lightvec_normal = normalize(lightvec);
	vec3 lighthit;
	int lightisplane;
	void * light_sph = intersect_scene(hit, lightvec_normal, scene, &lighthit, &lightisplane);
	
	vec3 surf_normal = isplane?vec3(0,1,0):sphere_normal(hit, (Spheres*)sph);

	if(light_sph==NULL) {
		color += phong(-1*ray, lightvec, surf_normal, sph, hit, isplane);
	}
	else {
		if(!shadow_on) {
			color += phong(-1*ray, lightvec, surf_normal, sph, hit, isplane);
		}
		else {
			color += get_shadow(-1*ray, lightvec, surf_normal, sph, hit, isplane);
		}
	}

	if(reflect_on) {
		vec3 reflect_vector = 2*dot(-1*ray, surf_normal)*surf_normal + ray;
		reflect_vector = normalize(reflect_vector);
		if(isplane) {
			color += ((struct plane*)sph)->reflectance * recursive_ray_trace(hit, reflect_vector,num+1, inobj);
		}
		else if(!isplane)
			color += ((Spheres *)sph)->reflectance * recursive_ray_trace(hit, reflect_vector, num+1, inobj);
	}
	
	if(refract_on) {
		vec3 outlightvector; 
		if(refraction(hit, -1*ray, sph, isplane, inobj, &outlightvector)) {
			if(!isplane) {

				Spheres * refractsph = (Spheres *)sph;
				color += refractsph->refr*recursive_ray_trace(hit, outlightvector, num+1, !inobj);
			}
		}
	}
	
	if(diffuse_reflection_on && num<2) {
		int i;
		for (i=0;i<DIFFUSE_REFLECTION;i++) {
			float xtheta = 2.0 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 1.0;
			float ytheta = 2.0 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 1.0;
			float ztheta = 2.0 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 1.0;
			vec3 dfray;
			dfray = rotateX(xtheta*M_PI,surf_normal);
			dfray = rotateY(ytheta*M_PI,dfray);
			dfray = rotateZ(ztheta*M_PI,dfray);
			color += (0.1/DIFFUSE_REFLECTION)*recursive_ray_trace(hit, dfray, num+1, inobj);
		}
	}

	return color;

}

void ray_trace() {
  printf("entering the ray tracer\n");
  int i, j;
  float x_grid_size = image_width / float(win_width);
  float y_grid_size = image_height / float(win_height);
  float x_start = -0.5 * image_width;
  float y_start = -0.5 * image_height;
  vec3 ret_color;
  vec3 cur_pixel_pos;
  vec3 ray;


  cur_pixel_pos.x = x_start + 0.5 * x_grid_size;
  cur_pixel_pos.y = y_start + 0.5 * y_grid_size;
  cur_pixel_pos.z = image_plane;


  for (i=0; i<win_height; i++) {
    for (j=0; j<win_width; j++) {

      ret_color = vec3(0,0,0);
      ray = cur_pixel_pos - eye_pos;
      ray = normalize(ray);

      ret_color += recursive_ray_trace(eye_pos, ray, 0, false);

      if(antialias_on) {
	      vec3 pixel;
	      pixel.z = cur_pixel_pos.z;

	      // pixel 1
	      pixel.x = cur_pixel_pos.x - 0.25*x_grid_size;
	      pixel.y = cur_pixel_pos.y + 0.25*y_grid_size;
	      ray = normalize(pixel - eye_pos);
	      ret_color += recursive_ray_trace(eye_pos, ray, 0, false);

	      // pixel 2
	      pixel.x = cur_pixel_pos.x - 0.25*x_grid_size;
	      pixel.y = cur_pixel_pos.y - 0.25*y_grid_size;
	      ray = normalize(pixel - eye_pos);
	      ret_color += recursive_ray_trace(eye_pos, ray, 0, false);

	      // pixel 3
	      pixel.x = cur_pixel_pos.x + 0.25*x_grid_size;
	      pixel.y = cur_pixel_pos.y + 0.25*y_grid_size;
	      ray = normalize(pixel - eye_pos);
	      ret_color += recursive_ray_trace(eye_pos, ray, 0, false);

	      // pixel 4
	      pixel.x = cur_pixel_pos.x + 0.25*x_grid_size;
	      pixel.y = cur_pixel_pos.y - 0.25*y_grid_size;
	      ray = normalize(pixel - eye_pos);
	      ret_color += recursive_ray_trace(eye_pos, ray, 0, false);

	      ret_color = 0.2*ret_color;
      }

      frame[i][j][0] = GLfloat(ret_color.x);
      frame[i][j][1] = GLfloat(ret_color.y);
      frame[i][j][2] = GLfloat(ret_color.z);

      cur_pixel_pos.x += x_grid_size;
    }

    cur_pixel_pos.y += y_grid_size;
    cur_pixel_pos.x = x_start;
  }
}
