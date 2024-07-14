/**
 * @file Font.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Game engine font
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_RENDER_FONT_H_
#define _SEKAI_ENGINE_RENDER_FONT_H_

#include "SekaiEngine/BaseType.h"

namespace SekaiEngine
{
    namespace Render
    {
        /**
         * @brief Font class
         * 
         */
        class EXTENDAPI Font
        {
        public:
            /**
             * @brief Construct a new Font object
             * 
             * @param id id of font
             * @param filename the file path of font
             */
            Font(const char* id, const char* filename);

            /**
             * @brief Construct a new Font object
             * 
             * @param id id of font
             */
            Font(const char* id);

            /**
             * @brief Construct a new Font object
             * 
             * @param font copied object
             */
            Font(const Font& font);

            /**
             * @brief Copied assignement operator
             * 
             * @param font copied object
             * @return Font& the reference of itself
             */
            Font& operator=(const Font& font);

            /**
             * @brief Destroy the Font object
             * 
             */
            ~Font();
            
            /**
             * @brief Get the ID of font
             * 
             * @return const char* the ID of font
             */
            const char* Id() const;

            /**
             * @brief Get the ID of font
             * 
             * @return const char* the ID of font
             */
            const char* Id();

            /**
             * @brief Check if font is validated
             * 
             * @return true if font is validated
             * @return false if font is not validated
             */
            bool isValid() const;

            /**
             * @brief Check if font is validated
             * 
             * @return true if font is validated
             * @return false if font is not validated
             */
            bool isValid();
        private:
            const char* m_id; /*!< The ID of font*/
        };

        /**
         * @brief Initilized fonts
         * 
         */
        void initFonts();

        /**
         * @brief Core function to load font
         * 
         * @param id id of font
         * @param filename the file path of font
         * @return true if font is loaded
         * @return false if font is not loaded
         */
        bool loadFont(const char* id, const char* filename);

        /**
         * @brief Unload all the fonts
         * 
         */
        void unloadFonts();

        inline const char* Font::Id() const
        {
            return m_id;
        }

        inline const char* Font::Id()
        {
            return static_cast<const Font&>(*this).Id();
        }

        inline bool Font::isValid()
        {
            return static_cast<const Font&>(*this).isValid();
        }
    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_FONT_H_