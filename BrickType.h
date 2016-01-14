#ifndef BRICKTYPE_H
#define BRICKTYPE_H

class BrickType {
public:
	int type;
	mapArray map;
	Vector2D center;

	BrickType(int type);
	~BrickType();

	Vector2D getVectorFromCenter(Vector2D pos) const;

	static Vector2D getSizeByBrickType(int type);
	static void getMapByBrickType(mapArray *map, int type);
};



#endif