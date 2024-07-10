#pragma once
#include "renderer/d3d/device.h"
#include "renderer/d3d/context.h"
#include "renderer/d3d/buffer.h"
#include "renderer/shaders/vertex.h"
#include <vector>

namespace dwn::directx
{
	struct draw_call
	{
		ID3D11ShaderResourceView* texture{};

		uint32_t index_count{};
		uint32_t index_offset{};

		uint32_t vertex_offset{};
	};
	class batch
	{
	public:
		batch(context& ctx) : m_context(ctx)
		{}
		void create(device& dev, context& ctx, uint32_t size);

		void destroy();

		void begin();

		void draw(D3D11_PRIMITIVE_TOPOLOGY topology, bool indexed, const uint16_t* indices, uint32_t indexCount, vertex* vertices, uint32_t vertexCount, ID3D11ShaderResourceView* texture);

		template <uint32_t vertex_count, uint32_t index_count>
		void draw(vertex(&vertices)[vertex_count], const uint16_t(&indices)[index_count], ID3D11ShaderResourceView* texture = nullptr)
		{
			draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, true, indices, index_count, vertices, vertex_count, texture);
		}
		template <uint32_t vertex_count>
		void draw(vertex(&vertices)[vertex_count], ID3D11ShaderResourceView* texture = nullptr)
		{
			draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, false, nullptr, 0, vertices, vertex_count, texture);
		}

		void end();

	private:
		context& m_context;

		buffer m_vertex_buffer{};
		buffer m_index_buffer{};

		std::vector<draw_call> m_draw_calls{};

		std::vector<vertex> m_vertices{};
		std::vector<uint16_t> m_indices{};

		uint8_t m_vertex_buffer_size{};

		uint32_t m_current_vertex{};
		uint32_t m_current_index{};

		uint32_t m_max_vertices{};
		uint32_t m_max_indices{};

		ID3D11ShaderResourceView* m_current_texture{};
	};
}