#include "SekaiEngine/Audio/Sound.h"

namespace SekaiEngine
{
    namespace Sound
    {
            Sound::Sound(const char* filename)
            {
                m_id = loadSound(filename);
            }

            Sound::Sound(const size_t& id)
                :m_id(id)
            {

            }

            Sound::Sound(const Sound& sound)
                :m_id(sound.m_id)
            {

            }
            Sound& Sound::operator=(const Sound& sound)
            {
                m_id = sound.m_id;
                return (*this);
            }

            Sound::~Sound()
            {

            }
    } // namespace Sound
    
} // namespace SekaiEngine
