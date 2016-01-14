#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG_MODE
class Debug_Collision {
public:
	Vector2D position;
	char* text;

	Debug_Collision() : text(""), position(V2D(0, 0)) {}
};

class Debug {
public:
	Debug_Collision *collision;

	void printCollision();

	Debug() {
		collision = new Debug_Collision();
	}
};
#endif

#endif