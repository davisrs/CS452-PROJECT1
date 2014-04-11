const int num_indices = 36;
const int num_vertices = 5;

GLfloat vertices[] = {
	0.00000f, 0.00000f, 0.00000f, 
	0.00000f, 0.00000f, 1.00000f, 
	1.00000f, 0.00000f, 1.00000f, 
	1.00000f, 0.00000f, 0.00000f, 
	0.50000f, 1.00000f, 0.50000f, 
};

GLuint indices[] = {
	0, 2, 1,
	0, 2, 3,
	0, 4, 3,
	0, 4, 1,
	1, 4, 2,
	2, 4, 3
};

GLfloat colors[] = {
	1.0f, 1.0f, 1.0f, 1.0f, 
	1.0f, 0.0f, 0.0f, 1.0f, 
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
};