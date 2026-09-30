/**
 * @file MusicStream.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Streaming music track
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_SOUND_MUSIC_STREAM_H_
#define _SEKAI_ENGINE_SOUND_MUSIC_STREAM_H_

#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Timer.h"
#include <cstddef>

namespace SekaiEngine
{
    namespace Sound
    {
        /**
         * @brief Streaming music track
         *
         * The object itself only holds the id of a track owned by the engine, so
         * copying one is cheap and never duplicates the underlying audio data.
         */
        class EXTENDAPI MusicStream
        {
        public:
            /**
             * @brief Construct a new MusicStream object from a file
             *
             * @param filename the path of the music file to stream
             * @note the resulting stream is invalid if the file cannot be loaded
             */
            MusicStream(const char* filename);

            /**
             * @brief Construct a new MusicStream object from an existing stream id
             *
             * @param id the id of a music stream already loaded by loadMusicStream
             */
            MusicStream(const size_t& id);

            /**
             * @brief Construct a new MusicStream object
             *
             * @param stream copied object, the copy shares the same underlying track
             */
            MusicStream(const MusicStream& stream);

            /**
             * @brief Copied assignment operator
             *
             * @param stream the object copy from
             * @return MusicStream& the reference of the object itself
             */
            MusicStream& operator=(const MusicStream& stream);

            /**
             * @brief Destroy the MusicStream object
             *
             * @note the track itself stays loaded, call unloadMusicStreams to release it
             */
            ~MusicStream();

            /**
             * @brief Get the id of the music stream
             *
             * @return const size_t& the id of the music stream
             */
            const size_t& Id() const;

            /**
             * @brief Get the id of the music stream
             *
             * @return const size_t& the id of the music stream
             */
            const size_t& Id();

            /**
             * @brief Check if the music stream refers to a loaded track
             *
             * @return true the stream refers to a loaded track
             * @return false the stream is invalid
             */
            const bool IsValid() const;

            /**
             * @brief Check if the music stream refers to a loaded track
             *
             * @return true the stream refers to a loaded track
             * @return false the stream is invalid
             */
            const bool IsValid();

            /**
             * @brief Check if the music stream is currently playing
             *
             * @return true the stream is playing
             * @return false the stream is not playing
             */
            const bool IsPlaying() const;

            /**
             * @brief Check if the music stream is currently playing
             *
             * @return true the stream is playing
             * @return false the stream is not playing
             */
            const bool IsPlaying();

            /**
             * @brief Get the total duration of the music stream
             *
             * @return const Timestep the duration of the whole track
             */
            const Timestep Length() const;

            /**
             * @brief Get the total duration of the music stream
             *
             * @return const Timestep the duration of the whole track
             */
            const Timestep Length();

            /**
             * @brief Get the time the music stream has played so far
             *
             * @return const Timestep the position reached in the track
             */
            const Timestep Played() const;

            /**
             * @brief Get the time the music stream has played so far
             *
             * @return const Timestep the position reached in the track
             */
            const Timestep Played();

            /**
             * @brief Start playing the music stream from the current position
             *
             */
            void Play();

            /**
             * @brief Stop playing the music stream and rewind it to the beginning
             *
             */
            void Stop();

            /**
             * @brief Pause the music stream, keeping its current position
             *
             */
            void Pause();

            /**
             * @brief Resume a paused music stream from where it was paused
             *
             */
            void Resume();

            /**
             * @brief Move the playback position of the music stream
             *
             * @param position the position to jump to
             */
            void Seek(const Timestep& position);
        private:
            size_t m_id; /*!< the id of the music stream owned by the engine */
        };

        /**
         * @brief Clear all the music streams, the already loaded tracks become invalid
         *
         */
        EXTENDAPI void initMusicStreams();

        /**
         * @brief Load a music file as a stream
         *
         * @param filename the path of the music file to stream
         * @return size_t the id of the new music stream, or an invalid id if the load failed
         */
        EXTENDAPI size_t loadMusicStream(const char* filename);

        /**
         * @brief Update all the music streams, it must be called once per frame
         *
         */
        EXTENDAPI void updateMusicStream();

        /**
         * @brief Unload all the music streams and release their resources
         *
         */
        EXTENDAPI void unloadMusicStreams();

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