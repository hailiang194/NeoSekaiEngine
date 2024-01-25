#ifndef _SEKAI_ENGINE_MATH_VECTOR_H_
#define _SEKAI_ENGINE_MATH_VECTOR_H_

#include <initializer_list>
#include <cassert>
#include "SekaiEngine/BaseType.h"
#include "SekaiEngine/Math/Vector.h"

namespace SekaiEngine
{
    namespace Math
    {
        template <std::size_t dimensions>
        class CommonVector
        {
        public:
            CommonVector(const std::initializer_list<float>& values);
            CommonVector(const CommonVector& vector);
            CommonVector& operator=(const CommonVector& vector);
            virtual ~CommonVector();
        protected:
            float m_values[dimensions];
        };

        class EXTENDAPI Vector2D: public CommonVector<2>
        {
        public:
            Vector2D(const float& x = 0.0f, const float& y = 0.0f);
            Vector2D(const Vector2D& vt);
            Vector2D& operator=(const Vector2D& vt);
            ~Vector2D();

            const float& X() const;
            float& X();

            const float& Y() const;
            float& Y();
        };

        class EXTENDAPI Vector3D: public CommonVector<3>
        {
        public:
            Vector3D(const float& x = 0.0f, const float& y = 0.0f, const float& z = 0.0f);
            Vector3D(const Vector3D& vt);
            Vector3D& operator=(const Vector3D& vt);
            ~Vector3D();

            const float& X() const;
            float& X();

            const float& Y() const;
            float& Y();

            const float& Z() const;
            float& Z();
        };

        class EXTENDAPI Vector4D: public CommonVector<4>
        {
        public:
            Vector4D(const float& x = 0.0f, const float& y = 0.0f, const float& z = 0.0f, const float& t = 0.0f);
            Vector4D(const Vector4D& vt);
            Vector4D& operator=(const Vector4D& vt);
            ~Vector4D();

            const float& X() const;
            float& X();

            const float& Y() const;
            float& Y();

            const float& Z() const;
            float& Z();

            const float&T() const;
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