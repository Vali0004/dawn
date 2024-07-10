#pragma once
#include "../vectors.h"
#include "../color.h"

namespace dwn::directx
{
	class vertex
	{
	public:
		vertex(Vector2 pos, Color col, Vector2 texCoord, bool texture) : position(pos), color(col.serialize()), texture_coord(texCoord), use_texture(texture ? 1 : 0)
		{}
		vertex(Vector2 pos, Color col, bool texture) : vertex(pos, col, {}, texture)
		{}
		vertex(Vector2 pos, Color col) : vertex(pos, col, false)
		{}
		vertex() : vertex({ 0.f, 0.f }, {})
		{}
		Vector2 position{};
		Vector4 color{};
		Vector2 texture_coord{};
		uint8_t use_texture{};

		void set(Vector2 pos, Color col, Vector2 texCoord = {}, bool texture = false)
		{
			position = pos;
			color = col.serialize();
			texture_coord = texCoord;
			use_texture = (texture ? 1 : 0);
		}
	};
}