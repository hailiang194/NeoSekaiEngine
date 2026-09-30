/**
 * @file TextEngine.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Glyph rasterization and text drawing
 * @version 0.1
 * @date 2024-08-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */
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

        /**
         * @brief Text engine: rasterizes glyphs from font faces and caches them
         */
        class TextEngine
        {
        public:
            /**
             * @brief Construct a new TextEngine object
             *
             * @note check IsAvaiable to know if the underlying library was initialized
             */
            EXTENDAPI TextEngine();

            /**
             * @brief The copy constructor is deleted, the engine owns non copyable resources
             *
             */
            TextEngine(const TextEngine& engine) = delete;

            /**
             * @brief The copied assignment operator is deleted, the engine owns non copyable resources
             *
             */
            TextEngine& operator=(const TextEngine& engine) = delete;

            /**
             * @brief Destroy the TextEngine object and release all the loaded font faces
             *
             */
            EXTENDAPI ~TextEngine();

            /**
             * @brief Check if the text engine is available for use
             *
             * @return const bool& true the engine is initialized and usable
             * @return const bool& false the underlying library failed to initialize
             */
            EXTENDAPI const bool& IsAvaiable() const;

            /**
             * @brief Check if the text engine is available for use
             *
             * @return const bool& true the engine is initialized and usable
             * @return const bool& false the underlying library failed to initialize
             */
            EXTENDAPI const bool& IsAvaiable();

            /**
             * @brief Load a font face and set its pixel size
             *
             * @param name the name used later to refer to this face
             * @param path the path of the font file
             * @param fontSize the pixel size of the face
             * @return true the font face is loaded
             * @return false the engine is unavailable, the name is already taken, or the file cannot be loaded
             */
            EXTENDAPI bool LoadFontFace(const char* name, const char* path, const unsigned int& fontSize);

            /**
             * @brief Get the rasterized glyph of a codepoint, the result is cached
             *
             * @param faceName the name of the font face given to LoadFontFace
             * @param codepoint the codepoint to rasterize
             * @param out the metric of the glyph, only written on success
             * @return true the glyph is rasterized
             * @return false the face is unknown, or the face has no glyph for the codepoint
             * @note the returned buffer belongs to the engine and stays valid until the glyph is re-cached or the engine is destroyed
             */
            EXTENDAPI bool GetGlyph(const char* faceName, uint32_t codepoint, GlyphMetric& out);

            /**
             * @brief Get the pixel size of a font face
             *
             * @param faceName the name of the font face given to LoadFontFace
             * @return unsigned int the pixel size of the face, or 0 if the face is unknown
             */
            EXTENDAPI unsigned int PixelSize(const char* faceName) const;

            /**
             * @brief Draw a UTF-8 encoded text with a loaded font face
             *
             * @param text the UTF-8 encoded text to draw
             * @param position the position of the left baseline of the text
             * @param color the tint color of the text
             * @param fontName the name of the font face given to LoadFontFace
             * @note a codepoint missing from the face is skipped and advances the pen by half the pixel size
             */
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