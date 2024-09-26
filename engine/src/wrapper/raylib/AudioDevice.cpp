#include "SekaiEngine/Audio/Device.h"

#include "raylib.h"

namespace SekaiEngine
{
    namespace Audio
    {
        Device::Device()
        {
            InitAudioDevice();
            SetMasterVolume(1.0f);
            m_volume = 1.0f;
        }

        Device::~Device()
        {
            CloseAudioDevice();
        }

        void Device::SetVolume(const float& volume)
        {
            SetMasterVolume(volume);
            m_volume = volume;
        }

    } // namespace Audio
    
} // namespace SekaiEngine
