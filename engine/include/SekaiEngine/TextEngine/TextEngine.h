#ifndef _SEKAI_ENGINE_TEXT_ENGINE_TEXT_ENGINE_H_
#define _SEKAI_ENGINE_TEXT_ENGINE_TEXT_ENGINE_H_

#include <SekaiEngine/BaseType.h>
#include <unordered_map>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace SekaiEngine
{
    namespace TextEngine
    {
        class TextEngine
        {
        public:
            TextEngine();
            TextEngine(const TextEngine& engine) = delete;
            TextEngine& operator=(const TextEngine& engine) = delete;
            ~TextEngine();

            EXTENDAPI const bool& IsAvaiable() const;
            EXTENDAPI const bool& IsAvaiable();

            EXTENDAPI bool LoadFontFace(const char* name, const char* path, const unsigned int& fontSize);
            EXTENDAPI bool DrawCharacter(const int* uniChar, const char* fontName);

        private:
            FT_Library m_library;
            bool m_isAvaiable;
            std::unordered_map<std::string, FT_Face> m_fontFaces; 
        };

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