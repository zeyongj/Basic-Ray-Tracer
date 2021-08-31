
#include "include/Angel.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trace.h"
#include "global.h"
#include "sphere.h"

#include "scene.h"


int win_width = WIN_WIDTH;
int win_height = WIN_HEIGHT;

GLfloat frame[WIN_HEIGHT][WIN_WIDTH][3];
// array for the final image 
// This gets displayed in glut window via texture mapping

float image_width = IMAGE_WIDTH;
float image_height = (float(WIN_HEIGHT) / float(WIN_WIDTH)) * IMAGE_WIDTH;

// some colors
vec3 background_clr; // background color
vec3 null_clr = vec3(0.0, 0.0, 0.0); 


vec3 eye_pos = vec3(0.0, 0.0, 0.0);  // eye position
float image_plane = -1.5;           

Spheres *scene = NULL;


// light  position and color
vec3 light1;
vec3 light1_intensity;
float light1_shadow;

// global ambient term
vec3 global_ambient;

// light decay parameters
float decay_a;
float decay_b;
float decay_c;

// a flag to indicate whether you want to have shadows
int shadow_on = 0;
int reflect_on = 0;
int board_on = 1;
int refract_on = 0;
int diffuse_reflection_on = 0;
int antialias_on = 1;

// OpenGL
const int NumPoints = 6;

//plane
struct plane pl;

//----------------------------------------------------------------------------

void init()
{
	// Vertices of a square
	double ext = 1.0;
	vec4 points[NumPoints] = {
		vec4( -ext, -ext,  0, 1.0 ), 
		vec4(  ext, -ext,  0, 1.0 ), 
		vec4( -ext,  ext,  0, 1.0 ), 	
		vec4( -ext,  ext,  0, 1.0 ), 	
		vec4(  ext, -ext,  0, 1.0 ), 
		vec4(  ext,  ext,  0, 1.0 ) 
	};

	// Texture coordinates
	vec2 tex_coords[NumPoints] = {
		vec2( 0.0, 0.0 ),
		vec2( 1.0, 0.0 ),
		vec2( 0.0, 1.0 ),
		vec2( 0.0, 1.0 ),
		vec2( 1.0, 0.0 ),
		vec2( 1.0, 1.0 )
	};

	// Initialize texture objects
	GLuint texture;
	glGenTextures( 1, &texture );

	glBindTexture( GL_TEXTURE_2D, texture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, WIN_WIDTH, WIN_HEIGHT, 0,
		GL_RGB, GL_FLOAT, frame );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glActiveTexture( GL_TEXTURE0 );

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers( 1, &buffer );
	glBindBuffer( GL_ARRAY_BUFFER, buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(tex_coords), NULL, GL_STATIC_DRAW );
	GLintptr offset = 0;
	glBufferSubData( GL_ARRAY_BUFFER, offset, sizeof(points), points );
	offset += sizeof(points);
	glBufferSubData( GL_ARRAY_BUFFER, offset, sizeof(tex_coords), tex_coords );

	// Load shaders and use the resulting shader program
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
	glUseProgram( program );

	// set up vertex arrays
	offset = 0;
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(offset) );

	offset += sizeof(points);
	GLuint vTexCoord = glGetAttribLocation( program, "vTexCoord" ); 
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(offset) );

	glUniform1i( glGetUniformLocation(program, "texture"), 0 );

	glClearColor( 1.0, 1.0, 1.0, 1.0 );
}



void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT );
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	glDrawArrays( GL_TRIANGLES, 0, NumPoints );
	glutPostRedisplay();
	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':case 'Q':
		free(scene);
		exit(0);
		break;
	default:
		break;
	}
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{

	if (argc < 1) {
		printf("Missing arguments ... use:\n");
		printf("./raycast  <options>\n");
		return -1;
	}

	set_up_default_scene();
	
	for(int i = 1; i < 2; i++)
	{
		if (strcmp(argv[i], "1") == 0){
				
			diffuse_reflection_on = 1;
		}
		else if (strcmp(argv[i], "2")==0) {
			reflect_on = 1;
			shadow_on = 1;
		}
		else if (strcmp(argv[i], "3")==0) {
			refract_on = 1;
		}
		else if (strcmp(argv[i], "4")==0) {
			diffuse_reflection_on = 1;
			reflect_on = 1;
			shadow_on = 1;
			refract_on = 1;
		}
	}
	if(board_on) {
		pl.leftbottom = vec3(-20, -3, -0);
		pl.righttop = vec3(20, -3,-10);
		pl.reflectance = 0.4;
		pl.mat_ambient = vec3(0, 0, 0);
		pl.mat_diffuse1 = vec3(4, 4, 4);
		pl.mat_diffuse2 = vec3(0.0, 0.0, 0.0);
		pl.mat_specular = vec3(1.0, 1.0, 1.0);
		pl.shineness = 10;
		
	}

	ray_trace();


	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowSize( WIN_WIDTH, WIN_HEIGHT );
	glutCreateWindow( "Assignment 3" );
	glewInit();
	init();

	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutMainLoop();
	return 0;
}
