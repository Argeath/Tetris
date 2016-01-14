#ifndef BRICK_H
#define BRICK_H

class Brick {
public:
	BrickType *brickType;
	mapArray map;
	Vector2D lastPosition;
	Vector2D nextPosition;
	Vector2D position;
	double timeSinceLastPosition;
	int rotation; // kat w stopniach
	bool toRotate;

	Brick(int type);
	~Brick();

	void update(double delta);

	void rotate();
	void onReachedPosition();
	bool isInCollision(Direction dir) const;
};


#endif