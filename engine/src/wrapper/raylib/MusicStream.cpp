#include "SekaiEngine/Audio/MusicStream.h"
#include <vector>

#define INVALID_MUSIC -1

#ifdef USE_RAYLIB

#include "raylib.h"

namespace SekaiEngine
{
    namespace Sound
    {
        std::vector<::Music> musics;
        void initMusicStreams()
        {
            musics.clear();
        }

        size_t loadMusicStream(const char* filename)
        {
            ::Music music = ::LoadMusicStream(filename);
            if(!IsMusicReady(music))
            {
                return INVALID_MUSIC;
            }

            musics.push_back(music);
            return musics.size() - 1;

        }

        void unloadMusicStreams()
        {
            for(std::vector<::Music>::const_iterator iter = musics.cbegin(); iter != musics.cend(); ++iter)
            {
                ::UnloadMusicStream(*iter);
            }
            musics.clear();
        }

        const bool MusicStream::IsValid() const
        {
            return m_id != INVALID_MUSIC;
        }

        const bool MusicStream::IsPlaying() const
        {
            return ::IsMusicStreamPlaying(musics.at(m_id));
        }

        const Timestep MusicStream::Length() const
        {
            return (Timestep)::GetMusicTimeLength(musics.at(m_id));
        }

        const Timestep MusicStream::Played() const
        {
            return (Timestep)::GetMusicTimePlayed(musics.at(m_id));
        }

        void MusicStream::Play()
        {
            ::PlayMusicStream(musics.at(m_id));
        }

        void MusicStream::Stop()
        {
            ::StopMusicStream(musics.at(m_id));
        }

        void MusicStream::Pause()
        {
            ::PauseMusicStream(musics.at(m_id));
        }

        void MusicStream::Resume()
        {
            ::ResumeMusicStream(musics.at(m_id));
        }

        void MusicStream::Seek(const Timestep& position)
        {
            ::SeekMusicStream(musics.at(m_id), position.ToSeconds());
        }
    } // namespace Sound
    
} // namespace SekaiEngine
#endif
