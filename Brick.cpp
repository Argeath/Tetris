#include "main.h"

Brick::Brick(int type) : timeSinceLastPosition(0), rotation(0), toRotate(false) {
	brickType = Window::getInstance()->brickTypes[type];
	lastPosition = V2D(rand() % 6 + 2, 0);
	nextPosition = lastPosition + V2D(0, 1);
	map = mapArray();
	map.size = brickType->map.size;
	map.allocateMemory();
	brickType->map.clone(&map);
}

Brick::~Brick() {
	map.freeMemory();
}

void Brick::update(double delta) {
	timeSinceLastPosition += delta;
	int speed = LOWEST_SPEED - (Window::getInstance()->game->stage * SPEED_PER_STAGE);

	if (speed < SPEED_LIMIT || Window::getInstance()->game->tempSpeedUp)
		speed = SPEED_LIMIT;

	double percent = (timeSinceLastPosition * 100) / speed;
	Vector2D vec = nextPosition - lastPosition;
	vec = vec * percent;
	position = lastPosition + vec;

	#ifdef DEBUG_MODE
	isInCollision(WEST);
	isInCollision(EAST);
	#endif

	if (percent >= 1)
		onReachedPosition();
}

void Brick::onReachedPosition() {
	timeSinceLastPosition = 0;
	lastPosition = nextPosition;
	nextPosition += V2D(0, 1);

	if (toRotate) {
		rotate();
	}

	EVENT_KEY key = Window::getInstance()->game->lastPressedKey;

	if (key == LEFT || key == RIGHT) {
		if (!isInCollision((key == LEFT) ? SOUTHWEST : SOUTHEAST)) {
			nextPosition += V2D((key == LEFT) ? -1 : 1, 0);
		}
		Window::getInstance()->game->lastPressedKey = NONE;
	}

	if ((nextPosition.x < lastPosition.x || toRotate) && isInCollision(WEST)) {
		lastPosition.x = 0;

		while (isInCollision(WEST))
			lastPosition.x++;

		nextPosition.x = position.x = --lastPosition.x;
	}

	if ((nextPosition.x > lastPosition.x || toRotate) && isInCollision(EAST)) {
		lastPosition.x = 10;

		while (isInCollision(EAST))
			lastPosition.x--;

		nextPosition.x = position.x = ++lastPosition.x;
	}
	if(toRotate)
		while (isInCollision(SOUTH)) {
			lastPosition -= V2D(0, 1);
			nextPosition -= V2D(0, 1);
		}

	toRotate = false;
}

bool Brick::isInCollision(Direction dir) const {
	for (int iy = 0; iy < map.size.y; iy++) {
		for (int ix = 0; ix < map.size.x; ix++) {
			if (map.map[iy][ix] > 0) {
				Vector2D pos = lastPosition + brickType->getVectorFromCenter(V2D(ix, iy)) + Vector2D(dir);

				if (pos.x < 0 || pos.x > 9 || ((dir == SOUTH || dir == SOUTHWEST || dir == SOUTHEAST) && pos.y > 19) || (pos.y > 0 && Window::getInstance()->game->map.map[int(pos.y)][int(pos.x)] != 0)) {
					#ifdef DEBUG_MODE
					Vector2D debugPos = position + brickType->getVectorFromCenter(V2D(ix, iy));
					Window::getInstance()->debug->collision->position = debugPos * BRICK_SIZE + V2D(33, 33 + PADDING_TOP);
					if (dir == WEST)
						Window::getInstance()->debug->collision->text = "\032";
					else if (dir == EAST)
						Window::getInstance()->debug->collision->text = "\033";
					else if(dir == SOUTH)
						Window::getInstance()->debug->collision->text = "\031";
					#endif

					return true;
				}
			}
		}
	}
	return false;
}

namespace Mathf {
	double round(double fValue)
	{
		return fValue < 0 ? ceil(fValue - 0.5)
			: floor(fValue + 0.5);
	}
}

void Brick::rotate() {
	if (brickType->type == 3)
		return;

	rotation += 90;
	if (rotation >= 360)
		rotation = 0;

	double rad = double(rotation) / 180 * M_PI;
	Vector2D center = brickType->center;

	map.fillMap(0);
	map.map[int(center.y)][int(center.x)] = 2;

	for (int iy = 0; iy < map.size.y; iy++) {
		for (int ix = 0; ix < map.size.x; ix++) {
			if (brickType->map.map[iy][ix] == 1) {
				Vector2D newPos;
				newPos.x = Mathf::round((ix - center.x) * cos(rad) - (iy - center.y) * sin(rad) + center.x);
				newPos.y = Mathf::round((ix - center.x) * sin(rad) + (iy - center.y) * cos(rad) + center.y);
				map.map[int(newPos.y)][int(newPos.x)] = 1;
			}
		}
	}
}