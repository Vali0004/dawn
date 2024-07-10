#pragma once
#include <cstdint>
#include "vectors.h"

namespace dwn::directx
{
	class Color
	{
	public:
		Color(int32_t r, int32_t g, int32_t b, int32_t a) : _r(r), _g(g), _b(b), _a(a)
		{}
		Color(float r, float g, float b, float a) : Color(int32_t(r) * 255, int32_t(g) * 255, int32_t(b) * 255, int32_t(a) * 255)
		{}
		Color() : Color(0, 0, 0, 0)
		{}
	public:
		Vector4 serialize()
		{
			return Vector4(r_f(), g_f(), b_f(), a_f());
		}
		uint32_t pack()
		{
			return _a << 24 | _b << 16 | _g << 8 | _r << 0;
		}
	public:
		const int r()
		{
			return _r;
		}
		const float r_f()
		{
			return (r() & 0xFF) / 255.f;
		}
	public:
		const int g()
		{
			return _g;
		}
		const float g_f()
		{
			return (g() & 0xFF) / 255.f;
		}
	public:
		const int b()
		{
			return _b;
		}
		const float b_f()
		{
			return (b() & 0xFF) / 255.f;
		}
	public:
		const int a()
		{
			return _a;
		}
		const float a_f()
		{
			return (a() & 0xFF) / 255.f;
		}
	private:
		uint8_t _r;
		uint8_t _g;
		uint8_t _b;
		uint8_t _a;
	};
}