#include "main.h"


BrickType::BrickType(int type) : type(type) {
	map = mapArray();
	map.size = getSizeByBrickType(type);
	map.allocateMemory();
	map.fillMap(0);
	getMapByBrickType(&map, type);

	center = V2D(0, 0);

	for (int iy = 0; iy < map.size.y; iy++) {
		for (int ix = 0; ix < map.size.x; ix++) {
			if (map.map[iy][ix] == 2)
				center = V2D(ix, iy);
		}
	}
}

BrickType::~BrickType() {
	map.freeMemory();
}

Vector2D BrickType::getVectorFromCenter(Vector2D pos) const
{
	return pos - center;
}

Vector2D BrickType::getSizeByBrickType(int type) {
	switch (type) {
	case 1:
		return V2D(5, 5);
	case 2:
	case 4:
	case 5:
	case 6:
	case 7:
		return V2D(3, 3);
	case 3:
		return V2D(2, 2);
	default:
		return V2D(0, 0);
	}
}

void BrickType::getMapByBrickType(mapArray *map, int type) {
	switch (type) {
	case 1: {
		int temp[5][5] = {
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 0, 0 },
			{ 0, 0, 2, 0, 0 },
			{ 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 0 }
		};
		for (int i = 0; i < 5; i++)
			for (int ix = 0; ix < 5; ix++)
				map->map[i][ix] = temp[i][ix];
		break;
	}
	case 2: {
		int temp[3][3] = {
			{ 0, 0, 0 },
			{ 1, 2, 1 },
			{ 0, 1, 0 }
		};
		for (int i = 0; i < 3; i++)
			for (int ix = 0; ix < 3; ix++)
				map->map[i][ix] = temp[i][ix];
		break;
	}
	case 3: {
		int temp[2][2] = {
			{ 2, 1 },
			{ 1, 1 }
		};
		for (int i = 0; i < 2; i++)
			for (int ix = 0; ix < 2; ix++)
				map->map[i][ix] = temp[i][ix];
		break;
	}
	case 4: {
		int temp[3][3] = {
			{ 0, 1, 0 },
			{ 0, 2, 0 },
			{ 0, 1, 1 }
		};
		for (int i = 0; i < 3; i++)
			for (int ix = 0; ix < 3; ix++)
				map->map[i][ix] = temp[i][ix];
		break;
	}
	case 5: {
		int temp[3][3] = {
			{ 0, 1, 0 },
			{ 0, 2, 0 },
			{ 1, 1, 0 }
		};
		for (int i = 0; i < 3; i++)
			for (int ix = 0; ix < 3; ix++)
				map->map[i][ix] = temp[i][ix];
		break;
	}
	case 6: {
		int temp[3][3] = {
			{ 0, 0, 0 },
			{ 0, 2, 1 },
			{ 1, 1, 0 }
		};
		for (int i = 0; i < 3; i++)
			for (int ix = 0; ix < 3; ix++)
				map->map[i][ix] = temp[i][ix];
		break;
	}
	case 7: {
		int temp[3][3] = {
			{ 0, 0, 0 },
			{ 1, 2, 0 },
			{ 0, 1, 1 }
		};
		for (int i = 0; i < 3; i++)
			for (int ix = 0; ix < 3; ix++)
				map->map[i][ix] = temp[i][ix];
		break;
	}
	}
}