/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#pragma once
#include <iostream>
#include <math.h>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <bullet/LinearMath/btVector3.h>

using namespace std;

namespace Dream
{
    class Math
    {
    public:
        static float degreesToRadians(float degrees)
        {
            return static_cast<float>(degrees*(M_PI/180));
        }

        static float radiansToDegrees(float radians)
        {
            return static_cast<float>(radians*(180/M_PI));
        }
    };

    class Vector2
    {
        float _x;
        float _y;

    public:
        Vector2(float f = 0.0f) : _x(f), _y(f) {}
        Vector2(const float* fp) : _x(fp[0]), _y(fp[1]) {}
        Vector2(float x, float y) : _x(x), _y(y) {}

        glm::vec2 toGLM() const
        {
            return glm::vec2(_x,_y);
        }

        float x() const { return _x; }
        void setX(float x) { _x = x; }

        float y() const { return _y; }
        void setY(float y) { _y = y; }

        bool operator==(const Vector2& other) const
        {
            return _x == other._x &&
                   _y == other._y;
        }

        Vector2 operator+(const Vector2& b) const
        {
            return Vector2(
                _x + b._x,
                _y + b._y
            );
        }

        Vector2 operator-(const Vector2& b) const
        {
            return Vector2(
                _x - b._x,
                _y - b._y
            );
        }

        Vector2 operator*(const Vector2& b) const
        {
            return Vector2(
                _x * b._x,
                _y * b._y
            );
        }

        Vector2 operator/(const Vector2& b) const
        {
            return Vector2(
                _x / b._x,
                _y / b._y
            );
        }

        void operator+=(const Vector2& other)
        {
            _x += other._x;
            _y += other._y;
        }

    };

    class Vector3
    {
        float _x;
        float _y;
        float _z;

    public:
        Vector3(float f = 0.0f) : _x(f), _y(f), _z(f) {}
        Vector3(float x, float y, float z) : _x(x), _y(y), _z(z) {}
        Vector3(const float* fp) : _x(fp[0]), _y(fp[1]), _z(fp[2]) {}
        Vector3(const glm::vec3& v) : _x(v.x), _y(v.y), _z(v.z) {}
        Vector3(const btVector3& v) : _x(v.x()), _y(v.y()), _z(v.z()) {}

        glm::vec3 toGLM() const
        {
            return glm::vec3(_x,_y,_z);
        }

        btVector3 toBullet() const
        {
            return btVector3(_x,_y,_z);
        }

        float x() const { return _x; }
        void setX(float x) { _x = x; }

        float y() const { return _y; }
        void setY(float y) { _y = y; }

        float z() const { return _z; }
        void setZ(float z) { _z = z; }

        bool operator==(const Vector3& other) const
        {
            return _x == other._x &&
                   _y == other._y &&
                   _z == other._z;
        }

        Vector3& operator=(const Vector3& other)
        {
           _x = other._x;
           _y = other._y;
           _z = other._z;
           return *this;
        }

        Vector3 operator+(const Vector3& b) const
        {
            return Vector3(
                _x + b._x,
                _y + b._y,
                _z + b._z
            );
        }

        Vector3 operator-(const Vector3& b) const
        {
            return Vector3(
                _x - b._x,
                _y - b._y,
                _z - b._z
            );
        }

        Vector3 operator*(const Vector3& b) const
        {
            return Vector3(
                _x * b._x,
                _y * b._y,
                _z * b._z
            );
        }

        Vector3 operator/(const Vector3& b) const
        {
            return Vector3(
                _x / b._x,
                _y / b._y,
                _z / b._z
            );
        }

        void operator+=(const Vector3& other)
        {
            _x += other._x;
            _y += other._y;
            _z += other._z;
        }

        static Vector3 normalize(const Vector3& p)
        {
            float w = sqrt( p.x() * p.x() + p.y() * p.y() + p.z() * p.z() );
            return Vector3(p.x()/w,p.y()/w,p.z()/w);
        }

        static Vector3 cross(const Vector3& a, const Vector3& b)
        {

            return Vector3(
                a.y()*b.z() - a.z()*b.y(),
                a.z()*b.x() - a.x()*b.z(),
                a.x()*b.y() - a.y()*b.x()
            );
        }

        static float dot(const Vector3& a, const Vector3& b)
        {
            return  a.x()*b.x()+
                    a.y()*b.y()+
                    a.z()*b.z();
        }
    };

    class Vector4
    {
        float _x;
        float _y;
        float _z;
        float _w;

    public:
        Vector4(float f = 0.0f) : _x(f), _y(f), _z(f), _w(f) {}
        Vector4(const float* fp) : _x(fp[0]), _y(fp[1]), _z(fp[2]), _w(fp[3]) {}
        Vector4(float x, float y, float z, float w)
            : _x(x), _y(y), _z(z), _w(w) {}

        glm::vec4 toGLM() const
        {
            return glm::vec4(_x,_y,_z,_w);
        }

        float x() const { return _x; }
        void setX(float x) { _x = x; }

        float y() const { return _y; }
        void setY(float y) { _y = y; }

        float z() const { return _z; }
        void setZ(float z) { _z = z; }

        float w() const { return _w; }
        void setW(float w) { _w = w; }

        bool operator==(const Vector4 other) const
        {
            return _x == other._x &&
                   _y == other._y &&
                   _z == other._z &&
                   _w == other._w;
        }

        Vector4 operator+(const Vector4& b) const
        {
            return Vector4(
                _x + b._x,
                _y + b._y,
                _z + b._z,
                _w + b._w
            );
        }

        Vector4 operator-(const Vector4& b) const
        {
            return Vector4(
                _x - b._x,
                _y - b._y,
                _z - b._z,
                _w - b._w
            );
        }

        Vector4 operator*(const Vector4& b) const
        {
            return Vector4(
                _x * b._x,
                _y * b._y,
                _z * b._z,
                _w * b._w
            );
        }

        Vector4 operator/(const Vector4& b) const
        {
            return Vector4(
                _x / b._x,
                _y / b._y,
                _z / b._z,
                _w / b._w
            );
        }

        void operator+=(const Vector4& other)
        {
            _x += other._x;
            _y += other._y;
            _z += other._z;
            _w += other._z;
        }

        static Vector4 normalize(const Vector4& p)
        {
            float w = sqrt( p.x() * p.x() + p.y() * p.y() + p.z() * p.z() + p.w()* p.w());
            return Vector4(p.x()/w, p.y()/w, p.z()/w, p.w()/w);
        }

    };
}
