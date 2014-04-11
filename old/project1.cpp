#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include "MathHelper.h"		// NEW!
//#include "shapes.h"
#include "shader.h"
#include <stdlib.h>
#include <iostream> 


#define NUM_VERTICES num_vertices
#define NUM_INDICES num_indices	

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

GLuint shaderProgramID;
GLuint squareVao;
GLuint platformVao;
GLuint vbo;
GLuint positionID;
GLuint colorID;
GLuint indexBufferID; 		

float horizontal = 0.0f;
float vertical = -0.0f;
float thetaY = 0.00f;
float thetaX = 0.00f;
float scaleAmount = 1.0f;
float barVertical=0.0f;
const int numLevels = 20;
const int num_vertices = 4 + 8 * numLevels; //number of lines  each bar adds 8
const int num_indices = 6 + 12 * numLevels; //num elements   each bar adds 12
GLfloat vertices[num_vertices * 3]; //num_verticies * 3
GLfloat color[num_vertices * 4]; //num_verticies * 3
GLuint indices[num_indices];

void genVertextData(){

	vertices[0] = -0.035f; vertices[1] = -0.04f; vertices[2] = 0.00f;
	
	vertices[3] = 0.035f; vertices[4] = -0.04f;	vertices[5] = 0.00f;
	
	vertices[6] = 0.035f; vertices[7] = 0.04f; vertices[8] = 0.00f;
	
	vertices[9] = -0.035f; vertices[10] = 0.04f; vertices[11] = 0.00f;

	

	float barhlx;
	float bartop = 0.05f;
	float barhrx;
	float barbottom;
	float border = 1.0f;

	int n = 1;

	for(int i=0; i<ARRAY_COUNT(color); i++){
		if(i < 15){
			color[i] = 1.0f;
		}else{
			if(n==1)
				color[i] = 1.0f;
			if(n==2)
				color[i] = 0.0f;
			if(n==3)
				color[i] = 0.0f;
			if(n==4){
				color[i] = 0.0f;
				n=0;
			}
			n++;
		}
		
	}

	for(int i=12; i<252; i+=24){

		float barhlx = -1.0 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2.0)));
		float barhrx = barhlx + 0.1f; 
		float barbottom = bartop - 0.1f;

		vertices[i]= -border; vertices[i+1] = barbottom; vertices[i+2] = 0.0f;
		
		vertices[i+3] = barhlx; vertices[i+4] = barbottom; vertices[i+5] = 0.0f;
		
		vertices[i+6] = barhlx; vertices[i+7] = bartop; vertices[i+8] = 0.0f;
		
		vertices[i+9]= -border; vertices[i+10] = bartop; vertices[i+11] = 0.0f;
		
		vertices[i+12] = barhrx; vertices[i+13] = barbottom; vertices[i+14] = 0.0f;

		vertices[i+15]= border; vertices[i+16] = barbottom; vertices[i+17] = 0.0f;

		vertices[i+18]= border; vertices[i+19] = bartop; vertices[i+20] = 0.0f;

		vertices[i+21] = barhrx; vertices[i+22] = bartop; vertices[i+23] = 0.0f;
		
		bartop -= 0.3f;

	}

	
	n=0;
	for(int i=0; i<126; i+=6){
		
		indices[i] = n;
		indices[i+1] = n+1;
		indices[i+2] = n+2;
		indices[i+3] = n;
		indices[i+4] = n+2;
		indices[i+5] = n+3;
		n+=4;
	}
}



void AdjustVertexData(float fXOffset, float fYOffset, float barV)
{
    std::vector<float> fNewData(ARRAY_COUNT(vertices));
    memcpy(&fNewData[0], vertices, sizeof(vertices));
   
    for(int i = 0; i < ARRAY_COUNT(vertices); i += 3)
    {
    	if(i<12){
    		fNewData[i] += fXOffset;
        	fNewData[i + 1] += fYOffset;
    	}else{
    		fNewData[i + 1] += barV;
    	}
        
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &fNewData[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void changeViewport(int w, int h){
	glViewport(0, 0, w, h);
}



// Here is the function that gets called each time the window needs to be redrawn.
// It is the "paint" method for our program, and is set up from the glutDisplayFunc in main
void render() {
	barVertical += 0.001;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(shaderProgramID);

	AdjustVertexData(horizontal, vertical, barVertical);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
	glDrawElements (GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, NULL);
	glutSwapBuffers();
	glutPostRedisplay();
}


void keyboard( unsigned char key, int x, int y ){
    /*update the variables based on key press*/
    switch( key ) {
	case 'a':
		horizontal -= 0.05;
		break;
	case 'd':
		horizontal += 0.05;
		break;
	case 'w':
		vertical += 0.05;
		break;
	case 's':
		vertical -= 0.05;
		break;
	case 033:
	    exit( EXIT_SUCCESS );
	    break;
    }
}


int main (int argc, char** argv) {
	
	genVertextData();


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Index Buffers");
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glewInit();

	// Make a shader
	shaderProgramID = createShaders();

	glGenVertexArrays(1, &squareVao);
	glBindVertexArray(squareVao);

	glGenVertexArrays(1, &platformVao);
	glBindVertexArray(platformVao);

/*
---------------------------------------------------------------------------
verticies	
---------------------------------------------------------------------------
*/
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Create the buffer, but don't load anything yet
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(color), NULL, GL_STATIC_DRAW);
	// Load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
	// Load the colors right after that
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices),sizeof(color), color);
/*
---------------------------------------------------------------------------
indices
---------------------------------------------------------------------------
*/	
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	positionID = glGetAttribLocation(shaderProgramID, "s_vPosition");
	colorID = glGetAttribLocation(shaderProgramID, "s_vColor");


	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));
	
	
	glutKeyboardFunc(keyboard);

	glUseProgram(shaderProgramID);
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	glutMainLoop();
	
	return 0;
}

