/**
 * @file Color.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Game engine color
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_RENDER_COLOR_H_
#define _SEKAI_ENGINE_RENDER_COLOR_H_

#include <stdint.h>
#include "SekaiEngine/BaseType.h"

namespace SekaiEngine
{
    namespace Render
    {
        /**
         * @brief Union for parsing color code to RGBA value
         * 
         */
        union ColorParser
        {
            uint32_t code; /*!< Color code*/
            /**
             * @brief parser struct
             * 
             */
            struct parser
            {
                uint8_t a; /*!< alpha value */
                uint8_t b; /*!< blue value*/
                uint8_t g; /*!< green value */
                uint8_t r; /*!< red value*/
            } value; /*!< struct hold the data */
        };

        /**
         * @brief Color class
         * 
         */
        class EXTENDAPI Color
        {
        public:
            /**
             * @brief Construct a new Color object
             * 
             * @param code color code whose format is RRGGBBAA
             */
            explicit Color(const uint32_t& code = 0x0);

            /**
             * @brief Construct a new Color object
             * 
             * @param color copied object
             */
            explicit Color(const Color& color);

            /**
             * @brief Copied assignment operator
             * 
             * @param color copied object
             * @return Color& the reference of object itself
             */
            Color& operator=(const Color& color);

            /**
             * @brief Copied assignment operator
             * 
             * @param code the color code whose format is RRGGBBAA 
             * @return Color& the reference of object itself
             */
            Color& operator=(const uint32_t& code);

            /**
             * @brief Check if 2 colors are the same
             * 
             * @param color checked color
             * @return true if 2 colors are the same
             * @return false if 2 colors are not the same
             */
            const bool operator==(const Color& color);

            /**
             * @brief Destroy the Color object
             * 
             */
            ~Color();

            /**
             * @brief Get color code
             * 
             * @return const uint32_t& the color code
             */
            const uint32_t& code() const;

            /**
             * @brief get color code
             * 
             * @return const uint32_t& the color code
             */
            const uint32_t& code();

            /**
             * @brief get the red value color
             * 
             * @return const uint8_t the red value
             */
            const uint8_t r() const;

            /**
             * @brief get the red value of color
             * 
             * @return const uint8_t the red value
             */
            const uint8_t r();

            /**
             * @brief get the green value of color
             * 
             * @return const uint8_t the green value
             */
            const uint8_t g() const;

            /**
             * @brief get the green value of color
             * 
             * @return const uint8_t the green value
             */
            const uint8_t g();

            /**
             * @brief get the blue value of color
             * 
             * @return const uint8_t the clue value
             */
            const uint8_t b() const;

            /**
             * @brief get the blue value of color
             * 
             * @return const uint8_t the blue value
             */
            const uint8_t b();

            /**
             * @brief get the alpha value of color
             * 
             * @return const uint8_t the alpha value
             */
            const uint8_t a() const;

            /**
             * @brief get the alpha value of color
             * 
             * @return const uint8_t the alpha value
             */
            const uint8_t a(); 
        private:
            uint32_t m_code; /*!< color code whose format as RRGGBBAA*/
        };

        inline const uint32_t& Color::code() const
        {
            return m_code;
        }

        inline const uint32_t& Color::code()
        {
            return static_cast<const Color&>(*this).code();
        }

        inline const uint8_t Color::r()
        {
            return static_cast<const Color&>(*this).r();
        }

        inline const uint8_t Color::g()
        {
            return static_cast<const Color&>(*this).g();
        }

        inline const uint8_t Color::b()
        {
            return static_cast<const Color&>(*this).b();
        }

        inline const uint8_t Color::a()
        {
            return static_cast<const Color&>(*this).a();
        }
    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_COLOR_H_