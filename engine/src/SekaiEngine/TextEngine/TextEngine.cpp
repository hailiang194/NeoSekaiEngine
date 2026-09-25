#include "SekaiEngine/TextEngine/TextEngine.h"

#include <cstring>

namespace SekaiEngine
{
    namespace TextEngine
    {
        std::vector<uint32_t> Utf8ToCodepoints(const char* text)
        {
            std::vector<uint32_t> codepoints;
            if(text == nullptr)
                return codepoints;

            const unsigned char* cursor = reinterpret_cast<const unsigned char*>(text);
            while(*cursor != '\0')
            {
                uint32_t codepoint = 0;
                if(*cursor < 0x80)
                {
                    codepoint = static_cast<uint32_t>(*cursor);
                    ++cursor;
                }
                else if((*cursor & 0xE0) == 0xC0)
                {
                    codepoint = (static_cast<uint32_t>(*cursor & 0x1F) << 6)
                        | static_cast<uint32_t>(cursor[1] & 0x3F);
                    cursor += 2;
                }
                else if((*cursor & 0xF0) == 0xE0)
                {
                    codepoint = (static_cast<uint32_t>(*cursor & 0x0F) << 12)
                        | (static_cast<uint32_t>(cursor[1] & 0x3F) << 6)
                        | static_cast<uint32_t>(cursor[2] & 0x3F);
                    cursor += 3;
                }
                else if((*cursor & 0xF8) == 0xF0)
                {
                    codepoint = (static_cast<uint32_t>(*cursor & 0x07) << 18)
                        | (static_cast<uint32_t>(cursor[1] & 0x3F) << 12)
                        | (static_cast<uint32_t>(cursor[2] & 0x3F) << 6)
                        | static_cast<uint32_t>(cursor[3] & 0x3F);
                    cursor += 4;
                }
                else
                {
                    ++cursor;
                    continue;
                }
                codepoints.push_back(codepoint);
            }
            return codepoints;
        }

        TextEngine::TextEngine()
            :m_library(), m_isAvaiable(false), m_fontFaces(), m_fontSizes(), m_glyphCache()
        {
            FT_Error error = FT_Init_FreeType(&m_library);
            m_isAvaiable = !error;
        }

        TextEngine::~TextEngine()
        {
            for(std::unordered_map<std::string, FT_Face>::const_iterator iter = m_fontFaces.cbegin(); iter != m_fontFaces.cend(); ++iter)
            {
                FT_Done_Face(iter->second);
            }
            m_fontFaces.clear();
            m_fontSizes.clear();
            m_glyphCache.clear();
            if(m_isAvaiable)
                FT_Done_FreeType(m_library);
        }

        bool TextEngine::LoadFontFace(const char* name, const char* path, const unsigned int& fontSize)
        {
            if(!m_isAvaiable || name == nullptr || path == nullptr)
                return false;

            if(m_fontFaces.find(std::string(name)) != m_fontFaces.end())
                return false;

            FT_Face fontFace = nullptr;
            if(FT_New_Face(m_library, path, 0, &fontFace) != 0)
                return false;

            if(FT_Set_Pixel_Sizes(fontFace, fontSize, fontSize) != 0)
            {
                FT_Done_Face(fontFace);
                return false;
            }

            m_fontFaces.insert(std::make_pair(name, fontFace));
            m_fontSizes.insert(std::make_pair(name, fontSize));
            return true;
        }

        bool TextEngine::GetGlyph(const char* faceName, uint32_t codepoint, GlyphMetric& out)
        {
            if(faceName == nullptr || !m_isAvaiable)
                return false;

            std::unordered_map<std::string, FT_Face>::const_iterator faceIter = m_fontFaces.find(std::string(faceName));
            if(faceIter == m_fontFaces.cend())
                return false;

            std::unordered_map<uint32_t, CachedGlyph>& cache = m_glyphCache[std::string(faceName)];
            std::unordered_map<uint32_t, CachedGlyph>::const_iterator glyphIter = cache.find(codepoint);
            if(glyphIter != cache.cend())
            {
                out = glyphIter->second.Metric;
                return true;
            }

            if(FT_Get_Char_Index(faceIter->second, codepoint) == 0)
                return false;

            if(FT_Load_Char(faceIter->second, codepoint, FT_LOAD_RENDER) != 0)
                return false;

            FT_GlyphSlot slot = faceIter->second->glyph;
            const FT_Bitmap& bitmap = slot->bitmap;
            if(bitmap.pixel_mode != FT_PIXEL_MODE_GRAY)
                return false;

            unsigned int rows = static_cast<unsigned int>(bitmap.rows);
            unsigned int width = static_cast<unsigned int>(bitmap.width);
            unsigned int pitch = static_cast<unsigned int>(bitmap.pitch);

            CachedGlyph& cached = cache[codepoint];
            cached.Pixels.resize(static_cast<std::size_t>(rows) * width);
            for(unsigned int row = 0; row < rows; ++row)
            {
                memcpy(&cached.Pixels[static_cast<std::size_t>(row) * width],
                    bitmap.buffer + static_cast<std::size_t>(row) * pitch, width);
            }

            cached.Metric.Buffer = cached.Pixels.empty() ? nullptr : cached.Pixels.data();
            cached.Metric.Width = width;
            cached.Metric.Height = rows;
            cached.Metric.BitmapLeft = static_cast<float>(slot->bitmap_left);
            cached.Metric.BitmapTop = static_cast<float>(slot->bitmap_top);
            cached.Metric.AdvanceX = static_cast<float>(slot->advance.x) / 64.0f;

            out = cached.Metric;
            return true;
        }

        unsigned int TextEngine::PixelSize(const char* faceName) const
        {
            if(faceName == nullptr)
                return 0;

            std::unordered_map<std::string, unsigned int>::const_iterator iter = m_fontSizes.find(std::string(faceName));
            if(iter == m_fontSizes.cend())
                return 0;
            return iter->second;
        }
    } // namespace TextEngine
    
} // namespace SekaiEngine