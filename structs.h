/*
 * structs.h
 *
 *  Created on: 6 de nov de 2019
 *      Author: Pedro Gaya
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

typedef struct object {
	char id; // Identifier for object type
	float x, y; // These represent world coordinates
	float dx, dy; // Velocity
	float size;
	int render; // Flag for render
	int score;
}OBJ;

#endif /* STRUCTS_H_ */
