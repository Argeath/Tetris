#include "mapArray.h"

mapArray::mapArray() {
	size = V2D(0, 0);
	map = nullptr;
}

void mapArray::allocateMemory() {
	map = new int*[int(size.y)];
	for (int i = 0; i < size.y; i++)
		map[i] = new int[int(size.x)];
	/*map = (int**)malloc(sizeof(int*) * size.y);
	for (int i = 0; i < size.y; i++) {
		map[i] = (int*)malloc(sizeof(int) * size.x);
	}*/
}

void mapArray::fillMap(int element) {
	for (int iy = 0; iy < size.y; iy++)
		for (int ix = 0; ix < size.x; ix++)
			map[iy][ix] = element;
}

void mapArray::freeMemory() {
	for (int i = 0; i < (int)size.y; i++)
		delete[] map[i];
	delete[] map;
	/*if (map != nullptr) {
		for (int i = 0; i < size.y; i++)
			free(map[i]);
		free(map);
	}*/
}

void mapArray::clone(mapArray *target) const
{
	if (target->size == size) {
		for (int iy = 0; iy < size.y; iy++)
			for (int ix = 0; ix < size.x; ix++)
				target->map[iy][ix] = map[iy][ix];
	}
}
