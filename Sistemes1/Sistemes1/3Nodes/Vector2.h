#pragma once
class Vector2
{
public:
	int X = 0;
	int Y = 0;

	Vector2();
	Vector2(int x, int y);
	Vector2(Vector2* other);

	Vector2 operator+=(Vector2 other);
	Vector2 operator-=(Vector2 other);
	Vector2 operator+(Vector2 other);
	Vector2 operator-(Vector2 other);
	bool operator==(const Vector2& other) const;
	bool operator!=(const Vector2& other) const;
};

