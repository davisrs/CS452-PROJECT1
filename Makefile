run: project1.cpp
	g++ project1.cpp MathHelper.cpp -lglut -lGLU -lGL -lGLEW -g
	
clean: 
	rm -f *.out *~ run
