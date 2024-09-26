#include "SekaiEngine/Audio/MusicStream.h"

namespace SekaiEngine
{
    namespace Sound
    {
        MusicStream::MusicStream(const char* filename)
        {
            m_id = loadMusicStream(filename);
        }

        MusicStream::MusicStream(const size_t& id)
            :m_id(id)
        {

        }

        MusicStream::MusicStream(const MusicStream& stream)
            :m_id(stream.m_id)
        {

        }

        MusicStream& MusicStream::operator=(const MusicStream& stream)
        {
            m_id = stream.m_id;
            return (*this);
        }

        MusicStream::~MusicStream()
        {

        }
    } // namespace Sound
    
} // namespace SekaiEngine
