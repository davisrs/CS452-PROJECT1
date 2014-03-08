run: projectWorking.cpp
	g++ projectWorking.cpp MathHelper.cpp -lglut -lGLU -lGL -lGLEW -g
	
clean: 
	rm -f *.out *~ run
	
