#version 130

// This is the same old fragment shader we've been using

in vec4 color;
out vec4 fColor;

void main () {
	fColor.r = 1.0f;
	fColor.g = 1.0f;
	fColor.b = 1.0f;
	fColor.a = 1.0f;
	//fColor = vec4(0.5, 1.0, 1.0, 1.0);
	//fColor.r = color.r;
	//fColor.g = 1.0f;
	//fColor.b = color.b;
	//fColor.a = 1.0f;	
}
