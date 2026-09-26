#ifndef _SEKAI_ENGINE_TEXT_ENGINE_TEXT_ENGINE_H_
#define _SEKAI_ENGINE_TEXT_ENGINE_TEXT_ENGINE_H_

#include <SekaiEngine/BaseType.h>
#include <SekaiEngine/Math/Vector.h>
#include <SekaiEngine/Render/Color.h>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace SekaiEngine
{
    namespace TextEngine
    {
        /**
         * @brief Rasterized glyph: tightly packed 8-bit coverage bitmap plus metrics
         */
        struct GlyphMetric
        {
            const unsigned char* Buffer; /*!< coverage bitmap, Width x Height, row-major */
            unsigned int Width;          /*!< bitmap width */
            unsigned int Height;         /*!< bitmap height */
            float BitmapLeft;            /*!< horizontal bearing */
            float BitmapTop;             /*!< vertical bearing */
            float AdvanceX;              /*!< horizontal pen advance */
        };

        /**
         * @brief Decode a UTF-8 string into a sequence of Unicode codepoints
         * 
         * @param text the UTF-8 encoded string
         * @return std::vector<uint32_t> the decoded codepoints
         */
        EXTENDAPI std::vector<uint32_t> Utf8ToCodepoints(const char* text);

        class TextEngine
        {
        public:
            EXTENDAPI TextEngine();
            TextEngine(const TextEngine& engine) = delete;
            TextEngine& operator=(const TextEngine& engine) = delete;
            EXTENDAPI ~TextEngine();

            EXTENDAPI const bool& IsAvaiable() const;
            EXTENDAPI const bool& IsAvaiable();

            EXTENDAPI bool LoadFontFace(const char* name, const char* path, const unsigned int& fontSize);
            EXTENDAPI bool GetGlyph(const char* faceName, uint32_t codepoint, GlyphMetric& out);
            EXTENDAPI unsigned int PixelSize(const char* faceName) const;
            EXTENDAPI void DrawText(const char* text, const Math::Vector2D& position, const Render::Color& color, const char* fontName);

        private:
            /**
             * @brief Cached rasterization of one codepoint
             */
            struct CachedGlyph
            {
                std::vector<unsigned char> Pixels; /*!< stable storage for the coverage bitmap */
                GlyphMetric Metric;                /*!< metrics pointing into Pixels */
            };

            FT_Library m_library;
            bool m_isAvaiable;
            std::unordered_map<std::string, FT_Face> m_fontFaces;
            std::unordered_map<std::string, unsigned int> m_fontSizes;
            std::unordered_map<std::string, std::unordered_map<uint32_t, CachedGlyph> > m_glyphCache;
        };

        /**
         * @brief Initialize the glyph texture cache
         */
        EXTENDAPI void initTextEngine();

        /**
         * @brief Free all glyph textures held by the text engine
         */
        EXTENDAPI void unloadTextEngine();

        inline const bool& TextEngine::IsAvaiable() const
        {
            return m_isAvaiable;
        }

        inline const bool& TextEngine::IsAvaiable()
        {
            return static_cast<const TextEngine&>(*this).IsAvaiable();
        }

    } // namespace TextEngine
    
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_TEXT_ENGINE_TEXT_ENGINE_H_