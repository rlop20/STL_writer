/*
 * Author: Robert Lopez
 * 
 * Description:
 * 	This program uses 3d.h to create a scene that contains a sphere, cuboid, pyramid,
 * 	and a fractal.
 */
#include "3d.h"
#include <stdio.h>
#include <stdlib.h>

int main() 
{
	Scene3D* s = Scene3D_create();
	// sphere
	Coordinate3D origin = (Coordinate3D){100, 100, 0};
	Scene3D_add_sphere(s, origin, 40, 5);
	// pyramid
	char* dir = "backward";
	Scene3D_add_pyramid(s, origin, 55, 100, dir);
	// cuboid
	Coordinate3D floor = (Coordinate3D){100, 0, 0};
	Scene3D_add_cuboid(s, floor, 300, 7, 100);
	// fractal
	Scene3D_add_fractal(s, origin, 50, 2);
	Scene3D_write_stl_text(s, "output.stl");
	Scene3D_destroy(s);
	return 0;
}
