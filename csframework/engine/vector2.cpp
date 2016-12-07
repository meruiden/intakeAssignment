#include "vector2.h"

Vector2::Vector2(float xx, float yy)
{
    this->x = xx;
    this->y = yy;
}

Vector2::Vector2(Vector2 vec1, Vector2 vec2)
{
    this->x = vec1.x - vec2.x;
    this->y = vec1.y - vec2.y;
}

Vector2::~Vector2()
{

}

void Vector2::normalize()
{
    float m = magnitude();
    if(m != 0){
        *this /= m;
    }
}

// handle += Vector2 or float
Vector2& Vector2::operator+=(const Vector2& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}


Vector2& Vector2::operator+=(const float rhs)
{
    x += rhs;
    y += rhs;
    return *this;
}

// handle -= Vector2 or float
Vector2& Vector2::operator-=(const Vector2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}


Vector2& Vector2::operator-=(const float rhs)
{
    x -= rhs;
    y -= rhs;
    return *this;
}

// handle /= Vector2 or float
Vector2& Vector2::operator/=(const Vector2& rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    return *this;
}


Vector2& Vector2::operator/=(const float rhs)
{
    x /= rhs;
    y /= rhs;
    return *this;
}

// handle *= Vector2 or float
Vector2& Vector2::operator*=(const Vector2& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    return *this;
}


Vector2& Vector2::operator*=(const float rhs)
{
    x *= rhs;
    y *= rhs;
    return *this;
}

// handle + Vector2 or float
Vector2 Vector2::operator+(const Vector2& rhs)
{
    return Vector2(x + rhs.x, y + rhs.y);
}

Vector2 Vector2::operator+(const float rhs)
{
   return Vector2(x + rhs, y + rhs);
}

// handle - Vector2 or float
Vector2 Vector2::operator-(const Vector2& rhs)
{
    return Vector2(x - rhs.x, y - rhs.y);
}

Vector2 Vector2::operator-(const float rhs)
{
    return Vector2(x - rhs, y - rhs);
}

// handle * Vector2 or float
Vector2 Vector2::operator*(const Vector2& rhs)
{
    return Vector2(x * rhs.x, y * rhs.y);
}

Vector2 Vector2::operator*(const float rhs)
{
    return Vector2(x * rhs, y * rhs);
}

// handle / Vector2 or float
Vector2 Vector2::operator/(const Vector2& rhs)
{
    return Vector2(x / rhs.x, y / rhs.y);
}

Vector2 Vector2::operator/(const float rhs)
{
    return Vector2(x / rhs, y / rhs);
}

bool Vector2::operator==(const Vector2& rhs)
{
    return(x==rhs.x && y==rhs.y);
}

bool Vector2::operator!=(const Vector2& rhs)
{
    return(x!=rhs.x || y!=rhs.y);
}

void Vector2::limit(float value)
{
    if(magnitude() > value){
        normalize();
        Vector2(x,y) *= value;
    }
}


Vector2 Vector2::getNormalPoint(Vector2 p, Vector2 a, Vector2 b)
{
    Vector2 ap = p - a;
    Vector2 ab = b - a;
    ab.normalize();

    ab *= ap.dot(ab);

    Vector2 normalPoint = a + ab;
    return normalPoint;
}

float Vector2::dot(Vector2 b)
{
    return ((x * b.x) + (y * b.y));
}


std::string Vector2::toString()
{
	std::stringstream ss;
	ss << "(";
	ss << x;
	ss << ", ";
	ss << y;
	ss << ")";

	return ss.str();
}
