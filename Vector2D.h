#ifndef vector2d_h
#define vector2d_h

#define V2D(a, b) Vector2D(a, b)

enum Direction {
	NORTH,
	EAST,
	SOUTH,
	WEST,
	SOUTHWEST,
	SOUTHEAST
};

class Vector2D {
public:
	double x;
	double y;

	Vector2D() : x(0), y(0) {}
	Vector2D(double a, double b) : x(a), y(b) {}
	Vector2D(Direction dir) : x(0), y(0) {
		switch (dir) {
		case NORTH:
			y = -1;
			break;
		case SOUTH:
			y = 1;
			break;
		case SOUTHEAST:
			y = 1;
		case EAST:
			x = 1;
			break;
		case SOUTHWEST:
			y = 1;
		case WEST:
			x = -1;
			break;
		}
	}

	Vector2D reversed() {
		return V2D(y, x);
	}

	Vector2D& operator+=(const Vector2D& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	friend Vector2D operator+(Vector2D lhs, const Vector2D &v) {
		lhs += v;
		return lhs;
	}

	Vector2D& operator-=(const Vector2D& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	friend Vector2D operator-(Vector2D lhs, const Vector2D &v) {
		lhs -= v;
		return lhs;
	}

	Vector2D& operator*=(const Vector2D& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	friend Vector2D operator*(Vector2D lhs, const Vector2D &v) {
		lhs *= v;
		return lhs;
	}

	Vector2D& operator/=(const Vector2D& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		return *this;
	}

	friend Vector2D operator/(Vector2D lhs, const Vector2D &v) {
		lhs /= v;
		return lhs;
	}

	Vector2D& operator*=(const double& rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}

	friend Vector2D operator*(Vector2D lhs, const double &c) {
		lhs *= c;
		return lhs;
	}


	Vector2D& operator/=(const double& rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}

	friend Vector2D operator/(Vector2D lhs, const double &c) {
		lhs /= c;
		return lhs;
	}

	friend bool operator==(const Vector2D &v1, const Vector2D &v2) {
		return (v1.x == v2.x && v1.y == v2.y);
	}

	friend bool operator!=(const Vector2D &v1, const Vector2D &v2) {
		return (v1.x != v2.x || v1.y != v2.y);
	}

	friend bool operator>(const Vector2D &v1, const Vector2D &v2) {
		return (v1.x > v2.x && v1.y > v2.y);
	}

	friend bool operator<(const Vector2D &v1, const Vector2D &v2) {
		return (v1.x < v2.x && v1.y < v2.y);
	}

	friend bool operator>=(const Vector2D &v1, const Vector2D &v2) {
		return (v1.x >= v2.x && v1.y >= v2.y);
	}

	friend bool operator<=(const Vector2D &v1, const Vector2D &v2) {
		return (v1.x <= v2.x && v1.y <= v2.y);
	}

};


#endif