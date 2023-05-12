/*Author: Robert Lopez
 *
 *  * Instructor: Ben Dicken
 *   *
 *    * Course: CSC 352 - U of A 
 *     *
 *      * Description:
 *       *
 *        * 	This file implements the functions declared in 3d.h
 *         */

#include "3d.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

Scene3D* Scene3D_create() {
	Scene3D* new = malloc(sizeof(Scene3D));
	new->count = 0;
	new->root = NULL;
	return new;
}

void Scene3D_destroy(Scene3D* scene) {
	Triangle3DNode* curr;
	curr = scene->root;
	while (curr != NULL) {
		Triangle3DNode* next = curr->next;
		free(curr);
		curr = next;
	}

	free(scene);
}

void Scene3D_write_stl_text(Scene3D* scene, char* file_name) {
	FILE* fp;
	Triangle3DNode* node;
	Triangle3D* tri;
	Coordinate3D* coord;
	int i;

	fp = fopen(file_name, "w");

	fprintf(fp, "solid scene\n");

	node = scene->root;
	while (node != NULL) {
		tri = &(node->triangle);
		fprintf(fp, "  facet normal 0.0 0.0 0.0\n");
		fprintf(fp, "    outer loop\n");
		for (i = 0; i < 3; i++) {
			coord = &(tri->a);
			if (i == 1) {
				coord = &(tri->b);
			} else if (i == 2) {
				coord = &(tri->c);
			}
			fprintf(fp,"      vertex %0.5f %0.5f %0.5f\n", coord->x, coord->y, coord->z);
		}
		fprintf(fp, "    endloop\n");
		fprintf(fp, "  endfacet\n");
		node = node->next;
	}

	fprintf(fp, "endsolid scene\n");
	fclose(fp);
}

void Scene3D_add_triangle( Scene3D* scene, Triangle3D triangle) {

	Triangle3DNode* node = (Triangle3DNode*) malloc(sizeof(Triangle3DNode));
	node->triangle = triangle;
	node->next = scene->root;
	scene->root = node;
	scene->count++;
	}



void Scene3D_add_quadrilateral( Scene3D* scene, Coordinate3D a, Coordinate3D b, Coordinate3D c, Coordinate3D d) {
	Triangle3D triangle_1 = (Triangle3D) {a, b, c};
	Triangle3D triangle_2 = (Triangle3D) {b, c, d};
	Triangle3D triangle_3 = (Triangle3D) {a, c, d};
	Triangle3D triangle_4 = (Triangle3D) {a, b, d};
	Scene3D_add_triangle(scene, triangle_1);
	Scene3D_add_triangle(scene, triangle_2);
	Scene3D_add_triangle(scene, triangle_3);
	Scene3D_add_triangle(scene, triangle_4);
}

void Scene3D_add_pyramid(Scene3D* scene, Coordinate3D origin, double width, double height, char*orientation) {

	double x = origin.x;
	double y = origin.y;
	double z = origin.z;


	Coordinate3D a,b,c,d;
	if(strcmp(orientation, "up")==0 || strcmp(orientation, "down")==0){
		a = (Coordinate3D){x - width/2, y - width/2, z};
		b = (Coordinate3D){x + width/2, y - width/2, z};
		c = (Coordinate3D){x + width/2, y + width/2, z};
		d = (Coordinate3D){x - width/2, y + width/2, z};
	} else if(strcmp(orientation, "forward") == 0 || strcmp(orientation, "backward") == 0) {
		a = (Coordinate3D){x - width/2, y, z-width/2};
		b = (Coordinate3D){x + width/2, y, z-width/2};
		c = (Coordinate3D){x + width/2, y, z+width/2};
		d = (Coordinate3D){x - width/2, y, z+width/2};
	} else if(strcmp(orientation, "left") == 0 || strcmp(orientation, "right") == 0) {
		a = (Coordinate3D){x, y - width/2, z-width/2};
		b = (Coordinate3D){x, y - width/2, z+width/2};
		c = (Coordinate3D){x, y + width/2, z+width/2};
		d = (Coordinate3D){x, y + width/2, z-width/2};
	}


	Scene3D_add_quadrilateral(scene, a, b, c, d);

	Coordinate3D apex;

	if (strcmp(orientation, "up") == 0) {
		apex = (Coordinate3D){x, y, z+height};
	} else if (strcmp(orientation, "down") == 0) {
		apex = (Coordinate3D) {x, y, z-height};
	} else if (strcmp(orientation, "forward") == 0){
		apex = (Coordinate3D) {x, y+height, z};
	} else if (strcmp(orientation, "backward") == 0){
		apex = (Coordinate3D) {x, y-height, z};
	} else if (strcmp(orientation, "left") == 0){
		apex = (Coordinate3D) {x-height, y, z};
	} else if (strcmp(orientation, "right") == 0){
		apex = (Coordinate3D) {x+height, y, z};
	}


	Triangle3D one = (Triangle3D) {a, b, apex};
	Triangle3D two = (Triangle3D) {b, c, apex};
	Triangle3D three = (Triangle3D) {c, d, apex};
	Triangle3D four = (Triangle3D) {d, a, apex};
	Scene3D_add_triangle(scene, one);
	Scene3D_add_triangle(scene, two);
	Scene3D_add_triangle(scene, three);
	Scene3D_add_triangle(scene, four);
}


void Scene3D_add_cuboid(Scene3D* scene, Coordinate3D origin, double width, double height, double depth) {

	double half_width = width / 2.0;
	double half_height = height / 2.0;
	double half_depth = depth / 2.0;


	Coordinate3D vertices[8];
	vertices[0] = (Coordinate3D) {origin.x - half_width, origin.y - half_height, origin.z + half_depth};
	vertices[1] = (Coordinate3D) {origin.x + half_width, origin.y - half_height, origin.z + half_depth};
	vertices[2] = (Coordinate3D) {origin.x + half_width, origin.y - half_height, origin.z - half_depth};
	vertices[3] = (Coordinate3D) {origin.x - half_width, origin.y - half_height, origin.z - half_depth};
	vertices[4] = (Coordinate3D) {origin.x - half_width, origin.y + half_height, origin.z + half_depth};
	vertices[5] = (Coordinate3D) {origin.x + half_width, origin.y + half_height, origin.z + half_depth};
	vertices[6] = (Coordinate3D) {origin.x + half_width, origin.y + half_height, origin.z - half_depth};
	vertices[7] = (Coordinate3D) {origin.x - half_width, origin.y + half_height, origin.z - half_depth};

	Scene3D_add_quadrilateral(scene, vertices[0], vertices[1], vertices[2], vertices[3]);
	Scene3D_add_quadrilateral(scene, vertices[1], vertices[5], vertices[6], vertices[2]);
	Scene3D_add_quadrilateral(scene, vertices[5], vertices[4], vertices[7], vertices[6]);
	Scene3D_add_quadrilateral(scene, vertices[4], vertices[0], vertices[3], vertices[7]);
	Scene3D_add_quadrilateral(scene, vertices[4], vertices[5], vertices[1], vertices[0]);
	Scene3D_add_quadrilateral(scene, vertices[3], vertices[2], vertices[6], vertices[7]);
}

void Scene3D_write_stl_binary(Scene3D* scene, char* file_name) {
	//printf("writing binary");

	Triangle3DNode* node;
	Triangle3D* tri;
	//Coordinate3D* coord;
	int i;

	// open file in write binary mode
	FILE* file = fopen(file_name, "wb");

	// write header
	//fprintf(file, "UINT8[80]\n");
	char header[80] = {0};	
	fwrite(&header, sizeof(char), 80, file);
	//printf("wrote header");

	// write the number of triangls as 32-bit unsigned ints
	//printf("%ld\n", scene->count);
	uint32_t count = scene->count;
	//printf("%d\n", count);
	fwrite(&count, sizeof(uint32_t), 1, file);
	//printf("UINT32\n");

	node = scene->root;
	int counter = 0;
	while (node != NULL) {
		//printf("writing");
		tri = &(node->triangle);
		// write each triangle
		//fprintf(file, "foreach triangle\n");
		//fprintf(file, "REAL32[3] - Normal vector\n");
		// write normal vector
		float v1 = 0.0f;
		float v2 = 0.0f;
		float v3 = 0.0f;
		fwrite(&v1, sizeof(float), 1, file);
		fwrite(&v2, sizeof(float), 1, file);
		fwrite(&v3, sizeof(float), 1, file);

		// write vertexes
		for (i = 0; i < 3; i++) {
			Coordinate3D* coord = NULL;
			coord = &(tri->a);
			if (i == 1) {
				coord = &(tri->b);
			} else if (i == 2) {
				coord = &(tri->c);
			}
			float x = (float)coord->x;
			float y = (float)coord->y;
			float z = (float)coord->z;
			fwrite(&x, sizeof(float), 1, file);
			fwrite(&y, sizeof(float), 1, file);
			fwrite(&z, sizeof(float), 1, file);
		}
		// write byte count which will always be 0
		uint16_t byte_count = 0;
		fwrite(&byte_count, sizeof(uint16_t), 1, file);
		node = node->next;
		++counter;
		//printf("done");
	}
	//printf("counter: %d\n", counter);	
	fclose(file);

}

/*
 * This function will follow this algorithm to create a 3d sphere:
 * 	- loop through vertical degrees with phi incremented by increment
 * 	- loop throug horizontal degrees with theta incremented by increment
 * 	- convert the four spherical coordinates to cartesian
 * 	- Round each cartesian to 4 decimals points then add 0 to avoid negative zeroes.
 * 	- offset each catesian coord by the respective origin 
 */
void Scene3D_add_sphere(Scene3D* scene, Coordinate3D origin, double radius, double increment) {
	double phi, theta, sin_phi, cos_phi, sin_theta, cos_theta;
	double r = radius;	
	Coordinate3D vertices[4];
	for (phi = increment ; phi <= 180; phi += increment) {
		for (theta = 0; theta < 360; theta += increment) {
		double phi_rad = phi * PI/180;
		double theta_rad = theta * PI/180;
		sin_phi = sin(phi_rad);
		cos_phi = cos(phi_rad);
		cos_theta = cos(theta_rad);
		sin_theta = sin(theta_rad);
		double increment_rad = increment * PI/180;
			vertices[0].x = round((r * sin_phi * cos_theta + origin.x) * 1000) / 1000 + 0;
			vertices[0].y = round((r * sin_phi * sin_theta + origin.y) * 1000) / 1000 + 0;
			vertices[0].z = round((r * cos_phi + origin.z) * 1000) / 1000 + 0;
			vertices[1].x = round((r * sin_phi * cos(theta_rad - increment_rad) + origin.x) * 1000) / 1000 + 0;
			vertices[1].y = round((r * sin_phi * sin(theta_rad - increment_rad) + origin.y) * 1000) / 1000 +0;
			vertices[1].z = round((r * cos_phi + origin.z) * 1000) / 1000+0;
			vertices[2].x = round((r * sin(phi_rad - increment_rad) * cos_theta + origin.x) * 1000) / 1000+0;
			vertices[2].y = round((r * sin(phi_rad - increment_rad) * sin_theta + origin.y) * 1000) / 1000+0;
			vertices[2].z = round((r * cos(phi_rad - increment_rad) + origin.z) * 1000) / 1000+0;
			vertices[3].x = round((r * sin(phi_rad - increment_rad) * cos(theta_rad - increment_rad) + origin.x) * 1000) / 1000+0;
			vertices[3].y = round((r * sin(phi_rad - increment_rad) * sin(theta_rad - increment_rad) + origin.y) * 1000) / 1000+0;
			vertices[3].z = round((r * cos(phi_rad - increment_rad) + origin.z) * 1000) / 1000+0;
			Scene3D_add_quadrilateral(scene, vertices[0], vertices[2], vertices[1], vertices[3]);
		}
	}
}
void Scene3D_add_fractal(Scene3D* scene, Coordinate3D origin, double size, int levels) {

	Scene3D_add_cuboid(scene, origin, size, size, size);
	// start by creating a cube with given paramters until depth of level 1 is reached
	if (levels == 1) {
		//Scene3D_add_cuboid(scene, origin, size, size, size);
		return;
	}
	// create half size for each 6 cubes
	double half_size = size / 2;

	// center of each side of the current cube
	Coordinate3D centers[6] = {
		{origin.x + half_size, origin.y, origin.z},  // right face
		{origin.x - half_size, origin.y, origin.z},  // left face
		{origin.x, origin.y + half_size, origin.z},  // top face
		{origin.x, origin.y - half_size, origin.z},  // bottom face
		{origin.x, origin.y, origin.z + half_size},
		{origin.x, origin.y, origin.z - half_size}
	};
	// create 6 new cubes with 1/4 the volume of the current level
	for (int i = 0; i < 6; i++) {
		Scene3D_add_fractal(scene, centers[i], half_size, levels - 1);
	}
}
