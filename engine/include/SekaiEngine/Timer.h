/**
 * @file Timer.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief All the classes related to time
 * @version 0.1
 * @date 2024-07-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_TICKER_H_
#define _SEKAI_ENGINE_TICKER_H_

#include <atomic>
#include <chrono>
#include "SekaiEngine/BaseType.h"

namespace SekaiEngine
{
    /**
     * @brief Time step object
     * 
     */
    class EXTENDAPI Timestep
    {
    public:
        /**
         * @brief Construct a new Timestep object
         * 
         * @param time time step in seconds
         */
        explicit Timestep(const float& time = 0.0f);

        /**
         * @brief Construct a new Timestep object
         * 
         * @param step copied object
         */
        Timestep(const Timestep& step);

        /**
         * @brief Assign the content of one Timestep to other
         * 
         * @param step the object copy from
         * @return Timestep& the reference of the object itself
         */
        Timestep& operator=(const Timestep& step);

        /**
         * @brief Copied assignment operator
         * 
         * @param time the value of timestep in seconds
         * @return Timestep& the reference of the object itself
         */
        Timestep& operator=(const float& time);

        /**
         * @brief Equal comparation operator
         * 
         * @param cmpTimestep compared timestep
         * @return true 2 timesteps is equal
         * @return false 2 timesteps is not equal
         */
        bool operator==(const Timestep& cmpTimestep);

        /**
         * @brief Not equal comparator operator
         * 
         * @param cmpTimestep compared timestep
         * @return true 2 timesteps is not equal
         * @return false 2 timesteps is equal
         */
        bool operator!=(const Timestep& cmpTimestep);

        /**
         * @brief Less than comparation operator
         * 
         * @param cmpTimestep the right-hand side timestep in comparation
         * @return true the timestep is less than cmpTimestep
         * @return false the timestep is not less than cmpTimestep
         */
        bool operator<(const Timestep& cmpTimestep);

        /**
         * @brief Less than or equal comparation operator
         * 
         * @param cmpTimestep the right-hand side timestep in comparation
         * @return true the timestep is less than or equal to cmpTimestep
         * @return false the timestep is not less than or equal to cmpTimestep
         */
        bool operator<=(const Timestep& cmpTimestep);

        /**
         * @brief Greater than comparation operator
         * 
         * @param cmpTimestep the right-hand side timestep in comparation
         * @return true the timestep is greater than cmpTimestep
         * @return false the timestep is not greater than cmpTimestep
         */
        bool operator>(const Timestep& cmpTimestep);

        /**
         * @brief Greater than or equal comparation operator
         * 
         * @param cmpTimestep the right-hand side timestep in comparation
         * @return true the timestep is greater than or equal to cmpTimestep
         * @return false the timestep is not greater than or equal to cmpTimestep
         */
        bool operator>=(const Timestep& cmpTimestep);

        /**
         * @brief Destroy the Timestep object
         * 
         */
        ~Timestep();

        /**
         * @brief Get the seconds of time step
         * 
         * @return float the seconds
         */
        float ToSeconds() const;

        /**
         * @brief Get the seconds of time step
         * 
         * @return float the seconds
         */
        float ToSeconds();

        /**
         * @brief Get the miliseconds of time step
         * 
         * @return float the miliseconds
         */
        float ToMiliseconds() const;

        /**
         * @brief Get the miliseconds of time step
         * 
         * @return float the miliseconds
         */
        float ToMiliseconds();

    private:
        float m_time; /*!< The value of timestep in seconds*/
    };

    /**
     * @brief Game timer
     * 
     */
    class Timer
    {
    public:
        /**
         * @brief Construct a new Timer object
         * 
         */
        EXTENDAPI Timer();

        /**
         * @brief Construct a new Timer object
         * 
         * @param timer copied object
         */
        EXTENDAPI Timer(const Timer& timer);

        /**
         * @brief Copied assignment operator
         * 
         * @param timer copied object
         * @return Timer& the reference of the object itself
         */
        EXTENDAPI Timer& operator=(const Timer& timer);

        /**
         * @brief Destroy the Timer object
         * 
         */
        EXTENDAPI ~Timer();
        
        /**
         * @brief Set the Target FPS object
         * 
         * @param fps target fpg
         */
        EXTENDAPI void SetTargetFPS(const int& fps);

        /**
         * @brief wait until the frame reach to target fps, it is called at the end of a frame
         * 
         */
        EXTENDAPI void wait();

        /**
         * @brief update timer, it is called at the beginning of a frame
         * 
         * @return Timestep the timestep of last frame
         */
        EXTENDAPI Timestep update();

        /**
         * @brief Get the frame rate the loop is currently achieving, in frames per second
         *
         * @return float the frames completed in the last closed measurement window, or
         * 0 before the first window closes
         *
         * @note The value is a rate over a recent interval, not the reciprocal of one
         * frame's duration, so it holds steady while it is read. It is written on the
         * thread that owns the window and may be read from any thread.
         */
        EXTENDAPI float FPS();

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_latestFrameTime; /*!< the application time of the latest frame */
        /* The only member another thread may touch, so it is the only one that has to be
           read as a whole: update() stores it on the window's thread, and a game may read
           it from its own thread. */
        std::atomic<float> m_fps; /*!< the frame rate of the last closed measurement window */
        /* Plain, because update() is reached only from Application::BeginFrame, so only
           the thread that owns the window ever counts. */
        float m_windowSeconds; /*!< the seconds counted so far in the open window */
        int m_windowFrames; /*!< the frames counted so far in the open window */
    };

    inline float Timer::FPS()
    {
        /* Relaxed, because a lone scalar carries no other value that has to be seen
           with it, and the value itself is already whole. */
        return m_fps.load(std::memory_order_relaxed);
    }

    inline float Timestep::ToSeconds() const
    {
        return m_time;
    }

    inline float Timestep::ToSeconds()
    {
        return static_cast<const Timestep&>(*this).ToSeconds();
    }

    inline float Timestep::ToMiliseconds() const
    {
        return m_time * 1000;
    }

    inline float Timestep::ToMiliseconds()
    {
        return static_cast<const Timestep&>(*this).ToMiliseconds();
    }

} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_TICKER_H_