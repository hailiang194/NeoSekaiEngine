#include "SekaiEngine/Render/Texture.h"
#include "wrapper/raylib/Texture.h"

#ifdef USE_RAYLIB
#include "raylib.h"
#include <unordered_map>
#include <stdexcept>

namespace SekaiEngine
{
    namespace Render
    {
        Texture::Texture(const char* filename)
            :m_id(0), m_width(0), m_height(0)
        {
            m_id = LoadTextureFromFileAndGetID(filename, &m_width, &m_height);
        }

        std::unordered_map<int,::Texture> textures;
        void initTextures()
        {
            textures.clear();
        }

        int LoadTextureFromFileAndGetID(const char * filename, int* width, int* height)
        {
            if(width != nullptr)
                *width = 0;
            if(height != nullptr)
                *height = 0;

            ::Texture texture = LoadTexture(filename);
            if(texture.id == 0)
                return texture.id;

            if(width != nullptr)
                *width = texture.width;
            if(height != nullptr)
                *height = texture.height;

            textures.insert({texture.id, texture});
            return texture.id;
        }


        ::Texture* getTexture(const int& id)
        {
            try
            {
                return &textures.at(id);
            }catch(const std::out_of_range& e)
            {
                return nullptr;
            }
        }

        void destroyTextures()
        {
            for(auto iter = textures.begin(); iter != textures.end(); ++iter)
            {
                UnloadTexture(iter->second);
            }
            textures.clear();
        }
    } // namespace Render
    
} // namespace SekaiEngine


#endif
