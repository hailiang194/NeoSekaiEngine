#include "SekaiEngine/TextEngine/TextEngine.h"

namespace SekaiEngine
{
    namespace TextEngine
    {
        TextEngine::TextEngine()
            :m_library(), m_isAvaiable(true), m_fontFaces()
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
            FT_Done_FreeType(m_library);
        }

        bool TextEngine::LoadFontFace(const char* name, const char* path, const unsigned int& fontSize)
        {
            FT_Face fontFace;
            if(!FT_New_Face(m_library, path, 0, &fontFace))
            {
                return false;
            }

            if(!FT_Set_Pixel_Sizes(fontFace, fontSize, fontSize))
            {
                return false;
            }

            m_fontFaces.insert({name, fontFace});

            return true;
        }

        bool TextEngine::DrawCharacter(const int* uniChar, const char* fontName)
        {
            if(m_fontFaces.find(fontName) == m_fontFaces.end())
            {
                return false;
            }

            return true;
        }
    } // namespace TextEngine
    
} // namespace SekaiEngine
