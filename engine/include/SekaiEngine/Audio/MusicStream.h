#ifndef _SEKAI_ENGINE_SOUND_MUSIC_STREAM_H_
#define _SEKAI_ENGINE_SOUND_MUSIC_STREAM_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Timer.h"
#include <cstddef>

namespace SekaiEngine
{
    namespace Sound
    {
        class EXTENDAPI MusicStream
        {
        public:
            MusicStream(const char* filename);
            MusicStream(const size_t& id);
            MusicStream(const MusicStream& stream);
            MusicStream& operator=(const MusicStream& stream);
            ~MusicStream();

            const size_t& Id() const;
            const size_t& Id();

            const bool IsValid() const;
            const bool IsValid();

            const bool IsPlaying() const;
            const bool IsPlaying();

            const Timestep Length() const;
            const Timestep Length();

            const Timestep Played() const;
            const Timestep Played();

            void Play();
            void Stop();
            void Pause();
            void Resume();
            void Seek(const Timestep& position);
        private:
            size_t m_id;
        };

        void initMusicStreams();
        size_t loadMusicStream(const char* filename);
        void updateMusicStream();
        void unloadMusicStreams();

        inline const size_t& MusicStream::Id() const
        {
            return m_id;
        }

        inline const size_t& MusicStream::Id()
        {
            return static_cast<const MusicStream&>(*this).Id();
        }

        inline const bool MusicStream::IsValid()
        {
            return static_cast<const MusicStream&>(*this).IsValid();
        }

        inline const bool MusicStream::IsPlaying()
        {
            return static_cast<const MusicStream&>(*this).IsPlaying();
        }

        inline const Timestep MusicStream::Length()
        {
            return static_cast<const MusicStream&>(*this).Length();
        }

        inline const Timestep MusicStream::Played()
        {
            return static_cast<const MusicStream&>(*this).Played();
        }
    } // namespace Sound
    
} // namespace SekaiEngine


#endif //!_SEKAI_ENGINE_SOUND_MUSIC_STREAM_H_