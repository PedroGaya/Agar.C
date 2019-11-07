/*
 * structs.h
 *
 *  Created on: 6 de nov de 2019
 *      Author: Pedro Gaya
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

typedef struct object {
	char id;
	float x, y;
	float dx, dy;
	float size;
	int render;
}OBJ;

#endif /* STRUCTS_H_ */
