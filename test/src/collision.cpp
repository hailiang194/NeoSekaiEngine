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