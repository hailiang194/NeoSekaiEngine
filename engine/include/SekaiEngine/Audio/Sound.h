#ifndef _SEKAI_ENGINE_AUDIO_SOUND_H_
#define _SEKAI_ENGINE_AUDIO_SOUND_H_

#include "SekaiEngine/BaseType.h"

namespace SekaiEngine
{
    namespace Sound
    {
        class EXTENDAPI Sound
        {
        public:
            Sound(const char* filename);
            Sound(const size_t& id);
            Sound(const Sound& sound);
            Sound& operator=(const Sound& sound);
            ~Sound();

            const bool IsValid() const;
            const bool IsValid();

            const size_t& Id() const;
            const size_t& Id();

            void Play();
            void Stop();
            void Pause();
            void Resume();

            const bool IsPlaying() const;
            const bool IsPlaying();
        private:
            size_t m_id;
        };

        void initSounds();
        size_t loadSound(const char* filename);
        void unloadSounds();

        inline const bool Sound::IsValid()
        {
            return static_cast<const Sound&>(*this).IsValid();
        }

        inline const size_t& Sound::Id() const
        {
            return m_id;
        }

        inline const size_t& Sound::Id()
        {
            return static_cast<const Sound&>(*this).Id();
        }

        inline const bool Sound::IsPlaying()
        {
            return static_cast<const Sound&>(*this).IsPlaying();
        }
    } // namespace Sound
    
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_AUDIO_SOUND_H_