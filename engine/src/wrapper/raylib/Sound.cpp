#include "SekaiEngine/Audio/Sound.h"
#include <vector>

#define INVALID_SOUND -1

#ifdef USE_RAYLIB
#include "raylib.h"

namespace SekaiEngine
{
    namespace Sound
    {
        std::vector<::Sound> sounds;

        void initSounds()
        {
            sounds.clear();
        }

        size_t loadSound(const char* filename)
        {
            ::Sound sound = ::LoadSound(filename);
            if(!IsSoundReady(sound))
            {
                return INVALID_SOUND;
            }

            sounds.push_back(sound);

            return sounds.size() - 1;
        }

        void unloadSounds()
        {
            for(std::vector<::Sound>::const_iterator iter = sounds.cbegin(); iter != sounds.cend(); ++iter)
            {
                ::UnloadSound(*iter);
            }

            sounds.clear();
        }

        const bool Sound::IsValid() const
        {
            return m_id != INVALID_SOUND;
        }

        void Sound::Play()
        {
            ::PlaySound(sounds.at(m_id));
        }
        void Sound::Stop()
        {
            ::StopSound(sounds.at(m_id));
        }

        void Sound::Pause()
        {
            ::PauseSound(sounds.at(m_id));
        }

        void Sound::Resume()
        {
            ::ResumeSound(sounds.at(m_id));
        }

        const bool Sound::IsPlaying() const
        {
            return ::IsSoundPlaying(sounds.at(m_id));
        }
    } // namespace Sound
    
} // namespace SekaiEngine
#endif