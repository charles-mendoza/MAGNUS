#pragma once
#ifndef MAGNUS_STRUCTURES_H
#define MAGNUS_STRUCTURES_H

#ifdef _MSC_VER
#pragma warning(disable : 4244)
#pragma warning(disable : 4251)
#endif

#include <string>
#include <math.h>

namespace Magnus
{
	namespace Controls
	{
		class Control;
		class Canvas;
	}

	typedef wchar_t UnicodeChar;
	
	typedef std::string String;
	typedef std::wstring UnicodeString;

	class TextObject;

	/*

	Represnets an ARGB (alpha, red, green, blue) color.

	*/
	struct Color
	{
		Color(unsigned char a_ = 255, unsigned char r_ = 255, unsigned char g_ = 255, unsigned char b_ = 255)
		{
			this->a = a_;
			this->r = r_;
			this->g = g_;
			this->b = b_;
		}

		static Color ToRGB(unsigned long hex)
		{
			Color c;
			c.a = ((hex >> 24) & 0xff);
			c.r = ((hex >> 16) & 0xff);
			c.g = ((hex >> 8) & 0xff);
			c.b = ((hex) & 0xff);
			return c;
		}

		static unsigned long ToHex(Color rgb)
		{
			return (unsigned long)((((rgb.a) & 0xff) << 24) | (((rgb.r) & 0xff) << 16) | (((rgb.g) & 0xff) << 8) | ((rgb.b) & 0xff));
		}

		Color operator + (const Color & c) const
		{
			return Color(a + c.a, r + c.r, g + c.g, b + c.b);
		}

		Color operator - (const Color & c) const
		{
			return Color(a - c.a, r - c.r, g - c.g, b - c.b);
		}

		bool operator == (const Color & c) const
		{
			return ((a == c.a) && (r == c.b) && (g == c.g) && (b == c.b));
		}

		unsigned char a, r, g, b;
	};

	/*

	Represents an HSV (hue, saturation, value) color.

	*/
	struct HSV
	{
		float h;
		float s;
		float v;
	};

	/*

	2D Vector

	*/
	struct Vector2
	{
		Vector2(float x_ = 0.0f, float y_ = 0.0f)
		{
			this->x = x_;
			this->y = y_;
		}

		static float Angle(const Vector2 & from, const Vector2 & to)
		{
			float dot = Dot(from.Normalized(), to.Normalized());

			if (dot < -1.0f) { dot = -1.0f; }

			if (dot > 1.0f) { dot = 1.0f; }

			return acos(dot) * 57.29578f;
		}

		float Magnitude() const
		{
			return sqrt(x * x + y * y);
		}

		float SqrMagnitude() const
		{
			return x * x + y * y;
		}

		Vector2 Normalized() const
		{
			Vector2 result = Vector2(x, y);
			result.Normalize();
			return result;
		}

		void Normalize()
		{
			float m = Magnitude();

			if (m)
			{
				const float il = 1.0f / m;
				x *= il;
				y *= il;
			}
			else
			{
				x = y = 0.0f;
			}
		}

		float Distance(const Vector2 & v) const
		{
			float dx = x - v.x;
			float dy = y - v.y;
			return sqrt(dx * dx + dy * dy);
		}

		static float Dot(const Vector2 & lhs, const Vector2 & rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y;
		}

		float Dot(const Vector2 & v) const
		{
			return x * v.x + y * v.y;
		}

		static Vector2 Lerp(Vector2 a, Vector2 b, float t)
		{
			if (t < 0.0f) { t = 0.0f; }

			if (t > 1.0f) { t = 1.0f; }

			return Vector2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
		}

		static Vector2 LerpUnclamped(Vector2 a, Vector2 b, float t)
		{
			return Vector2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
		}

		const Vector2 & operator = (const Vector2 & v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}

		float & operator [] (int index)
		{
			return (&x)[index];
		}

		float operator [] (int index) const
		{
			return (&x)[index];
		}

		bool operator == (const Vector2 & v) const
		{
			return ((x == v.x) && (y == v.y));
		}

		bool operator != (const Vector2 & v) const
		{
			return ((x != v.x) || (y != v.y));
		}

		Vector2 operator - () const
		{
			return Vector2(-x, -y);
		}

		Vector2 operator + (const Vector2 & v) const
		{
			return Vector2(x + v.x, y + v.y);
		}

		Vector2 operator - (const Vector2 & v) const
		{
			return Vector2(x - v.x, y - v.y);
		}

		Vector2 operator * (float f) const
		{
			return Vector2(x * f, y * f);
		}

		Vector2 operator / (float f) const
		{
			f = 1.0f / f; return Vector2(x * f, y * f);
		}

		Vector2 & operator += (const Vector2 & v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		Vector2 & operator -= (const Vector2 & v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		Vector2 & operator *= (float f)
		{
			x *= f;
			y *= f;
			return *this;
		}

		Vector2 & operator /= (float f)
		{
			f = 1.0f / f;
			x *= f;
			y *= f;
			return *this;
		}

		float x, y;
	};

	/*

	3D Vector

	*/
	struct Vector3
	{
		Vector3(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f)
		{
			this->x = x_;
			this->y = y_;
			this->z = z_;
		}

		static float Angle(const Vector3 & from, const Vector3 & to)
		{
			float dot = Dot(from.Normalized(), to.Normalized());

			if (dot < -1.0f) { dot = -1.0f; }

			if (dot > 1.0f) { dot = 1.0f; }

			return acos(dot) * 57.29578f;
		}

		float Magnitude() const
		{
			return sqrt(x * x + y * y + z * z);
		}

		float SqrMagnitude() const
		{
			return x * x + y * y + z * z;
		}

		Vector3 Normalized() const
		{
			Vector3 result = Vector3(x, y, z);
			result.Normalize();
			return result;
		}

		void Normalize()
		{
			float m = Magnitude();

			if (m)
			{
				const float il = 1.0f / m;
				x *= il;
				y *= il;
				z *= il;
			}
			else
			{
				x = y = z = 0.0f;
			}
		}

		float Distance(const Vector3 & v) const
		{
			float dx = x - v.x;
			float dy = y - v.y;
			float dz = z - v.z;
			return sqrt(dx * dx + dy * dy + dz * dz);
		}

		static float Dot(const Vector3 & lhs, const Vector3 & rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
		}

		float Dot(const Vector3 & v) const
		{
			return x * v.x + y * v.y + z * v.z;
		}

		static float Cross(const Vector3 & lhs, const Vector3 & rhs)
		{
			return lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
		}

		float Cross(const Vector3 & v) const
		{
			return x * v.x - y * v.y - z * v.z;
		}

		static Vector3 Lerp(Vector3 a, Vector3 b, float t)
		{
			if (t < 0.0f) { t = 0.0f; }

			if (t > 1.0f) { t = 1.0f; }

			return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) *t, a.z + (b.z - a.z) * t);
		}

		static Vector3 LerpUnclamped(Vector3 a, Vector3 b, float t)
		{
			return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) *t, a.z + (b.z - a.z) * t);
		}

		static Vector3 Slerp(Vector3 a, Vector3 b, float t)
		{
			float dot = Dot(a, b);

			if (dot < -1.0f) { dot = -1.0f; }

			if (dot > 1.0f) { dot = 1.0f; }

			if (t < 0.0f) { t = 0.0f; }

			if (t > 1.0f) { t = 1.0f; }

			float theta = acos(dot) * t;
			Vector3 relative = b - a * dot;
			relative.Normalize();

			return ((a * cos(theta)) + (relative * sin(theta)));
		}

		static Vector3 SlerpUnclamped(Vector3 a, Vector3 b, float t)
		{
			float dot = Dot(a, b);

			if (dot < -1.0f) { dot = -1.0f; }

			if (dot > 1.0f) { dot = 1.0f; }

			float theta = acos(dot) * t;
			Vector3 relative = b - a * dot;
			relative.Normalize();

			return ((a * cos(theta)) + (relative * sin(theta)));
		}

		const Vector3 & operator = (const Vector3 & v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		float & operator [] (int index)
		{
			return (&x)[index];
		}

		float operator [] (int index) const
		{
			return (&x)[index];
		}

		bool operator == (const Vector3 & v) const
		{
			return ((x == v.x) && (y == v.y) && (z == v.z));
		}

		bool operator != (const Vector3 & v) const
		{
			return ((x != v.x) || (y != v.y) || (z != v.z));
		}

		Vector3 operator - () const
		{
			return Vector3(-x, -y, -z);
		}

		Vector3 operator + (const Vector3 & v) const
		{
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		Vector3 operator - (const Vector3 & v) const
		{
			return Vector3(x - v.x, y - v.y, z - v.z);
		}

		Vector3 operator * (float f) const
		{
			return Vector3(x * f, y * f, z * f);
		}

		Vector3 operator / (float f) const
		{
			f = 1.0f / f; return Vector3(x * f, y * f, z * f);
		}

		Vector3 & operator += (const Vector3 & v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		Vector3 & operator -= (const Vector3 & v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		Vector3 & operator *= (float f)
		{
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}

		Vector3 & operator /= (float f)
		{
			f = 1.0f / f;
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}

		float x, y, z;
	};

	/*

	4D Vector

	*/
	struct Vector4
	{
		Vector4(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f, float w_ = 0.0f)
		{
			this->x = x_;
			this->y = y_;
			this->z = z_;
			this->w = w_;
		}

		float Magnitude() const
		{
			return sqrt(x * x + y * y + z * z + w * w);
		}

		float SqrMagnitude() const
		{
			return x * x + y * y + z * z + w * w;
		}

		Vector4 Normalized() const
		{
			Vector4 result = Vector4(x, y, z, w);
			result.Normalize();
			return result;
		}

		void Normalize()
		{
			float m = Magnitude();

			if (m)
			{
				const float il = 1.0f / m;
				x *= il;
				y *= il;
				z *= il;
				w *= il;
			}
			else
			{
				x = y = z = w = 0.0f;
			}
		}

		float Distance(const Vector4 & v) const
		{
			float dx = x - v.x;
			float dy = y - v.y;
			float dz = z - v.z;
			float dw = w - v.w;
			return sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
		}

		static float Dot(const Vector4 & lhs, const Vector4 & rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
		}

		float Dot(const Vector4 & v) const
		{
			return x * v.x + y * v.y + z * v.z + w * v.w;
		}

		static Vector4 Lerp(Vector4 a, Vector4 b, float t)
		{
			if (t < 0.0f) { t = 0.0f; }

			if (t > 1.0f) { t = 1.0f; }

			return Vector4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
		}

		static Vector4 LerpUnclamped(Vector4 a, Vector4 b, float t)
		{
			return Vector4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
		}

		const Vector4 & operator = (const Vector4 & v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
			return *this;
		}

		float & operator [] (int index)
		{
			return (&x)[index];
		}

		float operator [] (int index) const
		{
			return (&x)[index];
		}

		bool operator == (const Vector4 & v) const
		{
			return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w));
		}

		bool operator != (const Vector4 & v) const
		{
			return ((x != v.x) || (y != v.y) || (z != v.z) || (w != v.w));
		}

		Vector4 operator - () const
		{
			return Vector4(-x, -y, -z, -w);
		}

		Vector4 operator + (const Vector4 & v) const
		{
			return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
		}

		Vector4 operator - (const Vector4 & v) const
		{
			return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
		}

		Vector4 operator * (float f) const
		{
			return Vector4(x * f, y * f, z * f, w * f);
		}

		Vector4 operator / (float f) const
		{
			f = 1.0f / f; return Vector4(x * f, y * f, z * f, w * f);
		}

		Vector4 & operator += (const Vector4 & v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}

		Vector4 & operator -= (const Vector4 & v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}

		Vector4 & operator *= (float f)
		{
			x *= f;
			y *= f;
			z *= f;
			w *= f;
			return *this;
		}

		Vector4 & operator /= (float f)
		{
			f = 1.0f / f;
			x *= f;
			y *= f;
			z *= f;
			w *= f;
			return *this;
		}

		float x, y, z, w;
	};

	/*

	3x3 matrix

	*/
	struct Matrix3x3
	{
		Matrix3x3(float _11 = 0.0f, float _12 = 0.0f, float _13 = 0.0f,
				  float _21 = 0.0f, float _22 = 0.0f, float _23 = 0.0f,
				  float _31 = 0.0f, float _32 = 0.0f, float _33 = 0.0f)
		{
			this->_11 = _11;
			this->_12 = _12;
			this->_13 = _13;
			this->_21 = _21;
			this->_22 = _22;
			this->_23 = _23;
			this->_31 = _31;
			this->_32 = _32;
			this->_33 = _33;
		}

		Vector3 GetColumn(int index) const
		{
			return Vector3(m[0][index], m[1][index], m[2][index]);
		}

		Vector3 GetRow(int index) const
		{
			return Vector3(m[index][0], m[index][1], m[index][2]);
		}

		union
		{
			struct
			{
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;
			};

			float m[3][3];
			float mm[9];
		};
	};

	/*

	4x4 matrix

	*/
	struct Matrix4x4
	{
		Matrix4x4(float _11 = 0.0f, float _12 = 0.0f, float _13 = 0.0f, float _14 = 0.0f,
				  float _21 = 0.0f, float _22 = 0.0f, float _23 = 0.0f, float _24 = 0.0f,
				  float _31 = 0.0f, float _32 = 0.0f, float _33 = 0.0f, float _34 = 0.0f,
				  float _41 = 0.0f, float _42 = 0.0f, float _43 = 0.0f, float _44 = 0.0f)
		{
			this->_11 = _11;
			this->_12 = _12;
			this->_13 = _13;
			this->_14 = _14;
			this->_21 = _21;
			this->_22 = _22;
			this->_23 = _23;
			this->_24 = _24;
			this->_31 = _31;
			this->_32 = _32;
			this->_33 = _33;
			this->_34 = _34;
			this->_41 = _41;
			this->_42 = _42;
			this->_43 = _43;
			this->_44 = _44;
		}

		Vector4 GetColumn(int index) const
		{
			return Vector4(m[0][index], m[1][index], m[2][index], m[3][index]);
		}

		Vector4 GetRow(int index) const
		{
			return Vector4(m[index][0], m[index][1], m[index][2], m[index][3]);
		}

		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};

			float m[4][4];
			float mm[16];
		};
	};

	/*

	Represents an ordered pair of integer x- and y-coordinates that defines a point in a two-dimensional plane.

	*/
	struct Point
	{
		Point(int x_ = 0, int y_ = 0)
		{
			this->x = x_;
			this->y = y_;
		}

		Point operator + (const Point & p) const
		{
			return Point(x + p.x, y + p.y);
		}

		Point operator - (const Point & p) const
		{
			return Point(x - p.x, y - p.y);
		}

		bool operator == (const Point & p) const
		{
			return ((x == p.x) && (y == p.y));
		}

		int x, y;
	};

	/*

	Stores an odered pair of integers, which specify a Height and Width.

	*/
	struct Size
	{
		Size(int w_ = 0, int h_ = 0)
		{
			this->w = w_;
			this->h = h_;
		}

		Size operator + (const Size & s) const
		{
			return Size(w + s.w, h + s.h);
		}

		Size operator - (const Size & s) const
		{
			return Size(w - s.w, h - s.h);
		}

		bool operator == (const Size & s) const
		{
			return ((w == s.w) && (h == s.h));
		}

		int w, h;
	};

	/*

	Stores a set of four integers that represent the location and size of a rectangle.

	*/
	struct Rect
	{
		Rect(int x_ = 0, int y_ = 0, int w_ = 0, int h_ = 0)
		{
			this->x = x_;
			this->y = y_;
			this->w = w_;
			this->h = h_;
		}

		Rect(Point pos, Size size)
		{
			this->x = pos.x;
			this->y = pos.y;
			this->w = size.w;
			this->h = size.h;
		}

		Rect operator + (const Rect & r) const
		{
			return Rect(x + r.x, y + r.y, w + r.w, h + r.h);
		}

		Rect operator - (const Rect & r) const
		{
			return Rect(x - r.x, y - r.y, w - r.w, h - r.h);
		}

		bool operator == (const Rect & r) const
		{
			return ((x == r.x) && (y == r.y) && (w == r.w) && (h == r.h));
		}

		int x, y, w, h;
	};

	/*

	Represents margin information associated with a user interface (UI) element.

	*/
	struct Margin
	{
		Margin(int left_ = 0, int top_ = 0, int right_ = 0, int bottom_ = 0)
		{
			this->left = left_;
			this->top = top_;
			this->right = right_;
			this->bottom = bottom_;
		}

		Margin operator + (const Margin & m) const
		{
			return Margin(left + m.left, top + m.top, right + m.right, bottom + m.bottom);
		}

		Margin operator - (const Margin & m) const
		{
			return Margin(left - m.left, top - m.top, right - m.right, bottom - m.bottom);
		}

		bool operator == (const Margin & m) const
		{
			return ((left == m.left) && (top == m.top) && (right == m.right) && (bottom == m.bottom));
		}

		int left, top, right, bottom;
	};


	/*

	Represents padding information associated with a user interface (UI) element.

	*/
	typedef Margin Padding;
}

#endif