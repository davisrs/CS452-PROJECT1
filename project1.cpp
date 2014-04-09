#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include "MathHelper.h"		// NEW!
#include "shapes.h"
#include "shader.h"



#define NUM_VERTICES num_vertices
#define NUM_INDICES num_indices	

// From http://www.opengl.org/registry/specs/EXT/pixel_buffer_object.txt 
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

//MACRO For Collision detection fix
#define epsilon 1
//#define epsilon 0.0001

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

const int font=(int)GLUT_BITMAP_9_BY_15;//GLUT FONT

float horizontal = 0.0f;
float vertical = -0.0f;
float thetaY = 0.00f;
float thetaX = 0.00f;
float scaleAmount = 1.0f;

void AdjustVertexData(float fXOffset, float fYOffset)
{
    std::vector<float> fNewData(ARRAY_COUNT(vertices));
    memcpy(&fNewData[0], vertices, sizeof(vertices));
   
    for(int i = 0; i < num_vertices*3; i += 3)
    {
    	if(i<12){
    		fNewData[i] += fXOffset;
        	fNewData[i + 1] += fYOffset;
    	}
        
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &fNewData[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(shaderProgramID);

	AdjustVertexData(horizontal, vertical);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
	glDrawElements (GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, NULL);
	glutSwapBuffers();
	glutPostRedisplay();
}

void renderBitmapString(float x, float y, void *font,const char *string){
    const char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
} 

void keyboard( unsigned char key, int x, int y ){
    /*update the variables based on key press*/
	int box_0x=0;//sw corner
	int box_0y=1;
	int box_1x=3;//se corner
	int plat_1x=15;//left se
	int plat_1y=16;
	int plat_2x=18;//left ne
	int plat_2y=19;
	int plat_4x=24;//right sw
	//int temp_h= horizontal;
	//int temp_v= vertical;
	float box_x=vertices[box_0x]+horizontal;//lower left coord
	float box_y=vertices[box_0y]+vertical;//lower left coord
	
	float box_x_1=vertices[box_1x]+horizontal;//lower right coord
	float box_x_0=vertices[box_0x]+horizontal;//lower left coord (model coord+ofset)
	
    switch( key ) {//ERROR : HOLDING A KEY BREAKS COLLIOSION DETECTION
    	case 'q'://debugging
    		printf("\nDebug\n");
    		printf("horizontal %f\n", horizontal);
    		printf("vertical %f\n", vertical);
    		printf("d\n");
    		printf("box_1x %f\n", vertices[box_1x]+horizontal);
		printf("plat_4x %f\n",vertices[plat_4x]);
		printf("a\n");
		printf("plat_1x %f\n",vertices[plat_1x]);
		printf("box_0x %f\n", vertices[box_0x]+horizontal);
		break;
	case 'e'://debugging
    		printf("\nDebug\n");
    		printf("horizontal %f\n", horizontal);
    		printf("vertical %f\n", vertical);
    		printf("a\n");//left
    		//printf("box_x %f\n", box_x);
    		printf("box_y %f\n", box_y);
    		//printf("plat_1x %f\n",vertices[plat_1x]);
		//printf("plat_1y %f\n",vertices[plat_1y]);
		//printf("plat_2x %f\n",vertices[plat_2x]);
		//printf("plat_2y %f\n",vertices[plat_2y]);
		printf("box_y < plat_2y = %s\n",(box_y < vertices[plat_2y]) ? "true" : "false");
		printf("d\n");//right --WORKS!
		printf("box_x_1 %f\n", box_x_1);
		printf("(vertices[box_1x]+horizontal) %f\n", (vertices[box_1x]+horizontal));
		printf("vertices[plat_4x] %f\n",vertices[plat_4x]);
		printf("(vertices[box_1x]+horizontal) == vertices[plat_4x]) = %s\n",( (vertices[box_1x]+horizontal) == vertices[plat_4x]) ? "true" : "false");
		printf("(box_x_1 == vertices[plat_4x]) = %s\n", ( box_x_1 == vertices[plat_4x] ) ? "true" : "false");
		printf("(box_x_1 - vertices[plat_4x]) = %f\n", ( box_x_1 - vertices[plat_4x] ) );
		break;
	case 'a'://left -- WORKS!
		//if( (box_x == vertices[plat_1x]) &&
		if( ( ( -epsilon <= (box_x_0 - vertices[plat_1x])) && ( (box_x_0 - vertices[plat_1x]) <= epsilon ) ) && ( box_y >= vertices[plat_1y] && box_y < vertices[plat_2y] ) ){//be able to still move if above platform
			printf("\nCan't move\n");
			printf("box_0x %f\n", vertices[box_0x]+horizontal);
			printf("plat_1x %f\n",vertices[plat_1x]);
		}
		else{
			horizontal -= 0.1;
			printf("\nbox_0x %f\n", vertices[box_0x]+horizontal);
			printf("plat_1x %f\n",vertices[plat_1x]);
			printf("horizontal %f\n", horizontal);
		}
		break;
		//glColor3f(1.0, 0.2, 0.2); //a great red
		//glRasterPos2i(-1.5, 3);
		//glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"text to render");
		//renderBitmapString(300,240,(void *)font,"Esc - Quit");
		//perror ("hjkhfs %d", horizontal);		
	case 'd'://right box1, plat4
		if( ( ( -epsilon <= (box_x_1 - vertices[plat_4x])) && ( (box_x_1 - vertices[plat_4x]) <= epsilon ) ) && ( box_y >= vertices[plat_1y] && box_y < vertices[plat_2y] ) ){//IT NOW WORKS!!
			printf("\nCan't move\n");
			printf("box_1x %f\n", vertices[box_1x]+horizontal);
			printf("plat_4x %f\n",vertices[plat_4x]);
		}
		else{
			horizontal += 0.1;	
			printf("\nbox_1x %f\n", vertices[box_1x]+horizontal);
			printf("box_1x %f\n", vertices[box_1x]+horizontal);
			printf("plat_4x %f\n",vertices[plat_4x]);
		}
		break;
	case 'w'://up
		vertical += 0.1;
		break;
	case 's'://down
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW);
	// Load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
	// Load the colors right after that
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices),sizeof(colors), colors);
/*
---------------------------------------------------------------------------
indices
---------------------------------------------------------------------------
*/	
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

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

