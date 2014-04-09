#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "MathHelper.h"		// NEW!
#include "shapes.h"
#include "shader.h"



#define NUM_VERTICES num_vertices
#define NUM_INDICES num_indices	

// From http://www.opengl.org/registry/specs/EXT/pixel_buffer_object.txt 
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GLuint shaderProgramID;
GLuint squareVao;
GLuint platformVao;
GLuint vbo;
GLuint positionID;
GLuint indexBufferID; 

GLuint	perspectiveMatrixID, viewMatrixID, modelMatrixID;	// IDs of variables mP, mV and mM in the shader

GLfloat* rotXMatrix;	
GLfloat* rotYMatrix;	
GLfloat* rotZMatrix;	
GLfloat* transMatrix;	
GLfloat* scaleMatrix;	
GLfloat* tempMatrix1;	
GLfloat* tempMatrix2;	
GLfloat* M;				

GLfloat* V;				
GLfloat* P;				

float horizontal = 0.25f;
float vertical = -0.25f;
float thetaY = 0.00f;
float thetaX = 0.00f;
float scaleAmount = 1.0f;

void initMatrices() {

	scaleAmount = 1.0f;

	// Allocate memory for the matrices and initialize them to the Identity matrix
	rotXMatrix = new GLfloat[16];	MathHelper::makeIdentity(rotXMatrix);
	rotYMatrix = new GLfloat[16];	MathHelper::makeIdentity(rotYMatrix);
	rotZMatrix = new GLfloat[16];	MathHelper::makeIdentity(rotZMatrix);
	transMatrix = new GLfloat[16];	MathHelper::makeIdentity(transMatrix);
	scaleMatrix = new GLfloat[16];	MathHelper::makeIdentity(scaleMatrix);
	tempMatrix1 = new GLfloat[16];	MathHelper::makeIdentity(tempMatrix1);
	tempMatrix2 = new GLfloat[16];	MathHelper::makeIdentity(tempMatrix2);
	M = new GLfloat[16];			MathHelper::makeIdentity(M);
	V = new GLfloat[16];			MathHelper::makeIdentity(V);
	P = new GLfloat[16];			MathHelper::makeIdentity(P);

	MathHelper::makePerspectiveMatrix(P, 60.0f, 1.0f, 1.0f, 1000.0f);
}

void changeViewport(int w, int h){
	glViewport(0, 0, w, h);
}

// Here is the function that gets called each time the window needs to be redrawn.
// It is the "paint" method for our program, and is set up from the glutDisplayFunc in main
void render() {
	vertical -= 0.0025;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(shaderProgramID);

	// Fill the matrices with valid data
	MathHelper::makeScale(scaleMatrix, scaleAmount, scaleAmount, scaleAmount);	
	MathHelper::makeRotateY(rotYMatrix, thetaY);						
	MathHelper::makeRotateX(rotXMatrix, thetaX);						
	MathHelper::makeTranslate(transMatrix, horizontal, vertical, -2.0f);

	/*multiply the matricies*/
	MathHelper::matrixMult4x4(tempMatrix1, rotXMatrix, scaleMatrix);	
	MathHelper::matrixMult4x4(tempMatrix2, rotYMatrix, tempMatrix1);	
	MathHelper::matrixMult4x4(M, transMatrix, tempMatrix2);
		
	glUniformMatrix4fv(modelMatrixID, 1, GL_TRUE, M);
	glUniformMatrix4fv(viewMatrixID, 1, GL_TRUE, V);
	glUniformMatrix4fv(perspectiveMatrixID, 1, GL_TRUE, P);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
	glDrawElements (GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, NULL);
	glutSwapBuffers();
	glutPostRedisplay();
}


void keyboard( unsigned char key, int x, int y ){
    /*update the variables based on key press*/
    switch( key ) {
	case 'a':
		horizontal -= 0.1;
		break;
	case 'd':
		horizontal += 0.1;
		break;
	case 'w':
		vertical += 0.1;
		break;
	case 's':
		vertical -= 0.1;
		break;
	case 033:
	    exit( EXIT_SUCCESS );
	    break;
    }
}


int main (int argc, char** argv) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Index Buffers");
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glewInit();

	/*set up the matricies*/
	initMatrices(); 

	// Make a shader
	shaderProgramID = createShaders();

	glGenVertexArrays(1, &squareVao);
	glBindVertexArray(squareVao);

/*
---------------------------------------------------------------------------
verticies	
---------------------------------------------------------------------------
*/
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Create the buffer, but don't load anything yet
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)+ sizeof(vertices2), 0, GL_STATIC_DRAW);
	//array, starting location, size, data
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(vertices2), vertices2); 
/*
---------------------------------------------------------------------------
indices
---------------------------------------------------------------------------
*/	
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) + sizeof(indices2), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(vertices2), vertices2); 



	positionID = glGetAttribLocation(shaderProgramID, "s_vPosition");
	perspectiveMatrixID = glGetUniformLocation(shaderProgramID, "mP");
	viewMatrixID = glGetUniformLocation(shaderProgramID, "mV");
	modelMatrixID = glGetUniformLocation(shaderProgramID, "mM");

	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);	
	
	glutKeyboardFunc(keyboard);

	glUseProgram(shaderProgramID);
	glEnableVertexAttribArray(positionID);
	
	glutMainLoop();
	
	return 0;
}

