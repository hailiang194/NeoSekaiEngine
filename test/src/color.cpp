#include <gtest/gtest.h>
#include "SekaiEngine/Render/Color.h"

TEST(EngineTest, TestColorCode)
{
    SekaiEngine::Render::Color color(0xf3b6a199);
    EXPECT_TRUE(color == (SekaiEngine::Render::Color)0xf3b6a199);
}

TEST(EngineTest, TestColorOperator)
{
    SekaiEngine::Render::Color color;
    color = static_cast<uint32_t>(0xf3b6a199);
    EXPECT_TRUE(color == (SekaiEngine::Render::Color)0xf3b6a199);
}

TEST(EngineTest, TestColorCodeRed)
{
    SekaiEngine::Render::Color color(0xf3b6a199);
    EXPECT_TRUE(color.r() ==(uint8_t)0xf3);
}

TEST(EngineTest, TestColorCodeGreen)
{
    SekaiEngine::Render::Color color(0xf3b6a199);
    EXPECT_TRUE(color.g() ==(uint8_t)0xb6);
}

TEST(EngineTest, TestColorCodeBlue)
{
    SekaiEngine::Render::Color color(0xf3b6a199);
    EXPECT_TRUE(color.b() ==(uint8_t)0xa1);
}

TEST(EngineTest, TestColorCodeAlpha)
{
    SekaiEngine::Render::Color color(0xf3b6a199);
    EXPECT_TRUE(color.a() ==(uint8_t)0x99);
}

