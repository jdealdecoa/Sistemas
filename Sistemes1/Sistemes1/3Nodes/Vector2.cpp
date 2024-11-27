#include "Vector2.h"

Vector2::Vector2()
{
}

Vector2::Vector2(int x, int y) : X(x), Y(y) {
}

Vector2::Vector2(Vector2* other) : X(other->X), Y(other->Y) {
}

Vector2 Vector2::operator+=(Vector2 other) {
	return *this + other;
}

Vector2 Vector2::operator-=(Vector2 other)
{
	return *this - other;
}

Vector2 Vector2::operator+(Vector2 other){
	Vector2 result = *this;
	result.X += other.X;
	result.Y += other.Y;
	return Vector2();
}

Vector2 Vector2::operator-(Vector2 other){
	Vector2 result = *this;
	result.X -= other.X;
	result.Y -= other.Y;
	return Vector2();
}

bool Vector2::operator==(const Vector2& other) const
{
	return (X == other.X && Y == other.Y);
	//if (this->X == other.X && this->Y == other.Y)
	//	return true;
	//else
	//	return false;
}
