#include <gtest/gtest.h>
#include "SekaiEngine/Timer.h"

TEST(EngineTest, TestTimestepOperatorEqualTrue)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_TRUE(left == right);
}

TEST(EngineTest, TestTimestepOperatorEqualFalse)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1846f);

    EXPECT_FALSE(left == right);
}

TEST(EngineTest, TestTimestepOperatorNotEqualTrue)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1846f);

    EXPECT_TRUE(left != right);
}

TEST(EngineTest, TestTimestepOperatorNotEqualFalse)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_FALSE(left != right);
}

TEST(EngineTest, TestTimestepOperatorLessThanTrue)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1846f);

    EXPECT_TRUE(left < right);
}

TEST(EngineTest, TestTimestepOperatorLessThanByEqual)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_FALSE(left < right);
}

TEST(EngineTest, TestTimestepOperatorLessThanByGreaterThan)
{
    SekaiEngine::Timestep left(8.1846f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_FALSE(left < right);
}

TEST(EngineTest, TestTimestepOperatorLessThanOrEqualToByLessThan)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1846f);

    EXPECT_TRUE(left <= right);
}

TEST(EngineTest, TestTimestepOperatorLessThanOrEqualToByEqualTo)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_TRUE(left <= right);
}

TEST(EngineTest, TestTimestepOperatorLessThanOrEqualToByGreaterThan)
{
    SekaiEngine::Timestep left(8.1846f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_FALSE(left <= right);
}

TEST(EngineTest, TestTimestepOperatorGreaterThanByGreaterThan)
{
    SekaiEngine::Timestep left(8.1846f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_TRUE(left > right);
}

TEST(EngineTest, TestTimestepOperatorGreaterThanByEqual)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_FALSE(left > right);
}

TEST(EngineTest, TestTimestepOperatorGreaterThanByLessThan)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1846f);

    EXPECT_FALSE(left > right);
}

TEST(EngineTest, TestTimestepOperatorGreaterThanOrEqualByGreaterThan)
{
    SekaiEngine::Timestep left(8.1846f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_TRUE(left >= right);
}

TEST(EngineTest, TestTimestepOperatorGreaterThanOrEqualByEqualTo)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1845f);

    EXPECT_TRUE(left >= right);
}

TEST(EngineTest, TestTimestepOperatorGreaterThanOrEqualByLessThan)
{
    SekaiEngine::Timestep left(8.1845f);
    SekaiEngine::Timestep right(8.1846f);

    EXPECT_FALSE(left >= right);
}