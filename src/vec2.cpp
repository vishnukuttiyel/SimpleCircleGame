// Vec2.cpp
#include "include/vec2.h"

#include <math.h>

Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(float xin, float yin) : x(xin), y(yin) {}

bool Vec2::operator==(const Vec2& rhs) const { return x == rhs.x && y == rhs.y; }

bool Vec2::operator!=(const Vec2& rhs) const { return !(*this == rhs); }

Vec2 Vec2::operator+(const Vec2& rhs) const { return Vec2(x + rhs.x, y + rhs.y); }

Vec2 Vec2::operator-(const Vec2& rhs) const { return Vec2(x - rhs.x, y - rhs.y); }

Vec2 Vec2::operator/(const Vec2& rhs) const { return Vec2(x / rhs.x, y / rhs.y); }

Vec2 Vec2::operator*(const Vec2& rhs) const { return Vec2(x * rhs.x, y * rhs.y); }

void Vec2::operator+=(const Vec2& rhs) {
    x += rhs.x;
    y += rhs.y;
}

void Vec2::operator-=(const Vec2& rhs) {
    x -= rhs.x;
    y -= rhs.y;
}

void Vec2::operator*=(const Vec2& rhs) {
    x *= rhs.x;
    y *= rhs.y;
}

void Vec2::operator/=(const Vec2& rhs) {
    x /= rhs.x;
    y /= rhs.y;
}

float Vec2::distanceSquare(const Vec2& rhs) const {
    float dx = x - rhs.x;
    float dy = y - rhs.y;
    return dx * dx + dy * dy;
}

float Vec2::length() const { return std::sqrt(x * x + y * y); }
