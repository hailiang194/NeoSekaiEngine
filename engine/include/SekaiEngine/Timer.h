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
         * @brief Copied assignment operator
         * 
         * @param step copied object
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
        Timer();

        /**
         * @brief Construct a new Timer object
         * 
         * @param timer copied object
         */
        Timer(const Timer& timer);

        /**
         * @brief Copied assignment operator
         * 
         * @param timer copied object
         * @return Timer& the reference of the object itself
         */
        Timer& operator=(const Timer& timer);

        /**
         * @brief Destroy the Timer object
         * 
         */
        ~Timer();
        
        /**
         * @brief Set the Target FPS object
         * 
         * @param fps target fpg
         */
        void SetTargetFPS(const int& fps);

        /**
         * @brief wait until the frame reach to target fps, it is called at the end of a frame
         * 
         */
        void wait();

        /**
         * @brief update timer, it is called at the beginning of a frame
         * 
         * @return Timestep the timestep of last frame
         */
        Timestep update();

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_latestFrameTime; /*!< the application time of the latest frame */
    };

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