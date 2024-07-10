#pragma once

namespace dwn::directx
{
	class Vector2
	{
	public:
		Vector2(float x, float y) : x(x), y(y)
		{}
		Vector2(int x, int y) : x(static_cast<float>(x)), y(static_cast<float>(y))
		{}
		Vector2() : Vector2(0.f, 0.f)
		{}
	public:
		float x{}, y{};
	public:
		const Vector2 operator*(const float amount)
		{
			return { x * amount, y * amount };
		}
		const Vector2 operator*(const Vector2 vec)
		{
			return { x * vec.x, y * vec.y };
		}
		const Vector2 operator/(const float amount)
		{
			return { x / amount, y / amount };
		}
		const Vector2 operator/(const Vector2 vec)
		{
			return { x / vec.x, y / vec.y };
		}
		const Vector2 operator+(const float amount)
		{
			return { x + amount, y + amount };
		}
		const Vector2 operator+(const Vector2 vec)
		{
			return { x + vec.x, y + vec.y };
		}
		const Vector2 operator-(const float amount)
		{
			return { x - amount, y - amount };
		}
		const Vector2 operator-(const Vector2 vec)
		{
			return { x - vec.x, y - vec.y };
		}
		Vector2& operator+=(const float& amount)
		{
			x += amount;
			y += amount;
			return *this;
		}
		Vector2& operator+=(const Vector2& vec)
		{
			x += vec.x;
			y += vec.y;
			return *this;
		}
		Vector2& operator-=(const float& amount)
		{
			x -= amount;
			y -= amount;
			return *this;
		}
		Vector2& operator-=(const Vector2& vec)
		{
			x -= vec.x;
			y -= vec.y;
			return *this;
		}
		Vector2& operator/=(const float& amount)
		{
			x /= amount;
			y /= amount;
			return *this;
		}
		Vector2& operator/=(const Vector2& vec)
		{
			x /= vec.x;
			y /= vec.y;
			return *this;
		}
		Vector2& operator*=(const float& amount)
		{
			x *= amount;
			y *= amount;
			return *this;
		}
		Vector2& operator*=(const Vector2& vec)
		{
			x *= vec.x;
			y *= vec.y;
			return *this;
		}
	};
	class Vector3
	{
	public:
		Vector3(float x, float y, float z) : x(x), y(y), z(z)
		{}
		Vector3(int x, int y, int z) : x(static_cast<float>(x)), y(static_cast<float>(y)), z(static_cast<float>(z))
		{}
		Vector3() : Vector3(0.f, 0.f, 0.f)
		{}
	public:
		float x{}, y{}, z{};
	public:
		Vector3 operator*(const float amount)
		{
			return { x * amount, y * amount, z * amount };
		}
		Vector3 operator*(const Vector3 vec)
		{
			return { x * vec.x, y * vec.y, z * vec.z };
		}
		Vector3 operator/(const float amount)
		{
			return { x / amount, y / amount, z / amount };
		}
		Vector3 operator/(const Vector3 vec)
		{
			return { x / vec.x, y / vec.y, z / vec.z };
		}
		Vector3 operator+(const float amount)
		{
			return { x + amount, y + amount, z + amount };
		}
		Vector3 operator+(const Vector3 vec)
		{
			return { x + vec.x, y + vec.y, z + vec.z };
		}
		Vector3 operator-(const float amount)
		{
			return { x - amount, y - amount, z - amount };
		}
		Vector3 operator-(const Vector3 vec)
		{
			return { x - vec.x, y - vec.y, z - vec.z };
		}
		Vector3& operator+=(const float& amount)
		{
			x += amount;
			y += amount;
			z += amount;
			return *this;
		}
		Vector3& operator+=(const Vector3& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}
		Vector3& operator-=(const float& amount)
		{
			x -= amount;
			y -= amount;
			z -= amount;
			return *this;
		}
		Vector3& operator-=(const Vector3& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}
		Vector3& operator/=(const float& amount)
		{
			x /= amount;
			y /= amount;
			z /= amount;
			return *this;
		}
		Vector3& operator/=(const Vector3& vec)
		{
			x /= vec.x;
			y /= vec.y;
			z /= vec.z;
			return *this;
		}
		Vector3& operator*=(const float& amount)
		{
			x *= amount;
			y *= amount;
			z *= amount;
			return *this;
		}
		Vector3& operator*=(const Vector3& vec)
		{
			x *= vec.x;
			y *= vec.y;
			z *= vec.z;
			return *this;
		}
	};
	class Vector4
	{
	public:
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
		{}
		Vector4(Vector2 v1, Vector2 v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y)
		{}
		Vector4(int x, int y, int z, int w) : x(static_cast<float>(x)), y(static_cast<float>(y)), z(static_cast<float>(z)), w(static_cast<float>(w))
		{}
		Vector4() : Vector4(0.f, 0.f, 0.f, 0.f)
		{}
	public:
		float x{}, y{}, z{}, w{};
	public:
		Vector4 operator*(const float amount)
		{
			return { x * amount, y * amount, z * amount, w * amount };
		}
		Vector4 operator*(const Vector4 vec)
		{
			return { x * vec.x, y * vec.y, z * vec.z, w * vec.w };
		}
		Vector4 operator/(const float amount)
		{
			return { x / amount, y / amount, z / amount, w / amount };
		}
		Vector4 operator/(const Vector4 vec)
		{
			return { x / vec.x, y / vec.y, z / vec.z, w / vec.w };
		}
		Vector4 operator+(const float amount)
		{
			return { x + amount, y + amount, z + amount, w + amount };
		}
		Vector4 operator+(const Vector4 vec)
		{
			return { x + vec.x, y + vec.y, z + vec.z, w + vec.w };
		}
		Vector4 operator-(const float amount)
		{
			return { x - amount, y - amount, z - amount, w - amount };
		}
		Vector4 operator-(const Vector4 vec)
		{
			return { x - vec.x, y - vec.y, z - vec.z, w - vec.w };
		}
		Vector4& operator+=(const float& amount)
		{
			x += amount;
			y += amount;
			z += amount;
			w += amount;
			return *this;
		}
		Vector4& operator+=(const Vector4& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}
		Vector4& operator-=(const float& amount)
		{
			x -= amount;
			y -= amount;
			z -= amount;
			w -= amount;
			return *this;
		}
		Vector4& operator-=(const Vector4& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			w -= vec.w;
			return *this;
		}
		Vector4& operator/=(const float& amount)
		{
			x /= amount;
			y /= amount;
			z /= amount;
			w /= amount;
			return *this;
		}
		Vector4& operator/=(const Vector4& vec)
		{
			x /= vec.x;
			y /= vec.y;
			z /= vec.z;
			w /= vec.w;
			return *this;
		}
		Vector4& operator*=(const float& amount)
		{
			x *= amount;
			y *= amount;
			z *= amount;
			w *= amount;
			return *this;
		}
		Vector4& operator*=(const Vector4& vec)
		{
			x *= vec.x;
			y *= vec.y;
			z *= vec.z;
			w *= vec.w;
			return *this;
		}
	};
}