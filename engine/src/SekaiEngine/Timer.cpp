#include "SekaiEngine/Timer.h"
#include "SekaiEngine/Math/Utility.h"

namespace SekaiEngine
{
    /*How long a measurement window stays open. Long enough that one slow frame does not
      move the reported rate, short enough that a change is noticed while watching it.*/
    static const float FPS_WINDOW_SECONDS = 0.5f;

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
        :m_latestFrameTime(std::chrono::high_resolution_clock::now()), m_fps(0.0f),
         m_windowSeconds(0.0f), m_windowFrames(0)
    {

    }

    Timer::Timer(const Timer& timer)
        :m_latestFrameTime(timer.m_latestFrameTime),
         m_fps(timer.m_fps.load(std::memory_order_relaxed)),
         m_windowSeconds(timer.m_windowSeconds), m_windowFrames(timer.m_windowFrames)
    {

    }
    
    Timer& Timer::operator=(const Timer& timer)
    {
        m_latestFrameTime = timer.m_latestFrameTime;
        m_fps.store(timer.m_fps.load(std::memory_order_relaxed), std::memory_order_relaxed);
        m_windowSeconds = timer.m_windowSeconds;
        m_windowFrames = timer.m_windowFrames;
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

        /*The frame is counted here rather than at a call site of its own, because this is
          already reached once per frame, on the window's thread, on every platform.*/
        ++m_windowFrames;
        m_windowSeconds += elapse;
        if(m_windowSeconds >= FPS_WINDOW_SECONDS)
        {
            m_fps.store(m_windowFrames / m_windowSeconds, std::memory_order_relaxed);
            m_windowSeconds = 0.0f;
            m_windowFrames = 0;
        }

        return Timestep(elapse);
    }
} // namespace SekaiEngine
