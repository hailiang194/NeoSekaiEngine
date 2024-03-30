#include <gtest/gtest.h>
#include "SekaiEngine/Math/Vector.h"

TEST(EngineTest, TestVector2D_XValue)
{
    SekaiEngine::Math::Vector2D test(10.0f, 20.0f);
    EXPECT_TRUE(test.X() == 10.0f);
}

TEST(EngineTest, TestVector2D_YValue)
{
    SekaiEngine::Math::Vector2D test(10.0f, 20.0f);
    EXPECT_TRUE(test.Y() == 20.0f);
}

TEST(EngineTest, TestVector3D_XValue)
{
    SekaiEngine::Math::Vector3D test(10.0f, 20.0f, 30.f);
    EXPECT_TRUE(test.X() == 10.0f);
}

TEST(EngineTest, TestVector3D_YValue)
{
    SekaiEngine::Math::Vector3D test(10.0f, 20.0f, 30.f);
    EXPECT_TRUE(test.Y() == 20.0f);
}

TEST(EngineTest, TestVector3D_ZValue)
{
    SekaiEngine::Math::Vector3D test(10.0f, 20.0f, 30.f);
    EXPECT_TRUE(test.Z() == 30.0f);
}

TEST(EngineTest, TestVector4D_XValue)
{
    SekaiEngine::Math::Vector4D test(10.0f, 20.0f, 30.f, 40.0f);
    EXPECT_TRUE(test.X() == 10.0f);
}

TEST(EngineTest, TestVector4D_YValue)
{
    SekaiEngine::Math::Vector4D test(10.0f, 20.0f, 30.f, 40.0f);
    EXPECT_TRUE(test.Y() == 20.0f);
}

TEST(EngineTest, TestVector4D_ZValue)
{
    SekaiEngine::Math::Vector4D test(10.0f, 20.0f, 30.f, 40.0f);
    EXPECT_TRUE(test.Z() == 30.0f);
}

TEST(EngineTest, TestVector4D_TValue)
{
    SekaiEngine::Math::Vector4D test(10.0f, 20.0f, 30.f, 40.0f);
    EXPECT_TRUE(test.T() == 40.0f);
}

TEST(EngineTest, TestVector2D_Add)
{
    SekaiEngine::Math::Vector2D test(3.0f, 4.0f);
    EXPECT_TRUE(test + test == SekaiEngine::Math::Vector2D(6.0f, 8.0f));
}

TEST(EngineTest, TestVector2D_Subtract)
{
    SekaiEngine::Math::Vector2D test(3.0f, 4.0f);
    EXPECT_TRUE(test - test == SekaiEngine::Math::Vector2D(0.0f, 0.0f));
}

TEST(EngineTest, TestVector2D_Scale)
{
    SekaiEngine::Math::Vector2D test(3.0f, 4.0f);
    EXPECT_TRUE(test * 2.0f == SekaiEngine::Math::Vector2D(6.0f, 8.0f));
}

TEST(EngineTest, TestVector2D_Distance)
{
    SekaiEngine::Math::Vector2D test(3.0f, 4.0f);
    EXPECT_TRUE(SekaiEngine::Math::cmpFloat(test.Distance(), 5.0f) == 0);
}

TEST(EngineTest, TestVector2D_Dot)
{
    SekaiEngine::Math::Vector2D test(3.0f, 4.0f);
    EXPECT_TRUE(SekaiEngine::Math::cmpFloat(test.Dot(test), 25.0f) == 0);
}

TEST(EngineTest, TestVector2D_AssignAdd)
{
    SekaiEngine::Math::Vector2D op1(3.0f, 4.0f);
    SekaiEngine::Math::Vector2D op2(3.0f, 4.0f);

    SekaiEngine::Math::Vector2D result = op1 + op2;
    EXPECT_TRUE(result == SekaiEngine::Math::Vector2D(6.0f, 8.0f));
}

TEST(EngineTest, TestVector2D_AssignMinus)
{
    SekaiEngine::Math::Vector2D op1(3.0f, 4.0f);
    SekaiEngine::Math::Vector2D op2(3.0f, 4.0f);

    SekaiEngine::Math::Vector2D result = op1 - op2;
    EXPECT_TRUE(result == SekaiEngine::Math::Vector2D(0.0f, 0.0f));
}

TEST(EngineTest, TestVector2D_AssignScale)
{
    SekaiEngine::Math::Vector2D op1(3.0f, 4.0f);

    SekaiEngine::Math::Vector2D result = op1 * 0.5f;
    EXPECT_TRUE(result == SekaiEngine::Math::Vector2D(1.5f, 2.0f));
}

TEST(EngineTest, TestVector3D_AssignAdd)
{
    SekaiEngine::Math::Vector3D op1(3.0f, 4.0f, 5.0f);
    SekaiEngine::Math::Vector3D op2(3.0f, 4.0f, 5.0f);

    SekaiEngine::Math::Vector3D result = op1 + op2;
    EXPECT_TRUE(result == SekaiEngine::Math::Vector3D(6.0f, 8.0f, 10.0f));
}

TEST(EngineTest, TestVector3D_AssignMinus)
{
    SekaiEngine::Math::Vector3D op1(3.0f, 4.0f, 5.0f);
    SekaiEngine::Math::Vector3D op2(3.0f, 4.0f, 5.0f);

    SekaiEngine::Math::Vector3D result = op1 - op2;
    EXPECT_TRUE(result == SekaiEngine::Math::Vector3D(0.0f, 0.0f, 0.0f));
}

TEST(EngineTest, TestVector3D_AssignScale)
{
    SekaiEngine::Math::Vector3D op1(3.0f, 4.0f, 5.0f);

    SekaiEngine::Math::Vector3D result = op1 * 0.5f;
    EXPECT_TRUE(result == SekaiEngine::Math::Vector3D(1.5f, 2.0f, 2.5f));
}