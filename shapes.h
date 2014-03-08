const int num_indices = 6;
const int num_vertices = 4;
const int num_indicesPlatform = 16;
const int num_verticesPlatform = 8;
int numPlatforms = 1;
float leftHole;
float rightHole;
float topBar;
float bottomBar;
/*---------------------------------------------------------------------
square
-----------------------------------------------------------------------
*/
GLfloat squareVertices[] = {
	-0.1f, -0.1f, 0.00f, //0
	-0.1f, 0.1f, 0.00f, //1
	0.1f, -0.1f, 0.00f, //2
	0.1f, 0.1f, 0.00f, //3
};

GLuint squareIndices[] = {
	0,	2,	3, 
	0,	3, 1,
};

GLfloat squareColors[] = {
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f
};


/*---------------------------------------------------------------------
platform 1
-----------------------------------------------------------------------
*/
GLfloat platformVertices[] = {
	-3.0f,-1.05,0,			//0
	-3.0, -1.0f, 0,			//1
	-0.15f, -1.05, 0,	//2
	-0.15f, -1.0f, 0,		//3
	0.15f, -1.05, 0,	//4
	0.15f, -1.0f, 0,		//5
	3.0f, -1.05, 0,			//6
	3.0f, -1.0f, 0            	//7
};

GLuint platformIndices[] = {
	0,	2,	3,
	0,	3, 1,
	4, 6, 7, 
	4, 7, 5
};

GLfloat platformColors[] = {
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f
};

