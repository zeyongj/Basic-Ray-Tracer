typedef struct {
  float x;
  float y;
  float z;
} Point;  

typedef struct {
  float x;
  float y;
  float z;
} Vector;

typedef struct {
  float r;
  float g;
  float b;
} RGB_float;

float vec_len(Vector);
float vec_dot(Vector, Vector);
Vector vec_plus(Vector, Vector);
Vector vec_minus(Vector, Vector);
Vector get_vec(Point, Point);
Point get_point(Point, Vector);
void normalize(Vector *);
Vector vec_scale(Vector, float);
RGB_float clr_add(RGB_float, RGB_float);
RGB_float clr_scale(RGB_float, float);
