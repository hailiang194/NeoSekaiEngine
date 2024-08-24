#ifndef _SEKAI_ENGINE_AUDIO_DEVICE_H_
#define _SEKAI_ENGINE_AUDIO_DEVICE_H_

#include "SekaiEngine/BaseType.h"

namespace SekaiEngine
{
    namespace Audio
    {
        class EXTENDAPI Device
        {
        public:
            Device();
            ~Device();

            void SetVolume(const float& volume);
            
            const float& Volume() const;
            const float& Volume();
        private:
            float m_volume;
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