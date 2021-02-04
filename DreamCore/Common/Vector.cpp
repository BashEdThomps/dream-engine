#include "Vector.h"

#include "Common/Constants.h"

#define X_INDEX 0
#define Y_INDEX 1
#define Z_INDEX 2
#define W_INDEX 3

namespace octronic::dream
{
    // Vector2 =================================================================

    Vector2::Vector2(float f)
        : mData{f,f}
    {}

    Vector2::Vector2(const float* fp)
        : mData{fp[X_INDEX],fp[Y_INDEX]}
    {
    }

    Vector2::Vector2(float x, float y)
        : mData{x,y}
    {

    }

    Vector2::Vector2(const json& js)
        : mData{js[Constants::X], js[Constants::Y]}
    {

    }

    // Accessors

    float Vector2::x() const
    {
        return mData[X_INDEX];
    }

    void Vector2::setX(float x)
    {
        mData[X_INDEX] = x;
    }

    float Vector2::y() const
    {
        return mData[Y_INDEX];
    }

    void Vector2::setY(float y)
    {
        mData[Y_INDEX] = y;
    }

    // Arithmetic

    bool Vector2::operator==(const Vector2& other) const
    {
        return mData[X_INDEX] == other.mData[X_INDEX] && mData[Y_INDEX] == mData[Y_INDEX];
    }

    Vector2 Vector2::operator+(const Vector2& b) const
    {
        return Vector2(mData[X_INDEX] + mData[X_INDEX],mData[Y_INDEX] + mData[Y_INDEX]);
    }

    Vector2 Vector2::operator-(const Vector2& b) const
    {
        return Vector2(mData[X_INDEX] - b.mData[X_INDEX],mData[Y_INDEX] - b.mData[Y_INDEX]);
    }

    Vector2 Vector2::operator*(const Vector2& b) const
    {
        return Vector2(mData[X_INDEX] * b.mData[X_INDEX],mData[Y_INDEX] * b.mData[Y_INDEX]);
    }

    Vector2 Vector2::operator/(const Vector2& b) const
    {
        return Vector2(mData[X_INDEX] / b.mData[X_INDEX],mData[Y_INDEX] / b.mData[Y_INDEX]);
    }

    void Vector2::operator+=(const Vector2& other)
    {
        mData[X_INDEX] += other.mData[X_INDEX];
        mData[Y_INDEX] += other.mData[Y_INDEX];
    }

    void Vector2::operator-=(const Vector2& other)
    {
        mData[X_INDEX] -= other.mData[X_INDEX];
        mData[Y_INDEX] -= other.mData[Y_INDEX];
    }

    void Vector2::operator*=(const Vector2& other)
    {
        mData[X_INDEX] *= other.mData[X_INDEX];
        mData[Y_INDEX] *= other.mData[Y_INDEX];
    }

    void Vector2::operator/=(const Vector2& other)
    {
        mData[X_INDEX] /= other.mData[X_INDEX];
        mData[Y_INDEX] /= other.mData[Y_INDEX];
    }

    // Type Conversion

    vec2 Vector2::toGLM() const
    {
        return vec2(mData[X_INDEX],mData[Y_INDEX]);
    }

    json Vector2::toJson() const
    {
        json js = json::object();
        js[Constants::X] = mData[X_INDEX];
        js[Constants::Y] = mData[Y_INDEX];
        return js;
    }

    // Vector3 =================================================================


    Vector3::Vector3(float f)
        : mData{f,f,f}
    {

    }

    Vector3::Vector3(float x, float y, float z)
        : mData{x,y,z}
    {

    }

    Vector3::Vector3(const float* fp)
        : mData{fp[X_INDEX],fp[Y_INDEX],fp[Z_INDEX]}
    {

    }

    Vector3::Vector3(const glm::vec3& v)
        : mData{v.x, v.y, v.z}
    {

    }

    Vector3::Vector3(const btVector3& v)
        : mData{v.x(), v.y(), v.z()}
    {

    }

    Vector3::Vector3(const json& js)
    {
        bool x_found = js.find(Constants::X)   != js.end();
        bool r_found = js.find(Constants::RED) != js.end();
        if      (x_found) mData[X_INDEX] = js[Constants::X];
        else if (r_found) mData[X_INDEX] = js[Constants::RED];
        else              mData[X_INDEX] = 0.f;

        bool y_found = js.find(Constants::Y)     != js.end();
        bool g_found = js.find(Constants::GREEN) != js.end();
        if      (y_found) mData[Y_INDEX] = js[Constants::Y];
        else if (g_found) mData[Y_INDEX] = js[Constants::GREEN];
        else              mData[Y_INDEX] = 0.f;

        bool z_found = js.find(Constants::Z)    != js.end();
        bool b_found = js.find(Constants::BLUE) != js.end();
        if      (x_found) mData[Z_INDEX] = js[Constants::Z];
        else if (r_found) mData[Z_INDEX] = js[Constants::BLUE];
        else              mData[Z_INDEX] = 0.f;
    }

    // Accessors

    float Vector3::x() const
    {
        return mData[X_INDEX];
    }

    void Vector3::setX(float x)
    {
        mData[X_INDEX] = x;
    }

    float Vector3::r() const
    {
        return mData[X_INDEX];
    }

    void Vector3::setR(float r)
    {
        mData[X_INDEX] = r;
    }

    float Vector3::y() const
    {
        return mData[Y_INDEX];
    }

    void Vector3::setY(float y)
    {
        mData[Y_INDEX] = y;
    }

    float Vector3::g() const
    {
        return mData[Y_INDEX];
    }

    void Vector3::setG(float g)
    {
        mData[Y_INDEX] = g;
    }

    float Vector3::z() const
    {
        return mData[Z_INDEX];
    }

    void Vector3::setZ(float z)
    {
        mData[Z_INDEX] = z;
    }

    float Vector3::b() const
    {
        return mData[Z_INDEX];
    }

    void Vector3::setB(float b)
    {
        mData[Z_INDEX] = b;
    }

    // Type Conversion

    vec3 Vector3::toGLM() const
    {
        return vec3(mData[X_INDEX],mData[Y_INDEX],mData[Z_INDEX]);
    }

    btVector3 Vector3::toBullet() const
    {
        return btVector3(mData[X_INDEX],mData[Y_INDEX],mData[Z_INDEX]);
    }

    json Vector3::toJson() const
    {
        json js = json::object();
        js[Constants::X] = mData[X_INDEX];
        js[Constants::Y] = mData[Y_INDEX];
        js[Constants::Z] = mData[Z_INDEX];
        return js;
    }

    // Arithmetic

    bool Vector3::operator==(const Vector3& other) const
    {
        return mData[X_INDEX] == other.mData[X_INDEX] &&
                mData[Y_INDEX] == other.mData[Y_INDEX] &&
                mData[Z_INDEX] == other.mData[Z_INDEX];
    }

    Vector3& Vector3::operator=(const Vector3& other)
    {
        mData[X_INDEX] = other.mData[X_INDEX];
        mData[Y_INDEX] = other.mData[Y_INDEX];
        mData[Z_INDEX] = other.mData[Z_INDEX];
        return *this;
    }

    Vector3 Vector3::operator+(const Vector3& b) const
    {
        return Vector3(
                    mData[X_INDEX] + b.mData[X_INDEX],
                    mData[Y_INDEX] + b.mData[Y_INDEX],
                    mData[Z_INDEX] + b.mData[Z_INDEX]
                    );
    }

    Vector3 Vector3::operator-(const Vector3& b) const
    {
        return Vector3(
                    mData[X_INDEX] - b.mData[X_INDEX],
                    mData[Y_INDEX] - b.mData[Y_INDEX],
                    mData[Z_INDEX] - b.mData[Z_INDEX]
                    );
    }

    Vector3 Vector3::operator*(const Vector3& b) const
    {
        return Vector3(
                    mData[X_INDEX] * b.mData[X_INDEX],
                    mData[Y_INDEX] * b.mData[Y_INDEX],
                    mData[Z_INDEX] * b.mData[Z_INDEX]
                    );
    }

    Vector3 Vector3::operator/(const Vector3& b) const
    {
        return Vector3(
                    mData[X_INDEX] / b.mData[X_INDEX],
                    mData[Y_INDEX] / b.mData[Y_INDEX],
                    mData[Z_INDEX] / b.mData[Z_INDEX]
                    );
    }

    void Vector3::operator+=(const Vector3& other)
    {
        mData[X_INDEX] += other.mData[X_INDEX];
        mData[Y_INDEX] += other.mData[Y_INDEX];
        mData[Z_INDEX] += other.mData[Z_INDEX];
    }

    void Vector3::operator-=(const Vector3& other)
    {
        mData[X_INDEX] -= other.mData[X_INDEX];
        mData[Y_INDEX] -= other.mData[Y_INDEX];
        mData[Z_INDEX] -= other.mData[Z_INDEX];
    }

    void Vector3::operator*=(const Vector3& other)
    {
        mData[X_INDEX] *= other.mData[X_INDEX];
        mData[Y_INDEX] *= other.mData[Y_INDEX];
        mData[Z_INDEX] *= other.mData[Z_INDEX];
    }

    void Vector3::operator/=(const Vector3& other)
    {
        mData[X_INDEX] /= other.mData[X_INDEX];
        mData[Y_INDEX] /= other.mData[Y_INDEX];
        mData[Z_INDEX] /= other.mData[Z_INDEX];
    }

    // Statics

    Vector3 Vector3::normalize(const Vector3& p)
    {
        float w = sqrt( p.x() * p.x() + p.y() * p.y() + p.z() * p.z() );
        return Vector3(p.x()/w,p.y()/w,p.z()/w);
    }

    Vector3 Vector3::cross(const Vector3& a, const Vector3& b)
    {

        return Vector3(
                    a.y()*b.z() - a.z()*b.y(),
                    a.z()*b.x() - a.x()*b.z(),
                    a.x()*b.y() - a.y()*b.x()
                    );
    }

    float Vector3::dot(const Vector3& a, const Vector3& b)
    {
        return  a.x()*b.x()+
                a.y()*b.y()+
                a.z()*b.z();
    }


    // Vector4 =================================================================

    Vector4::Vector4(float f)
        : mData{f,f,f,f}
    {

    }

    Vector4::Vector4(const float* fp)
        : mData{fp[X_INDEX],fp[Y_INDEX],fp[Z_INDEX],fp[W_INDEX]}
    {

    }

    Vector4::Vector4(float x, float y, float z, float w)
        : mData{x,y,z,w}
    {

    }

    Vector4::Vector4(const json& js)
    {
        bool x_found = js.find(Constants::X)   != js.end();
        bool r_found = js.find(Constants::RED) != js.end();
        if      (x_found) mData[X_INDEX] = js[Constants::X];
        else if (r_found) mData[X_INDEX] = js[Constants::RED];
        else              mData[X_INDEX] = 0.f;

        bool y_found = js.find(Constants::Y)     != js.end();
        bool g_found = js.find(Constants::GREEN) != js.end();
        if      (y_found) mData[Y_INDEX] = js[Constants::Y];
        else if (g_found) mData[Y_INDEX] = js[Constants::GREEN];
        else              mData[Y_INDEX] = 0.f;

        bool z_found = js.find(Constants::Z)    != js.end();
        bool b_found = js.find(Constants::BLUE) != js.end();
        if      (x_found) mData[Z_INDEX] = js[Constants::Z];
        else if (r_found) mData[Z_INDEX] = js[Constants::BLUE];
        else              mData[Z_INDEX] = 0.f;

        bool w_found = js.find(Constants::W)     != js.end();
        bool a_found = js.find(Constants::ALPHA) != js.end();
        if      (x_found) mData[Z_INDEX] = js[Constants::W];
        else if (r_found) mData[Z_INDEX] = js[Constants::ALPHA];
        else              mData[Z_INDEX] = 0.f;

    }

    // Accessors

    float Vector4::x() const { return mData[X_INDEX]; }
    void Vector4::setX(float x) { mData[X_INDEX] = x; }

    float Vector4::r() const { return mData[X_INDEX]; }
    void Vector4::setR(float r) { mData[X_INDEX] = r; }


    float Vector4::y() const { return mData[Y_INDEX]; }
    void Vector4::setY(float y) { mData[Y_INDEX] = y; }

    float Vector4::g() const { return mData[Y_INDEX]; }
    void Vector4::setG(float g) { mData[Y_INDEX] = g; }


    float Vector4::z() const { return mData[Z_INDEX]; }
    void Vector4::setZ(float z) { mData[Z_INDEX] = z; }

    float Vector4::b() const { return mData[Z_INDEX]; }
    void Vector4::setB(float b) { mData[Z_INDEX] = b; }


    float Vector4::w() const { return mData[W_INDEX]; }
    void Vector4::setW(float w) { mData[W_INDEX] = w; }

    float Vector4::a() const { return mData[W_INDEX]; }
    void Vector4::setA(float a) { mData[W_INDEX] = a; }

    // Type Conversion

    json Vector4::toJson() const
    {
        json js = json::object();
        js[Constants::W] = mData[W_INDEX];
        js[Constants::X] = mData[X_INDEX];
        js[Constants::Y] = mData[Y_INDEX];
        js[Constants::Z] = mData[Z_INDEX];
        return js;
    }

    vec4 Vector4::toGLM() const
    {
        return vec4(mData[X_INDEX],mData[Y_INDEX],mData[Z_INDEX],mData[W_INDEX]);
    }

    // Arithmetic

    bool Vector4::operator==(const Vector4 other) const
    {
        return mData[X_INDEX] == other.mData[X_INDEX] &&
                mData[Y_INDEX] == other.mData[Y_INDEX] &&
                mData[Z_INDEX] == other.mData[Z_INDEX] &&
                mData[W_INDEX] == other.mData[W_INDEX];
    }

    Vector4 Vector4::operator+(const Vector4& b) const
    {
        return Vector4(
                    mData[X_INDEX] + b.mData[X_INDEX],
                    mData[Y_INDEX] + b.mData[Y_INDEX],
                    mData[Z_INDEX] + b.mData[Z_INDEX],
                    mData[W_INDEX] + b.mData[W_INDEX]
                    );
    }

    Vector4 Vector4::operator-(const Vector4& b) const
    {
        return Vector4(
                    mData[X_INDEX] - b.mData[X_INDEX],
                    mData[Y_INDEX] - b.mData[Y_INDEX],
                    mData[Z_INDEX] - b.mData[Z_INDEX],
                    mData[W_INDEX] - b.mData[W_INDEX]
                    );
    }

    Vector4 Vector4::operator*(const Vector4& b) const
    {
        return Vector4(
                    mData[X_INDEX] * b.mData[X_INDEX],
                    mData[Y_INDEX] * b.mData[Y_INDEX],
                    mData[Z_INDEX] * b.mData[Z_INDEX],
                    mData[W_INDEX] * b.mData[W_INDEX]
                    );
    }

    Vector4 Vector4::operator/(const Vector4& b) const
    {
        return Vector4(
                    mData[X_INDEX] / b.mData[X_INDEX],
                    mData[Y_INDEX] / b.mData[Y_INDEX],
                    mData[Z_INDEX] / b.mData[Z_INDEX],
                    mData[W_INDEX] / b.mData[W_INDEX]
                    );
    }

    void Vector4::operator+=(const Vector4& other)
    {
        mData[X_INDEX] += other.mData[X_INDEX];
        mData[Y_INDEX] += other.mData[Y_INDEX];
        mData[Z_INDEX] += other.mData[Z_INDEX];
        mData[W_INDEX] += other.mData[Z_INDEX];
    }

    void Vector4::operator-=(const Vector4& other)
    {
        mData[X_INDEX] -= other.mData[X_INDEX];
        mData[Y_INDEX] -= other.mData[Y_INDEX];
        mData[Z_INDEX] -= other.mData[Z_INDEX];
        mData[W_INDEX] -= other.mData[Z_INDEX];
    }

    void Vector4::operator*=(const Vector4& other)
    {
        mData[X_INDEX] *= other.mData[X_INDEX];
        mData[Y_INDEX] *= other.mData[Y_INDEX];
        mData[Z_INDEX] *= other.mData[Z_INDEX];
        mData[W_INDEX] *= other.mData[Z_INDEX];
    }

    void Vector4::operator/=(const Vector4& other)
    {
        mData[X_INDEX] /= other.mData[X_INDEX];
        mData[Y_INDEX] /= other.mData[Y_INDEX];
        mData[Z_INDEX] /= other.mData[Z_INDEX];
        mData[W_INDEX] /= other.mData[Z_INDEX];
    }

    // Static

    Vector4 Vector4::normalize(const Vector4& p)
    {
        float w = sqrt( p.x() * p.x() + p.y() * p.y() + p.z() * p.z() + p.w()* p.w());
        return Vector4(p.x()/w, p.y()/w, p.z()/w, p.w()/w);
    }

}
