/**
 * @file Sound.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief One-shot sound effect
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_AUDIO_SOUND_H_
#define _SEKAI_ENGINE_AUDIO_SOUND_H_

#include "SekaiEngine/BaseType.h"
#include <cstddef>

namespace SekaiEngine
{
    namespace Sound
    {
        /**
         * @brief One-shot sound effect
         *
         * The object itself only holds the id of a sample owned by the engine, so
         * copying one is cheap and never duplicates the underlying audio data.
         */
        class EXTENDAPI Sound
        {
        public:
            /**
             * @brief Construct a new Sound object from a file
             *
             * @param filename the path of the sound file to load
             * @note the resulting sound is invalid if the file cannot be loaded
             */
            Sound(const char* filename);

            /**
             * @brief Construct a new Sound object from an existing sound id
             *
             * @param id the id of a sound already loaded by loadSound
             */
            Sound(const size_t& id);

            /**
             * @brief Construct a new Sound object
             *
             * @param sound copied object, the copy shares the same underlying sample
             */
            Sound(const Sound& sound);

            /**
             * @brief Copied assignment operator
             *
             * @param sound the object copy from
             * @return Sound& the reference of the object itself
             */
            Sound& operator=(const Sound& sound);

            /**
             * @brief Destroy the Sound object
             *
             * @note the sample itself stays loaded, call unloadSounds to release it
             */
            ~Sound();

            /**
             * @brief Check if the sound refers to a loaded sample
             *
             * @return true the sound refers to a loaded sample
             * @return false the sound is invalid
             */
            const bool IsValid() const;

            /**
             * @brief Check if the sound refers to a loaded sample
             *
             * @return true the sound refers to a loaded sample
             * @return false the sound is invalid
             */
            const bool IsValid();

            /**
             * @brief Get the id of the sound
             *
             * @return const size_t& the id of the sound
             */
            const size_t& Id() const;

            /**
             * @brief Get the id of the sound
             *
             * @return const size_t& the id of the sound
             */
            const size_t& Id();

            /**
             * @brief Start playing the sound
             *
             */
            void Play();

            /**
             * @brief Stop playing the sound
             *
             */
            void Stop();

            /**
             * @brief Pause the sound
             *
             */
            void Pause();

            /**
             * @brief Resume a paused sound
             *
             */
            void Resume();

            /**
             * @brief Check if the sound is currently playing
             *
             * @return true the sound is playing
             * @return false the sound is not playing
             */
            const bool IsPlaying() const;

            /**
             * @brief Check if the sound is currently playing
             *
             * @return true the sound is playing
             * @return false the sound is not playing
             */
            const bool IsPlaying();
        private:
            size_t m_id; /*!< the id of the sound owned by the engine */
        };

        /**
         * @brief Clear all the sounds, the already loaded samples become invalid
         *
         */
        EXTENDAPI void initSounds();

        /**
         * @brief Load a sound file
         *
         * @param filename the path of the sound file to load
         * @return size_t the id of the new sound, or an invalid id if the load failed
         */
        EXTENDAPI size_t loadSound(const char* filename);

        /**
         * @brief Unload all the sounds and release their resources
         *
         */
        EXTENDAPI void unloadSounds();

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