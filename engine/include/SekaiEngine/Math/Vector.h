/**
 * @file Vector.h
 * @author Luong The Hai (hailuongthe2000@gmail.com)
 * @brief All the class related to dimension vectors
 * @version 0.1
 * @date 2024-07-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _SEKAI_ENGINE_MATH_VECTOR_H_
#define _SEKAI_ENGINE_MATH_VECTOR_H_

#include <initializer_list>
#include <cassert>
#include <cmath>
#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Math/Vector.h"
#include "SekaiEngine/Math/Utility.h"

namespace SekaiEngine
{
    namespace Math
    {
        /**
         * @brief The base class of mathematics' vector
         * 
         * @tparam dimensions the number of dimesions
         */
        template <std::size_t dimensions>
        class CommonVector
        {
        public:
            /**
             * @brief Construct a new Common Vector object
             * 
             * @param values list of the values of vector
             */
            CommonVector(const std::initializer_list<float>& values);

            /**
             * @brief Construct a new Common Vector object
             * 
             * @param vector copied vector
             */
            CommonVector(const CommonVector& vector);

            /**
             * @brief Copied assignment operator
             * 
             * @param vector copied object
             * @return CommonVector& the reference of itself
             */
            CommonVector& operator=(const CommonVector& vector);

            /**
             * @brief Destroy the Common Vector object
             * 
             */
            virtual ~CommonVector();

            /**
             * @brief Add 2 vectors
             * 
             * @param vector added vector
             * @return const CommonVector the result 
             */
            const CommonVector operator+(const CommonVector& vector) const;

            /**
             * @brief Add 2 vectors
             * 
             * @param vector added vector
             * @return const CommonVector the result 
             */
            const CommonVector operator+(const CommonVector& vector);

            /**
             * @brief get the scaled vector
             * 
             * @param scale scaled value
             * @return const CommonVector the result 
             */
            const CommonVector operator*(const float& scale) const;

            /**
             * @brief get the scaled vector
             * 
             * @param scale scaled value
             * @return const CommonVector the result 
             */
            const CommonVector operator*(const float& scale);

            /**
             * @brief Plus the negative direction of vector
             * 
             * @param vector added vector
             * @return const CommonVector the result 
             */
            const CommonVector operator-(const CommonVector& vector) const;

            /**
             * @brief Plus the negative direction of vecto
             * 
             * @param vector added vector
             * @return const CommonVector the result 
             */
            const CommonVector operator-(const CommonVector& vector);

            /**
             * @brief Check if 2 vectors are equal
             * 
             * @param vector compared vector
             * @return true if 2 vectors are equal
             * @return false if 2 vectors are not equal
             */
            const bool operator==(const CommonVector& vector) const;

            /**
             * @brief Check if 2 vectors are equal
             * 
             * @param vector compared vector
             * @return true if 2 vectors are equal
             * @return false if 2 vectors are not equal
             */
            const bool operator==(const CommonVector& vector);
            
            /**
             * @brief Get the dot product of 2 vectors
             * 
             * @param vector vector for dot product
             * @return const float the result
             */
            const float Dot(const CommonVector& vector) const;

            /**
             * @brief Get the dot product of 2 vectors
             * 
             * @param vector vector for dot product
             * @return const float the result
             */
            const float Dot(const CommonVector& vector);

            /**
             * @brief Get the distance from the vector to origin
             * 
             * @return const float the distance
             */
            const float Distance() const;

            /**
             * @brief Get the distance from the vector to origin
             * 
             * @return const float the distance
             */
            const float Distance();

        protected:
            /**
             * @brief Construct a new Common Vector object
             * 
             * @param values raw values
             */
            CommonVector(const float values[dimensions]);
            float m_values[dimensions]; /*!< raw value of vector */
        };

        /**
         * @brief 2-dimension vector
         * 
         */
        class EXTENDAPI Vector2D: public CommonVector<2>
        {
        public:
            /**
             * @brief Construct a new Vector 2D object
             * 
             * @param x the initialized x axis value
             * @param y the initialized y axis value
             */
            Vector2D(const float& x = 0.0f, const float& y = 0.0f);

            /**
             * @brief Construct a new Vector 2D object
             * 
             * @param vt copied vector
             */
            Vector2D(const CommonVector<2>& vt);

            /**
             * @brief Construct a new Vector 2D object
             * 
             * @param vt copied object
             */
            Vector2D(const Vector2D& vt);

            /**
             * @brief Copied assignment operator
             * 
             * @param vt copied object
             * @return Vector2D& the reference of itself
             */
            Vector2D& operator=(const Vector2D& vt);

            /**
             * @brief Copied assignment operator
             * 
             * @param vt copied object
             * @return Vector2D& the reference of itself
             */
            Vector2D& operator=(const CommonVector<2>& vt);

            /**
             * @brief Destroy the Vector 2D object
             * 
             */
            ~Vector2D();

            /**
             * @brief get the value of x axis
             * 
             * @return const float& the value
             */
            const float& X() const;

            /**
             * @brief get the value of x axis
             * 
             * @return const float& the value
             */
            float& X();

            /**
             * @brief get the value of y axis
             * 
             * @return const float& the value
             */
            const float& Y() const;

            /**
             * @brief get the value of y axis
             * 
             * @return const float& the value
             */
            float& Y();
        };

        /**
         * @brief 3-dimenstion vector
         * 
         */
        class EXTENDAPI Vector3D: public CommonVector<3>
        {
        public:
            /**
             * @brief Construct a new Vector 3D object
             * 
             * @param x the initialized value of x axis
             * @param y the initialized value of y axis
             * @param z the initialized value of z axis
             */
            Vector3D(const float& x = 0.0f, const float& y = 0.0f, const float& z = 0.0f);

            /**
             * @brief Construct a new Vector 3D object
             * 
             * @param vt copied object
             */
            Vector3D(const CommonVector<3>& vt);

            /**
             * @brief Construct a new Vector 3D object
             * 
             * @param vt copied object
             */
            Vector3D(const Vector3D& vt);

            /**
             * @brief Copied assignment operator
             * 
             * @param vt copied object
             * @return Vector3D& the reference of itself
             */
            Vector3D& operator=(const Vector3D& vt);

            /**
             * @brief Copied assignment operator
             * 
             * @param vt copied object
             * @return Vector3D& the reference of itself
             */
            Vector3D& operator=(const CommonVector<3>& vt);

            /**
             * @brief Destroy the Vector 3D object
             * 
             */
            ~Vector3D();

            /**
             * @brief Get the value of x axis
             * 
             * @return const float& the value
             */
            const float& X() const;

            /**
             * @brief Get the value of x axis
             * 
             * @return const float& the value
             */
            float& X();

            /**
             * @brief Get the value of y axis
             * 
             * @return const float& the value
             */
            const float& Y() const;

            /**
             * @brief Get the value of y axis
             * 
             * @return const float& the value
             */
            float& Y();
            /**
             * @brief Get the value of z axis
             * 
             * @return const float& the value
             */
            const float& Z() const;

            /**
             * @brief Get the value of z axis
             * 
             * @return const float& the value
             */
            float& Z();
        };

        /**
         * @brief 4-dimension vector
         * 
         */
        class EXTENDAPI Vector4D: public CommonVector<4>
        {
        public:
            /**
             * @brief Construct a new Vector 4D object
             * 
             * @param x the initialized value of x axis
             * @param y the initialized value of y axis
             * @param z the initialized value of z axis
             * @param t the initialized value of t axis
             */
            Vector4D(const float& x = 0.0f, const float& y = 0.0f, const float& z = 0.0f, const float& t = 0.0f);

            /**
             * @brief Construct a new Vector 4D object
             * 
             * @param vt copied object
             */
            Vector4D(const CommonVector<4>& vt);

            /**
             * @brief Construct a new Vector 4D object
             * 
             * @param vt copied object
             */
            Vector4D(const Vector4D& vt);

            /**
             * @brief Copied assignment operator
             * 
             * @param vt copied object
             * @return Vector4D& the reference of itself
             */
            Vector4D& operator=(const Vector4D& vt);

            /**
             * @brief Copied assignment operator
             * 
             * @param vt copied object
             * @return Vector4D& the reference of itself
             */
            Vector4D& operator=(const CommonVector<4>& vt);

            /**
             * @brief Destroy the Vector 4D object
             * 
             */
            ~Vector4D();

            /**
             * @brief Get the value of x axis
             * 
             * @return const float& the value
             */
            const float& X() const;

            /**
             * @brief Get the value of x axis
             * 
             * @return const float& the value
             */
            float& X();

            /**
             * @brief Get the value of x axis
             * 
             * @return const float& the value
             */
            const float& Y() const;

            /**
             * @brief Get the value of x axis
             * 
             * @return const float& the value
             */
            float& Y();

            /**
             * @brief Get the value of x axis
             * 
             * @return const float& the value
             */
            const float& Z() const;

            /**
             * @brief Get the value of x axis
             * 
             * @return const float& the value
             */
            float& Z();

            /**
             * @brief Get the value of x axis
             * 
             * @return const float& the value
             */
            const float& T() const;

            /**
             * @brief Get the value of x axis
             * 
             * @return const float& the value
             */
            float& T();
        };

        template <std::size_t dimensions>
        CommonVector<dimensions>::CommonVector(const std::initializer_list<float>& values)
        {
            assert(values.size() == dimensions);
            float* current = m_values;
            for(auto iter = values.begin(); iter != values.end(); ++iter)
            {
                (*current) = (*iter);
                ++current;
            }

        }
        template <std::size_t dimensions>
        CommonVector<dimensions>::CommonVector(const CommonVector<dimensions>& vector)
        {
            for(std::size_t i = 0; i < dimensions; ++i)
            {
                m_values[i] = vector.m_values[i];
            }
        }

        template <std::size_t dimensions>
        CommonVector<dimensions>& CommonVector<dimensions>::operator=(const CommonVector<dimensions>& vector)
        {
            for(std::size_t i = 0; i < dimensions; ++i)
            {
                m_values[i] = vector.m_values[i];
            }
            return (*this);
        }

        template <std::size_t dimensions>
        CommonVector<dimensions>::~CommonVector()
        {
        }

        template <std::size_t dimensions>
        CommonVector<dimensions>::CommonVector(const float values[dimensions])
        {
            for(std::size_t i = 0; i < dimensions; ++i)
            {
                m_values[i] = values[i];
            }
        }

        template <std::size_t dimensions>
        inline const CommonVector<dimensions> CommonVector<dimensions>::operator+(const CommonVector<dimensions>& vector) const
        {
            
            float result[dimensions] = { 0 };
            for(std::size_t i = 0; i < dimensions; ++i)
            {
                result[i] = m_values[i] + vector.m_values[i];
            }
            
            return CommonVector<dimensions>(result);
        }

        template <std::size_t dimensions>
        inline const CommonVector<dimensions> CommonVector<dimensions>::operator+(const CommonVector<dimensions>& vector)
        {
            return static_cast<const CommonVector<dimensions>&>(*this).operator+(vector);
        }
        
        template <std::size_t dimensions>
        inline const CommonVector<dimensions> CommonVector<dimensions>::operator*(const float& scale) const
        {
            float result[dimensions] = { 0 };
            for(std::size_t i = 0; i < dimensions; ++i)
            {
                result[i] = m_values[i] * scale;
            }
            
            return CommonVector<dimensions>(result);
        }

        template <std::size_t dimensions>
        inline const CommonVector<dimensions> CommonVector<dimensions>::operator*(const float& scale)
        {
            return static_cast<const CommonVector<dimensions>&>(*this).operator*(scale);
        }

        template <std::size_t dimensions>
        inline const CommonVector<dimensions> CommonVector<dimensions>::operator-(const CommonVector<dimensions>& vector) const
        {
            float result[dimensions] = { 0 };
            for(std::size_t i = 0; i < dimensions; ++i)
            {
                result[i] = m_values[i] - vector.m_values[i];
            }
            
            return CommonVector<dimensions>(result);
        }

        template <std::size_t dimensions>
        inline const CommonVector<dimensions> CommonVector<dimensions>::operator-(const CommonVector<dimensions>& vector)
        {
            return static_cast<const CommonVector<dimensions>&>(*this).operator-(vector);
        }

        template <std::size_t dimensions>
        inline const bool CommonVector<dimensions>::operator==(const CommonVector<dimensions>& vector) const
        {
            for(std::size_t i = 0; i < dimensions; ++i)
            {
                if(cmpFloat(m_values[i], vector.m_values[i]) != 0)
                    return false;
            }
            
            return true;
        }

        template <std::size_t dimensions>
        inline const bool CommonVector<dimensions>::operator==(const CommonVector<dimensions>& vector)
        {
            return static_cast<const CommonVector<dimensions>&>(*this).operator==(vector);
        }
        
        template <std::size_t dimensions>
        inline const float CommonVector<dimensions>::Dot(const CommonVector<dimensions>& vector) const
        {
            float result = 0.0f;

            for(std::size_t i = 0; i < dimensions; ++i)
            {
                result += (m_values[i] * vector.m_values[i]);
            }

            return result;
        }

        template <std::size_t dimensions>
        inline const float CommonVector<dimensions>::Dot(const CommonVector<dimensions>& vector)
        {
            return static_cast<const CommonVector<dimensions>&>(*this).Dot(vector);
        }

        template <std::size_t dimensions>
        inline const float CommonVector<dimensions>::Distance() const
        {
            float result = 0;

            for(std::size_t i = 0; i < dimensions; ++i)
            {
                float norm = std::abs(m_values[i]) * 0.0001f;

                result += (norm * norm);
            }

            return static_cast<float>(std::sqrt(result)) / 0.0001f;
        }

        template <std::size_t dimensions>
        inline const float CommonVector<dimensions>::Distance()
        {
            return static_cast<const CommonVector<dimensions>&>(*this).Distance();
        }

        inline const float& Vector2D::X() const
        {
            return m_values[0];
        }

        inline float& Vector2D::X()
        {
            return const_cast<float&>(static_cast<const Vector2D&>(*this).X());
        }

        inline const float& Vector2D::Y() const
        {
            return m_values[1];
        }

        inline float& Vector2D::Y()
        {
            return const_cast<float&>(static_cast<const Vector2D&>(*this).Y());
        }

        inline const float& Vector3D::X() const
        {
            return m_values[0];
        }

        inline float& Vector3D::X()
        {
            return const_cast<float&>(static_cast<const Vector3D&>(*this).X());
        }

        inline const float& Vector3D::Y() const
        {
            return m_values[1];
        }

        inline float& Vector3D::Y()
        {
            return const_cast<float&>(static_cast<const Vector3D&>(*this).Y());
        }

        inline const float& Vector3D::Z() const
        {
            return m_values[2];
        }

        inline float& Vector3D::Z()
        {
            return const_cast<float&>(static_cast<const Vector3D&>(*this).Z());
        }

        inline const float& Vector4D::X() const
        {
            return m_values[0];
        }

        inline float& Vector4D::X()
        {
            return const_cast<float&>(static_cast<const Vector4D&>(*this).X());
        }

        inline const float& Vector4D::Y() const
        {
            return m_values[1];
        }

        inline float& Vector4D::Y()
        {
            return const_cast<float&>(static_cast<const Vector4D&>(*this).Y());
        }

        inline const float& Vector4D::Z() const
        {
            return m_values[2];
        }

        inline float& Vector4D::Z()
        {
            return const_cast<float&>(static_cast<const Vector4D&>(*this).Z());
        }

        inline const float& Vector4D::T() const
        {
            return m_values[3];
        }

        inline float& Vector4D::T()
        {
            return const_cast<float&>(static_cast<const Vector4D&>(*this).T());
        }
    } // namespace Math
    
} // namespace SekaiEngine


#endif//!_SEKAI_ENGINE_MATH_VECTOR_H_