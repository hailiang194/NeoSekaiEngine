#include <gtest/gtest.h>
#include <vector>
#include "SekaiEngine/TextEngine/TextEngine.h"

static const char* NOTO_FONT_PATH = FONT_PATH_NOTO;

TEST(EngineTest, TestTextEngineLoadFontFaceSuccess)
{
    SekaiEngine::TextEngine::TextEngine engine;
    EXPECT_TRUE(engine.IsAvaiable());
    EXPECT_TRUE(engine.LoadFontFace("noto", NOTO_FONT_PATH, 16));
}

TEST(EngineTest, TestTextEngineLoadFontFaceMissingFile)
{
    SekaiEngine::TextEngine::TextEngine engine;
    EXPECT_FALSE(engine.LoadFontFace("missing", "no-such-font-file.ttf", 16));
}

TEST(EngineTest, TestTextEngineLoadFontFaceDuplicate)
{
    SekaiEngine::TextEngine::TextEngine engine;
    EXPECT_TRUE(engine.LoadFontFace("dup", NOTO_FONT_PATH, 16));
    EXPECT_FALSE(engine.LoadFontFace("dup", NOTO_FONT_PATH, 24));

    SekaiEngine::TextEngine::GlyphMetric glyph;
    EXPECT_TRUE(engine.GetGlyph("dup", 0x41, glyph));
    EXPECT_TRUE(glyph.Width > 0 && glyph.Height > 0);
}

TEST(EngineTest, TestTextEngineUtf8ToCodepoints)
{
    const char* title = "Hello World. 你好，新世海的遊戲引擎";
    std::vector<uint32_t> codepoints = SekaiEngine::TextEngine::Utf8ToCodepoints(title);
    const uint32_t expected[] = {
        72, 101, 108, 108, 111, 32, 87, 111, 114, 108, 100, 46, 32,
        0x4F60, 0x597D, 0xFF0C, 0x65B0, 0x4E16, 0x6D77, 0x7684, 0x904A, 0x6232, 0x5F15, 0x64CE
    };
    const std::size_t total = 24;
    ASSERT_EQ(codepoints.size(), total);
    for(std::size_t i = 0; i < codepoints.size(); ++i)
    {
        EXPECT_EQ(codepoints[i], expected[i]);
    }
}

TEST(EngineTest, TestTextEngineGetGlyphRasterizesAndCaches)
{
    SekaiEngine::TextEngine::TextEngine engine;
    ASSERT_TRUE(engine.LoadFontFace("noto", NOTO_FONT_PATH, 16));

    SekaiEngine::TextEngine::GlyphMetric first;
    SekaiEngine::TextEngine::GlyphMetric second;
    EXPECT_TRUE(engine.GetGlyph("noto", 0x41, first));
    EXPECT_TRUE(engine.GetGlyph("noto", 0x41, second));

    EXPECT_TRUE(first.Width > 0 && first.Height > 0);
    EXPECT_EQ(first.Width, second.Width);
    EXPECT_EQ(first.Height, second.Height);
    EXPECT_EQ(first.AdvanceX, second.AdvanceX);
    EXPECT_TRUE(first.Buffer == second.Buffer);

    EXPECT_FALSE(engine.GetGlyph("noto", 0x1F600, first));
    EXPECT_FALSE(engine.GetGlyph("unknown-face", 0x41, first));
}

TEST(EngineTest, TestTextEngineGetGlyphCJKFromNoto)
{
    SekaiEngine::TextEngine::TextEngine engine;
    ASSERT_TRUE(engine.LoadFontFace("noto", NOTO_FONT_PATH, 24));

    SekaiEngine::TextEngine::GlyphMetric glyph;
    EXPECT_TRUE(engine.GetGlyph("noto", 0x4F60, glyph));
    EXPECT_TRUE(glyph.Width > 0 && glyph.Height > 0);
    EXPECT_TRUE(engine.GetGlyph("noto", 0x5DF2, glyph));
    EXPECT_TRUE(glyph.Width > 0 && glyph.Height > 0);
}
