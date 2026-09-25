#include "SekaiEngine/TextEngine/TextEngine.h"

#ifdef USE_RAYLIB
#include "raylib.h"
#include "wrapper/raylib/Parser.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace SekaiEngine
{
    namespace TextEngine
    {
        /**
         * @brief Cached GPU texture for one glyph
         */
        struct GlyphTexture
        {
            ::Texture2D Texture;
        };

        static std::unordered_map<std::string, std::unordered_map<uint32_t, GlyphTexture> > g_glyphTextures;

        void initTextEngine()
        {
            g_glyphTextures.clear();
        }

        void unloadTextEngine()
        {
            for(std::unordered_map<std::string, std::unordered_map<uint32_t, GlyphTexture> >::iterator faceIter = g_glyphTextures.begin(); faceIter != g_glyphTextures.end(); ++faceIter)
            {
                for(std::unordered_map<uint32_t, GlyphTexture>::iterator glyphIter = faceIter->second.begin(); glyphIter != faceIter->second.end(); ++glyphIter)
                {
                    UnloadTexture(glyphIter->second.Texture);
                }
            }
            g_glyphTextures.clear();
        }

        namespace
        {
            /**
             * @brief Get the cached GPU texture for a glyph, uploading it on first use
             * 
             * @param faceName the face the glyph belongs to
             * @param codepoint the glyph codepoint
             * @param glyph the rasterized glyph metrics
             * @return ::Texture2D* pointer to the cached texture (never null)
             */
            ::Texture2D* getGlyphTexture(const char* faceName, uint32_t codepoint, const GlyphMetric& glyph)
            {
                std::string faceKey(faceName);
                std::unordered_map<uint32_t, GlyphTexture>& faceCache = g_glyphTextures[faceKey];
                std::unordered_map<uint32_t, GlyphTexture>::iterator glyphIter = faceCache.find(codepoint);
                if(glyphIter != faceCache.end())
                    return &glyphIter->second.Texture;

                std::vector<unsigned char> rgba(static_cast<std::size_t>(glyph.Width) * glyph.Height * 4, 0x00);
                for(unsigned int y = 0; y < glyph.Height; ++y)
                {
                    for(unsigned int x = 0; x < glyph.Width; ++x)
                    {
                        unsigned char coverage = glyph.Buffer[static_cast<std::size_t>(y) * glyph.Width + x];
                        unsigned char* pixel = &rgba[(static_cast<std::size_t>(y) * glyph.Width + x) * 4];
                        pixel[0] = 0xFF;
                        pixel[1] = 0xFF;
                        pixel[2] = 0xFF;
                        pixel[3] = coverage;
                    }
                }

                ::Image image;
                image.data = rgba.data();
                image.width = static_cast<int>(glyph.Width);
                image.height = static_cast<int>(glyph.Height);
                image.mipmaps = 1;
                image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

                GlyphTexture entry;
                entry.Texture = LoadTextureFromImage(image);
                faceCache.insert(std::make_pair(codepoint, entry));

                std::unordered_map<uint32_t, GlyphTexture>::iterator cached = faceCache.find(codepoint);
                return &cached->second.Texture;
            }
        } // namespace

        void TextEngine::DrawText(const char* text, const Math::Vector2D& position, const Render::Color& color, const char* fontName)
        {
            if(!m_isAvaiable || text == nullptr || fontName == nullptr)
                return;

            std::vector<uint32_t> codepoints = Utf8ToCodepoints(text);
            float penX = position.X();
            const unsigned int pixelSize = PixelSize(fontName);
            const float missingAdvance = static_cast<float>(pixelSize) / 2.0f;

            for(std::vector<uint32_t>::iterator iter = codepoints.begin(); iter != codepoints.end(); ++iter)
            {
                GlyphMetric glyph;
                if(!GetGlyph(fontName, *iter, glyph))
                {
                    penX += missingAdvance;
                    continue;
                }

                if(glyph.Width == 0 || glyph.Height == 0)
                {
                    penX += glyph.AdvanceX;
                    continue;
                }

                ::Texture2D* texture = getGlyphTexture(fontName, *iter, glyph);
                if(texture->id == 0)
                {
                    penX += glyph.AdvanceX;
                    continue;
                }

                ::Rectangle source = { 0.0f, 0.0f, static_cast<float>(glyph.Width), static_cast<float>(glyph.Height) };
                ::Rectangle dest = {
                    penX + glyph.BitmapLeft,
                    position.Y() - glyph.BitmapTop,
                    static_cast<float>(glyph.Width),
                    static_cast<float>(glyph.Height)
                };
                ::DrawTexturePro(*texture, source, dest, ::Vector2{ 0.0f, 0.0f }, 0.0f, parseToRaylibColor(color));
                penX += glyph.AdvanceX;
            }
        }
    } // namespace TextEngine
    
} // namespace SekaiEngine

#endif