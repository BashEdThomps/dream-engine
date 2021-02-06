#pragma once

#include <LinearMath/btVector3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <json.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using nlohmann::json;

namespace octronic::dream
{
	class Vector2 final
    {
        float mData[2];

    public:
		Vector2(float f = 0.0f);
        Vector2(const float* fp);
        Vector2(float x, float y);
        Vector2(const json& js);

        vec2 toGLM() const;

        float x() const;
        void setX(float x);

        float y() const;
        void setY(float y);

        bool operator==(const Vector2& other) const;
        Vector2 operator+(const Vector2& b) const;
        Vector2 operator-(const Vector2& b) const;
        Vector2 operator*(const Vector2& b) const;
        Vector2 operator/(const Vector2& b) const;
        void operator+=(const Vector2& other);
        void operator-=(const Vector2& other);
        void operator*=(const Vector2& other);
        void operator/=(const Vector2& other);
        json toJson() const;

    };

    class Vector3 final
    {
        float mData[3];

    public:
        Vector3(float f = 0.0f);
        Vector3(float x, float y, float z);
        Vector3(const float* fp);
        Vector3(const glm::vec3& v);
        Vector3(const btVector3& v);
        Vector3(const json& js);


        float x() const;
        void setX(float x);

        float r() const;
        void setR(float r);

        float y() const;
        void setY(float y);

        float g() const;
        void setG(float g);


        float z() const;
        void setZ(float z);
        float b() const;
        void setB(float b);

        vec3 toGLM() const;
        json toJson() const;
        btVector3 toBullet() const;

        bool operator==(const Vector3& other) const;
        Vector3& operator=(const Vector3& other);
        Vector3 operator+(const Vector3& b) const;
        Vector3 operator-(const Vector3& b) const;
        Vector3 operator*(const Vector3& b) const;
        Vector3 operator/(const Vector3& b) const;
        void operator+=(const Vector3& other);
        void operator-=(const Vector3& other);
        void operator*=(const Vector3& other);
        void operator/=(const Vector3& other);

        static Vector3 normalize(const Vector3& p);
        static Vector3 cross(const Vector3& a, const Vector3& b);
        static float dot(const Vector3& a, const Vector3& b);
    };

    class Vector4 final
    {
        float mData[4];

    public:
        Vector4(float f = 0.0f);
        Vector4(const float* fp);
        Vector4(float x, float y, float z, float w);
        Vector4(const json& js);


        float x() const;
        void setX(float x);

        float r() const;
        void setR(float r);

        float y() const;
        void setY(float y);

        float g() const;
        void setG(float g);

        float z() const;
        void setZ(float z);

        float b() const;
        void setB(float b);

        float w() const;
        void setW(float w);

        float a() const;
        void setA(float a);

        vec4 toGLM() const;
        json toJson() const;

        bool operator==(const Vector4 other) const;
        Vector4 operator+(const Vector4& b) const;
        Vector4 operator-(const Vector4& b) const;
        Vector4 operator*(const Vector4& b) const;
        Vector4 operator/(const Vector4& b) const;
        void operator+=(const Vector4& other);
        void operator-=(const Vector4& other);
        void operator*=(const Vector4& other);
        void operator/=(const Vector4& other);

        static Vector4 normalize(const Vector4& p);
    };
}
