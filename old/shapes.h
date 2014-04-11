const int num_vertices = 20; //number of lines  each bar adds 8
const int num_indices = 30; //num elements   each bar adds 12

float border = 1.0f;

float bar1hlx = -0.05; float bar1top = 0.05f; float bar1hrx = bar1hlx + 0.1f; float bar1bottom = bar1top - 0.1f;
float bar2hlx = 0.5; float bar2top = 0.5f;  float bar2hrx = bar2hlx + 0.1f;  float bar2bottom = bar2top - 0.1f;



/*GLfloat vertices[] = {
	-0.05f, -0.05f, 0.00f, //0
	0.05f, -0.05f, 0.00f, //1
	0.05f, 0.05f, 0.00f, //2
	-0.05f, 0.05f, 0.00f, //3

	//platform 1
	-border, bar1bottom, 0.00f, //4
	bar1hlx, bar1bottom, 0.00f, //5 
	bar1hlx, bar1top, 0.00f, //6
	-border, bar1top, 0.00f, //7
	bar1hrx, bar1bottom, 0.00f, //8
	border, bar1bottom, 0.00f, //9
	border, bar1top, 0.00f, //10
	bar1hrx, bar1top, 0.00f, //11


	//platform 2
	-border, bar2bottom, 0.00f, //4
	bar2hlx, bar2bottom, 0.00f, //5 
	bar2hlx, bar2top, 0.00f, //6
	-border, bar2top, 0.00f, //7
	bar2hrx, bar2bottom, 0.00f, //8
	border, bar2bottom, 0.00f, //9
	border, bar2top, 0.00f, //10
	bar2hrx, bar2top, 0.00f, //11

};

*/

GLuint indices[] = {
	0,	1,	2, 
	0,	2, 3,

	//platform 1
	4, 5, 6,
	4, 6, 7,

	8, 9, 10,
	8, 10, 11,

	//platform 2
	12, 13, 14,
	12, 14, 15,
	
	16, 17, 18,
	16, 18, 19

};
