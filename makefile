generator: generator.c 3d.o 3d.h 
	gcc -Wall -Werror -std=c11 generator.c -g -o generator 3d.o -lm
3d.o: 3d.c 3d.h
		gcc -Wall -Werror -std=c11 -c 3d.c -o 3d.o
clean:
		rm -f generator 3d.o sphere.stl triangle.stl fractal.stl binary.stl
