#ifndef maparray_h
#define maparray_h

#include "Vector2D.h"

class mapArray {
public:
	int** map;
	Vector2D size;

	mapArray();

	void allocateMemory();
	void fillMap(int element);
	void freeMemory();

	void clone(mapArray* target) const;
};

#endif