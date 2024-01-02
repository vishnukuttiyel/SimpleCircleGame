// test_Vec2.cpp
#include "gtest/gtest.h"
#include "include/vec2.h"

TEST(Vec2, Equality) {
  Vec2 v1(1, 2);
  Vec2 v2(1, 2);
  ASSERT_TRUE(v1 == v2);
}

TEST(Vec2, InEquality) {
  Vec2 v1(1, 2);
  Vec2 v2(1, 3);
  ASSERT_TRUE(v1 != v2);
}

TEST(Vec2, Addition) {
  Vec2 v1(1, 2);
  Vec2 v2(1, 2);
  Vec2 v3(2, 4);
  ASSERT_EQ(v1 + v2, v3);
}

TEST(Vec2, Subtraction) {
  Vec2 v1(2, 5);
  Vec2 v2(1, 3);
  Vec2 v3(1, 2);
  ASSERT_EQ(v1 - v2, v3);
}

TEST(Vec2, Division) {
  Vec2 v1(8, 4);
  Vec2 v2(2, 2);
  Vec2 v3(4, 2);
  ASSERT_EQ(v1 / v2, v3);
}

TEST(Vec2, Multiply) {
  Vec2 v1(3, 3);
  Vec2 v2(2, 2);
  Vec2 v3(6, 6);
  ASSERT_EQ(v1 * v2, v3);
}

TEST(Vec2, AdditionAssign) {
  Vec2 v1(1, 2);
  Vec2 v2(1, 2);
  Vec2 v3(2, 4);
  v1 += v2;
  ASSERT_EQ(v1, v3);
}

TEST(Vec2, SubtractionAssign) {
  Vec2 v1(2, 5);
  Vec2 v2(1, 3);
  Vec2 v3(1, 2);
  v1 -= v2;
  ASSERT_EQ(v1, v3);
}

TEST(Vec2, DivisionAssign) {
  Vec2 v1(8, 4);
  Vec2 v2(2, 2);
  Vec2 v3(4, 2);
  v1 /= v2;
  ASSERT_EQ(v1, v3);
}

TEST(Vec2, MultiplyAssign) {
  Vec2 v1(3, 3);
  Vec2 v2(2, 2);
  Vec2 v3(6, 6);
  v1 *= v2;
  ASSERT_EQ(v1, v3);
}

TEST(Vec2, DistanceSquare) {
  Vec2 v1(3, 3);
  Vec2 v2(2, 2);
  float distanceSquare = 2;
  ASSERT_EQ(v1.distanceSquare(v2), 2);
}

// Add more tests for other functions
