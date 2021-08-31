#include "object.h"

Object* Object:: add_Object(Object* obj) {
	if(obj==NULL) {
		return this;
	}
	else {
		this->next = obj;
		return this;
	}
}


float sphere:: intersect(vec3 origin, vec3 dir, vec3* hit) {
	vec3 toCenter = origin - center;

	float a = dot(dir,dir);

	float b = 2.0*dot(dir, toCenter);

	float c = dot(toCenter, toCenter) - radius * radius;
	
	float delta = b*b - 4*a*c;
	if(delta<-precision) {

		return -1.0;	
	}
	 
	float t1 = (-b + sqrt(delta)) / (2*a);
	float t2 = (-b - sqrt(delta)) / (2*a);

	*hit = origin + t2*dir;
	return t2;

}

vec3 sphere:: get_surfnormal(vec3 hitpoint) {
	vec3 surfnormal = hitpoint - center;
	surfnormal = normalize(surfnormal);
	return surfnormal;
}

float plane:: intersect(vec3 origin, vec3 dir, vec3* hit) {
	if(dir.y==0) return -1.0;
	
	float t = (leftbottom.y-origin.y)/dir.y;
	*hit = origin + t*dir;
	if(hit->x>righttop.x || hit->x<leftbottom.x || hit->z<righttop.z || hit->z>leftbottom.z) return -1.0;
	
	return t;

}

vec3 plane:: get_surfnormal(vec3 hitpoint) {
	return vec3(0,1,0);
	
}

Object * intersect_scene(vec3 origin, vec3 dir, Object* scene, vec3* hit) {
	vec3 nhit;
	Object * rv = NULL;
	float best_t = 9999999;
	
	Object * s_ptr = scene;
	while(s_ptr) {
		float local_t = s_ptr->intersect(origin, dir, &nhit);

		if(local_t>0) {
			if(local_t < best_t) {
				best_t = local_t;
				*hit = *nhit;
				rv = s_ptr;
			}
		}
		s_ptr = s_ptr->next;
	}
	return rv;
}

