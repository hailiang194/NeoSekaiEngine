#include "SekaiEngine/Timer.h"
#include "SekaiEngine/Math/Utility.h"

namespace SekaiEngine
{
    Timestep::Timestep(const float& time)
        :m_time(time)
    {

    }

    Timestep::Timestep(const Timestep& step)
        :m_time(step.m_time)
    {

    }

    Timestep& Timestep::operator=(const Timestep& step)
    {
        m_time = step.m_time;

        return (*this);
    }

    Timestep& Timestep::operator=(const float& time)
    {
        m_time = time;
        return (*this);
    }

#define TIMESTEP_COMPARE_VALUE Math::cmpFloat(m_time, cmpTimestep.m_time)
    
    bool Timestep::operator==(const Timestep& cmpTimestep)
    {
        return TIMESTEP_COMPARE_VALUE == 0; 
    }

    bool Timestep::operator!=(const Timestep& cmpTimestep)
    {
        return TIMESTEP_COMPARE_VALUE != 0;
    }

    bool Timestep::operator<(const Timestep& cmpTimestep)
    {
        return TIMESTEP_COMPARE_VALUE < 0;
    }

    bool Timestep::operator<=(const Timestep& cmpTimestep)
    {
        return TIMESTEP_COMPARE_VALUE <= 0;
    }

    bool Timestep::operator>(const Timestep& cmpTimestep)
    {
        return TIMESTEP_COMPARE_VALUE > 0;
    }

    bool Timestep::operator>=(const Timestep& cmpTimestep)
    {
        return TIMESTEP_COMPARE_VALUE >= 0;
    }

    Timestep::~Timestep()
    {

    }

    Timer::Timer()
        :m_latestFrameTime(std::chrono::high_resolution_clock::now())
    {

    }

    Timer::Timer(const Timer& timer)
        :m_latestFrameTime(timer.m_latestFrameTime)
    {

    }
    
    Timer& Timer::operator=(const Timer& timer)
    {
        m_latestFrameTime = timer.m_latestFrameTime;
        return (*this);
    }


    Timer::~Timer()
    {

    }

    Timestep Timer::update()
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> current = std::chrono::high_resolution_clock::now();
        float elapse = std::chrono::duration_cast<std::chrono::nanoseconds>(current - m_latestFrameTime).count() * 0.001f * 0.001f * 0.001f;
        m_latestFrameTime = current;

        return Timestep(elapse);
    }
} // namespace SekaiEngine
