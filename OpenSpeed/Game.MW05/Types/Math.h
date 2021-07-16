// clang-format off
//
//    Copyright (C) 2020 Berkay Yigit <berkaytgy@gmail.com>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program. If not, see <https://www.gnu.org/licenses/>.
//
// clang-format on

#pragma once
#include <OpenSpeed/Game.MW05/Types.h>

namespace OpenSpeed::MW05::Math {
  struct Vector2 {
    float x, y;

    Vector2 operator+(const Vector2& rhs) {
      Vector2 _result = *this;
      _result.x += rhs.x;
      _result.y += rhs.y;

      return _result;
    }
    Vector2 operator-(const Vector2& rhs) {
      Vector2 _result = *this;
      _result.x -= rhs.x;
      _result.y -= rhs.y;

      return _result;
    }
    Vector2 operator*(const Vector2& rhs) {
      Vector2 _result = *this;
      _result.x *= rhs.x;
      _result.y *= rhs.y;

      return _result;
    }
    Vector2 operator/(const Vector2& rhs) {
      Vector2 _result = *this;
      _result.x /= rhs.x;
      _result.y /= rhs.y;

      return _result;
    }
    Vector2 operator*(float rhs) {
      Vector2 _result = *this;
      _result.x *= rhs;
      _result.y *= rhs;

      return _result;
    }
    Vector2 operator/(float rhs) {
      Vector2 _result = *this;
      _result.x /= rhs;
      _result.y /= rhs;

      return _result;
    }

    void operator=(const Vector2& rhs) {
      this->x = rhs.x;
      this->y = rhs.y;
    }
    void operator+=(const Vector2& rhs) { *this = *this + rhs; }
    void operator-=(const Vector2& rhs) { *this = *this - rhs; }
    void operator*=(const Vector2& rhs) { *this = *this * rhs; }
    void operator/=(const Vector2& rhs) { *this = *this / rhs; }
    void operator*=(float rhs) { *this = *this * rhs; }
    void operator/=(float rhs) { *this = *this / rhs; }

                 operator float*() { return reinterpret_cast<float*>(this); }
                 operator const float*() { return reinterpret_cast<const float*>(this); }
    float&       operator[](size_t index) { return (reinterpret_cast<float*>(this))[index]; }
    const float& operator[](size_t index) const { return (reinterpret_cast<const float*>(this))[index]; }

    Vector2()  = default;
    ~Vector2() = default;
    Vector2(float x, float y) : x(x), y(y) {}
  };
  struct Vector3 {
    float x, y, z;

    Vector3 operator+(const Vector3& rhs) {
      Vector3 _result = *this;
      _result.x += rhs.x;
      _result.y += rhs.y;
      _result.z += rhs.z;

      return _result;
    }
    Vector3 operator-(const Vector3& rhs) {
      Vector3 _result = *this;
      _result.x -= rhs.x;
      _result.y -= rhs.y;
      _result.z -= rhs.z;

      return _result;
    }
    Vector3 operator*(const Vector3& rhs) {
      Vector3 _result = *this;
      _result.x *= rhs.x;
      _result.y *= rhs.y;
      _result.z *= rhs.z;

      return _result;
    }
    Vector3 operator/(const Vector3& rhs) {
      Vector3 _result = *this;
      _result.x /= rhs.x;
      _result.y /= rhs.y;
      _result.z /= rhs.z;

      return _result;
    }
    Vector3 operator*(float rhs) {
      Vector3 _result = *this;
      _result.x *= rhs;
      _result.y *= rhs;
      _result.z *= rhs;

      return _result;
    }
    Vector3 operator/(float rhs) {
      Vector3 _result = *this;
      _result.x /= rhs;
      _result.y /= rhs;
      _result.z /= rhs;

      return _result;
    }

    void operator=(const Vector3& rhs) {
      this->x = rhs.x;
      this->y = rhs.y;
      this->z = rhs.z;
    }
    void operator+=(const Vector3& rhs) { *this = *this + rhs; }
    void operator-=(const Vector3& rhs) { *this = *this - rhs; }
    void operator*=(const Vector3& rhs) { *this = *this * rhs; }
    void operator/=(const Vector3& rhs) { *this = *this / rhs; }
    void operator*=(float rhs) { *this = *this * rhs; }
    void operator/=(float rhs) { *this = *this / rhs; }

                 operator float*() { return reinterpret_cast<float*>(this); }
                 operator const float*() { return reinterpret_cast<const float*>(this); }
    float&       operator[](size_t index) { return (reinterpret_cast<float*>(this))[index]; }
    const float& operator[](size_t index) const { return (reinterpret_cast<const float*>(this))[index]; }

    Vector3()  = default;
    ~Vector3() = default;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
  };
  struct Vector4 {
    float x, y, z, w;

    Vector4 operator+(const Vector4& rhs) {
      Vector4 _result = *this;
      _result.x += rhs.x;
      _result.y += rhs.y;
      _result.z += rhs.z;
      _result.w += rhs.w;

      return _result;
    }
    Vector4 operator-(const Vector4& rhs) {
      Vector4 _result = *this;
      _result.x -= rhs.x;
      _result.y -= rhs.y;
      _result.z -= rhs.z;
      _result.w -= rhs.w;

      return _result;
    }
    Vector4 operator*(const Vector4& rhs) {
      Vector4 _result = *this;
      _result.x *= rhs.x;
      _result.y *= rhs.y;
      _result.z *= rhs.z;
      _result.w *= rhs.w;

      return _result;
    }
    Vector4 operator/(const Vector4& rhs) {
      Vector4 _result = *this;
      _result.x /= rhs.x;
      _result.y /= rhs.y;
      _result.z /= rhs.z;
      _result.w /= rhs.w;

      return _result;
    }
    Vector4 operator*(float rhs) {
      Vector4 _result = *this;
      _result.x *= rhs;
      _result.y *= rhs;
      _result.z *= rhs;
      _result.w *= rhs;

      return _result;
    }
    Vector4 operator/(float rhs) {
      Vector4 _result = *this;
      _result.x /= rhs;
      _result.y /= rhs;
      _result.z /= rhs;
      _result.w /= rhs;

      return _result;
    }

    void operator=(const Vector4& rhs) {
      this->x = rhs.x;
      this->y = rhs.y;
      this->z = rhs.z;
      this->w = rhs.w;
    }
    void operator+=(const Vector4& rhs) { *this = *this + rhs; }
    void operator-=(const Vector4& rhs) { *this = *this - rhs; }
    void operator*=(const Vector4& rhs) { *this = *this * rhs; }
    void operator/=(const Vector4& rhs) { *this = *this / rhs; }
    void operator*=(float rhs) { *this = *this * rhs; }
    void operator/=(float rhs) { *this = *this / rhs; }

                 operator float*() { return reinterpret_cast<float*>(this); }
                 operator const float*() { return reinterpret_cast<const float*>(this); }
    float&       operator[](size_t index) { return (reinterpret_cast<float*>(this))[index]; }
    const float& operator[](size_t index) const { return (reinterpret_cast<const float*>(this))[index]; }

    Vector4()  = default;
    ~Vector4() = default;
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
  };
  struct Matrix4 {
    Vector4 v0, v1, v2, v3;

    Matrix4 operator+(const Matrix4& rhs) {
      Matrix4 _result = *this;
      _result.v0 += rhs.v0;
      _result.v1 += rhs.v1;
      _result.v2 += rhs.v2;
      _result.v3 += rhs.v3;

      return _result;
    }
    Matrix4 operator-(const Matrix4& rhs) {
      Matrix4 _result = *this;
      _result.v0 -= rhs.v0;
      _result.v1 -= rhs.v1;
      _result.v2 -= rhs.v2;
      _result.v3 -= rhs.v3;

      return _result;
    }
    Matrix4 operator*(const Matrix4& rhs) {
      Matrix4 _result = *this;
      _result.v0 *= rhs.v0;
      _result.v1 *= rhs.v1;
      _result.v2 *= rhs.v2;
      _result.v3 *= rhs.v3;

      return _result;
    }
    Matrix4 operator/(const Matrix4& rhs) {
      Matrix4 _result = *this;
      _result.v0 /= rhs.v0;
      _result.v1 /= rhs.v1;
      _result.v2 /= rhs.v2;
      _result.v3 /= rhs.v3;

      return _result;
    }
    Matrix4 operator*(float rhs) {
      Matrix4 _result = *this;
      _result.v0 *= rhs;
      _result.v1 *= rhs;
      _result.v2 *= rhs;
      _result.v3 *= rhs;

      return _result;
    }
    Matrix4 operator/(float rhs) {
      Matrix4 _result = *this;
      _result.v0 /= rhs;
      _result.v1 /= rhs;
      _result.v2 /= rhs;
      _result.v3 /= rhs;

      return _result;
    }

    void operator=(const Matrix4& rhs) {
      this->v0 = rhs.v0;
      this->v1 = rhs.v1;
      this->v2 = rhs.v2;
      this->v3 = rhs.v3;
    }
    void operator+=(const Matrix4& rhs) { *this = *this + rhs; }
    void operator-=(const Matrix4& rhs) { *this = *this - rhs; }
    void operator*=(const Matrix4& rhs) { *this = *this * rhs; }
    void operator/=(const Matrix4& rhs) { *this = *this / rhs; }
    void operator*=(float rhs) { *this = *this * rhs; }
    void operator/=(float rhs) { *this = *this / rhs; }

                   operator float*() { return reinterpret_cast<float*>(this); }
                   operator const float*() { return reinterpret_cast<const float*>(this); }
                   operator Vector4*() { return reinterpret_cast<Vector4*>(this); }
                   operator const Vector4*() { return reinterpret_cast<const Vector4*>(this); }
    Vector4&       operator[](size_t index) { return (reinterpret_cast<Vector4*>(this))[index]; }
    const Vector4& operator[](size_t index) const { return (reinterpret_cast<const Vector4*>(this))[index]; }

    Matrix4()  = default;
    ~Matrix4() = default;
    Matrix4(Vector4 v0, Vector4 v1, Vector4 v2, Vector4 v3) : v0(v0), v1(v1), v2(v2), v3(v3) {}
  };
}  // namespace OpenSpeed::MW05::Math
