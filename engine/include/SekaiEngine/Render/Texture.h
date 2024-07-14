/**
 * @file Texture.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Engine texture
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_RENDER_TEXTURE_H_
#define _SEKAI_ENGINE_RENDER_TEXTURE_H_

#include "SekaiEngine/BaseType.h"

namespace SekaiEngine
{
    namespace Render
    {
        /**
         * @brief Texture class
         * 
         */
        class EXTENDAPI Texture
        {
        public:
            /**
             * @brief Construct a new Texture object
             * 
             * @param filename file path of texture
             */
            Texture(const char* filename);

            /**
             * @brief Construct a new Texture object
             * 
             * @param texture copied object
             */
            Texture(const Texture& texture);

            /**
             * @brief Copied assignment operator
             * 
             * @param texture copied object
             * @return Texture& the reference of the object itself
             */
            Texture& operator=(const Texture& texture);

            /**
             * @brief Destroy the Texture object
             * 
             */
            ~Texture();

            /**
             * @brief Check if texture is validated
             * 
             * @return true if the texture is validated
             * @return false if the texture is not validated
             */
            bool IsValidTexture();

            /**
             * @brief Get the ID of texture
             * 
             * @return const int& the ID
             */
            const int& Id() const;

            /**
             * @brief Get the ID of texture
             * 
             * @return const int& the ID
             */
            const int& Id();

            /**
             * @brief Get the width of texture
             * 
             * @return int the width
             */
            int Width() const;

            /**
             * @brief Get the width of texture
             * 
             * @return int the width
             */
            int Width();

            /**
             * @brief Get the height of texture
             * 
             * @return int the height
             */
            int Height() const;

            /**
             * @brief Get the height of texture
             * 
             * @return int the height
             */
            int Height();
        private:
            int m_id; /*!< The ID of texture */
        };

        /**
         * @brief initialize textures
         * 
         */
        void initTextures();

        /**
         * @brief Load the texture and get the id
         * 
         * @param filename the file path of texture
         * @return int the ID of loaded texture. If the ID = 0, the texture is failed to load
         */
        int LoadTextureFromFileAndGetID(const char * filename);

        /**
         * @brief Destroy all textures
         * 
         */
        void destroyTextures();

        inline bool Texture::IsValidTexture()
        {
            return m_id != 0;
        }

        inline const int& Texture::Id() const
        {
            return m_id;
        }

        inline const int& Texture::Id()
        {
            return static_cast<const Texture&>(*this).Id();
        }

        inline int Texture::Width()
        {
            return static_cast<const Texture&>(*this).Width();
        }

        inline int Texture::Height()
        {
            return static_cast<const Texture&>(*this).Height();
        }

    } // namespace Render
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_RENDER_TEXTURE_H_