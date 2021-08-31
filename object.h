#include"include/vec.h"
const float precision = 0.00001;
class Object {
public:
	int index;

	char type; 

	vec3 mat_ambient;    
	vec3 mat_diffuse;
	vec3 mat_specular;
	float mat_shineness;

	float reflectance;       

	Object * next;
	
	Object(int id, char t, vec3 amb, vec3 diff, vec3 spec, float shine, float reflec):
	index(id), type(t), mat_ambient(amb), mat_diffuse(diff), mat_specular(spec), mat_shineness(shine), reflectance(reflec), next(NULL){}
	
	virtual ~Object() {
		delete this;
	}
	virtual float intersect(vec3 origin, vec3 dir, vec3* hit) = 0;
	virtual vec3 get_surfnormal(vec3 point) = 0;
	Object* add_Object(Object *o);
	
};

class sphere: public Object {
public:
	vec3 center;
	float radius;
	
	sphere(int id, vec3 amb, vec3 diff, vec3 spec, float shine, float reflec, float r, vec3 c):
	Object(id, 's', amb, diff, spec, shine, reflec), radius(r), center(c) {};
	virtual ~sphere() {
		delete this;
	}
	
	float intersect(vec3 origin, vec3 dir, vec3* hit);
	vec3 get_surfnormal(vec3 point);
	
};

class plane: public Object {
public:
	vec3 leftbottom;
	vec3 righttop;
	
	plane(int id, vec3 lb, vec3 ut):
	Object(id, 'p', vec3(0,0,0), vec3(0,0,0), vec3(0,0,0), 10, 0.4), leftbottom(lb), righttop(ut) {};
	virtual ~plane() {
		delete this;
	}

	float intersect(vec3 origin, vec3 dir, vec3* hit);
	vec3 get_surfnormal(vec3 point);
	
	
};

Object * intersect_scene(vec3 origin, vec3 dir, Object* scene, vec3* hit) ;


