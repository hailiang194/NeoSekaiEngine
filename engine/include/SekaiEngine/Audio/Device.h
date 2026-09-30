/**
 * @file Device.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief Audio device: owns the audio context and the master volume
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_AUDIO_DEVICE_H_
#define _SEKAI_ENGINE_AUDIO_DEVICE_H_

#include "SekaiEngine/BaseType.h"

namespace SekaiEngine
{
    namespace Audio
    {
        /**
         * @brief Audio device: owns the audio context and the master volume
         *
         * Constructing the device opens the audio context and sets the master
         * volume to 1; destroying it closes the context, so a Sound or a
         * MusicStream cannot outlive it.
         */
        class EXTENDAPI Device
        {
        public:
            /**
             * @brief Construct a new Device object and open the audio context
             *
             */
            Device();

            /**
             * @brief Destroy the Device object and close the audio context
             *
             */
            ~Device();

            /**
             * @brief Set the master volume of every sound and music stream
             *
             * @param volume the new master volume
             */
            void SetVolume(const float& volume);

            /**
             * @brief Get the master volume
             *
             * @return const float& the master volume
             */
            const float& Volume() const;

            /**
             * @brief Get the master volume
             *
             * @return const float& the master volume
             */
            const float& Volume();
        private:
            float m_volume; /*!< the master volume applied to every sound */
        };

        inline const float& Device::Volume() const
        {
            return m_volume;
        }

        inline const float& Device::Volume()
        {
            return static_cast<const Device&>(*this).Volume();
        }
    } // namespace Audio
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_AUDIO_DEVICE_H_