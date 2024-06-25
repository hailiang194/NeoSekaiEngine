#include <gtest/gtest.h>
#include "SekaiEngine/Math/Collision.h"

TEST(EngineTest, TestCollisionRectAndPointAtTopLeftPosition)
{
    SekaiEngine::Math::Vector2D position(10.0f, 10.0f);
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);

    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndPoint(rect, position));
}

TEST(EngineTest, TestCollisionRectAndPointAtBottomRightPosition)
{
    SekaiEngine::Math::Vector2D position(30.0f, 25.0f);
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);

    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndPoint(rect, position));
}

TEST(EngineTest, TestCollisionRectAndPointAtBottomLeftPosition)
{
    SekaiEngine::Math::Vector2D position(10.0f, 25.0f);
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);

    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndPoint(rect, position));
}

TEST(EngineTest, TestCollisionRectAndPointAtTopRightPosition)
{
    SekaiEngine::Math::Vector2D position(30.0f, 10.0f);
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);

    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndPoint(rect, position));
}

TEST(EngineTest, TestCollisionRectAndPointNotCollsion)
{
    SekaiEngine::Math::Vector2D position(10.0f, -10.0f);
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);

    EXPECT_FALSE(SekaiEngine::Math::isCollisionBetweenRectAndPoint(rect, position));
}

TEST(EngineTest, TestCollisionRectAndPointAtTopLine)
{
    SekaiEngine::Math::Vector2D position(15.0f, 10.0f);
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);

    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndPoint(rect, position));
}

TEST(EngineTest, TestCollisionRectAndPointAtBottomLine)
{
    SekaiEngine::Math::Vector2D position(15.0f, 25.0f);
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);

    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndPoint(rect, position));
}

TEST(EngineTest, TestCollisionRectAndPointAtLeftLine)
{
    SekaiEngine::Math::Vector2D position(10.0f, 20.0f);
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);

    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndPoint(rect, position));
}

TEST(EngineTest, TestCollisionRectAndPointAtRightLine)
{
    SekaiEngine::Math::Vector2D position(30.0f, 20.0f);
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);

    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndPoint(rect, position));
}

TEST(EngineTest, TestCollisionCircleAndPointAtCenter)
{
    SekaiEngine::Math::Vector2D position(10.0f, 10.0f);
    SekaiEngine::Shape::Circle circle(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenCircleAndPoint(circle, position));
}

TEST(EngineTest, TestCollisionCircleAndPointInside)
{
    SekaiEngine::Math::Vector2D position(15.0f, 10.0f);
    SekaiEngine::Shape::Circle circle(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenCircleAndPoint(circle, position));
}

TEST(EngineTest, TestCollisionCircleAndPointAtBorder)
{
    SekaiEngine::Math::Vector2D position(30.0f, 10.0f);
    SekaiEngine::Shape::Circle circle(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenCircleAndPoint(circle, position));
}

TEST(EngineTest, TestCollisionCircleAndPointOutside)
{
    SekaiEngine::Math::Vector2D position(35.0f, 10.0f);
    SekaiEngine::Shape::Circle circle(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f);
    EXPECT_FALSE(SekaiEngine::Math::isCollisionBetweenCircleAndPoint(circle, position));
}

TEST(EngineTest, TestCollision2RectanglesOutsize)
{
    SekaiEngine::Shape::Rectangle rect1(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Rectangle rect2(SekaiEngine::Math::Vector2D(35.0f, 35.0f), 20.0f, 15.0f);
    EXPECT_FALSE(SekaiEngine::Math::isCollisionBetween2Rects(rect1, rect2));
}

TEST(EngineTest, TestCollision2RectanglesLeftCollision)
{
    SekaiEngine::Shape::Rectangle rect1(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Rectangle rect2(SekaiEngine::Math::Vector2D(-20.0f, -5.0f), 30.0f, 20.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetween2Rects(rect1, rect2));
}

TEST(EngineTest, TestCollision2RectanglesRightCollision)
{
    SekaiEngine::Shape::Rectangle rect1(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Rectangle rect2(SekaiEngine::Math::Vector2D(30.0f, 20.0f), 30.0f, 20.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetween2Rects(rect1, rect2));
}

TEST(EngineTest, TestCollision2RectanglesTopCollision)
{
    SekaiEngine::Shape::Rectangle rect1(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Rectangle rect2(SekaiEngine::Math::Vector2D(-20.0f, 20.0f), 30.0f, 20.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetween2Rects(rect1, rect2));
}

TEST(EngineTest, TestCollision2RectanglesBottomCollision)
{
    SekaiEngine::Shape::Rectangle rect1(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Rectangle rect2(SekaiEngine::Math::Vector2D(20.0f, 25.0f), 30.0f, 20.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetween2Rects(rect1, rect2));
}

TEST(EngineTest, TestCollision2RectanglesInside)
{
    SekaiEngine::Shape::Rectangle rect1(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Rectangle rect2(SekaiEngine::Math::Vector2D(15.0f, 15.0f), 10.0f, 8.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetween2Rects(rect1, rect2));
}

TEST(EngineTest, TestCollision2RectanglesInsideCollision)
{
    SekaiEngine::Shape::Rectangle rect1(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Rectangle rect2(SekaiEngine::Math::Vector2D(15.0f, 5.0f), 10.0f, 30.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetween2Rects(rect1, rect2));
}

TEST(EngineTest, TestCollisionRectAndCircleOutside)
{
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Circle circle(SekaiEngine::Math::Vector2D(0.0f, 0.0f), 5.0f);
    EXPECT_FALSE(SekaiEngine::Math::isCollisionBetweenRectAndCircle(rect, circle));
}

TEST(EngineTest, TestCollisionRectAndCircleTopBorder)
{
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Circle circle(SekaiEngine::Math::Vector2D(20.0f, 5.0f), 5.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndCircle(rect, circle));
}

TEST(EngineTest, TestCollisionRectAndCircleTopInside)
{
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Circle circle(SekaiEngine::Math::Vector2D(20.0f, 7.0f), 5.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndCircle(rect, circle));
}

TEST(EngineTest, TestCollisionRectAndCircleBottomBorder)
{
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Circle circle(SekaiEngine::Math::Vector2D(20.0f, 30.0f), 5.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndCircle(rect, circle));
}

TEST(EngineTest, TestCollisionRectAndCircleBottomInside)
{
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Circle circle(SekaiEngine::Math::Vector2D(20.0f, 28.0f), 5.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndCircle(rect, circle));
}

TEST(EngineTest, TestCollisionRectAndCircleLeftBorder)
{
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Circle circle(SekaiEngine::Math::Vector2D(5.0f, 20.0f), 5.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndCircle(rect, circle));
}

TEST(EngineTest, TestCollisionRectAndCircleLeftInside)
{
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Circle circle(SekaiEngine::Math::Vector2D(7.0f, 20.0f), 5.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndCircle(rect, circle));
}


TEST(EngineTest, TestCollisionRectAndCircleRightBottom)
{
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Circle circle(SekaiEngine::Math::Vector2D(35.0f, 20.0f), 5.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndCircle(rect, circle));
}

TEST(EngineTest, TestCollisionRectAndCircleRightInside)
{
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Circle circle(SekaiEngine::Math::Vector2D(33.0f, 20.0f), 5.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndCircle(rect, circle));
}

TEST(EngineTest, TestCollisionRectAndCircleInside)
{
    SekaiEngine::Shape::Rectangle rect(SekaiEngine::Math::Vector2D(10.0f, 10.0f), 20.0f, 15.0f);
    SekaiEngine::Shape::Circle circle(SekaiEngine::Math::Vector2D(20.0f, 20.0f), 5.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetweenRectAndCircle(rect, circle));
}

TEST(EngineTest, TestCollision2CircleOutside)
{
    SekaiEngine::Shape::Circle circle1(SekaiEngine::Math::Vector2D(5.0f, 5.0f), 10.0f);
    SekaiEngine::Shape::Circle circle2(SekaiEngine::Math::Vector2D(30.0f, 30.0f), 5.0f);
    EXPECT_FALSE(SekaiEngine::Math::isCollisionBetween2Circles(circle1, circle2));
}

TEST(EngineTest, TestCollision2CircleOverlap)
{
    SekaiEngine::Shape::Circle circle1(SekaiEngine::Math::Vector2D(5.0f, 5.0f), 10.0f);
    SekaiEngine::Shape::Circle circle2(SekaiEngine::Math::Vector2D(7.0f, 7.0f), 10.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetween2Circles(circle1, circle2));
}

TEST(EngineTest, TestCollision2CircleBorder)
{
    SekaiEngine::Shape::Circle circle1(SekaiEngine::Math::Vector2D(5.0f, 5.0f), 10.0f);
    SekaiEngine::Shape::Circle circle2(SekaiEngine::Math::Vector2D(25.0f, 5.0f), 10.0f);
    EXPECT_TRUE(SekaiEngine::Math::isCollisionBetween2Circles(circle1, circle2));
}