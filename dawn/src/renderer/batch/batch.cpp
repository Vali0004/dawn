#include "pch/pch.h"
#include "batch.h"
#include <algorithm>

namespace dwn::directx
{
	void batch::create(device& dev, context& ctx, uint32_t size)
	{
		m_context = ctx;
		m_max_indices = size * 3;
		m_max_vertices = size;
		m_vertex_buffer_size = sizeof(vertex);
		m_index_buffer.create(dev, m_max_indices * sizeof(uint16_t), D3D11_BIND_INDEX_BUFFER);
		m_vertex_buffer.create(dev, m_max_vertices * sizeof(vertex), D3D11_BIND_VERTEX_BUFFER);
		m_vertices.reserve(m_max_vertices);
		m_indices.reserve(m_max_indices);
	}

	void batch::destroy()
	{
		m_vertex_buffer.release();
		m_index_buffer.release();
		m_current_vertex = 0;
		m_current_index = 0;
		m_draw_calls.clear();
		m_vertices.clear();
		m_indices.clear();
		m_current_texture = nullptr;
	}

	void batch::begin()
	{
		m_current_vertex = 0;
		m_current_index = 0;
		m_draw_calls.clear();
		m_vertices.clear();
		m_indices.clear();
		m_current_texture = nullptr;
	}

	void batch::draw(D3D11_PRIMITIVE_TOPOLOGY topology, bool indexed, const uint16_t* indices, uint32_t index_count, vertex* vertices, uint32_t vertex_count, ID3D11ShaderResourceView* texture)
	{
		if (indexed && (!indices || index_count > m_max_indices)) return;
		if (vertex_count > m_max_vertices) return;

		if (m_draw_calls.empty() || m_current_texture != texture)
		{
			m_draw_calls.push_back({ texture, index_count, m_current_index, m_current_vertex });
			m_current_texture = texture;
		}
		else
		{
			m_draw_calls.back().index_count += index_count;
		}

		for (size_t i{}; i != vertex_count; ++i)
		{
			m_vertices.push_back(vertices[i]);
		}

		if (indexed)
		{
			for (size_t i{}; indexed && i != index_count; ++i)
			{
				m_indices.push_back(indices[i] + m_current_vertex);
			}
		}

		m_current_vertex += vertex_count;
		m_current_index += index_count;
	}

	void batch::end()
	{
		if (m_vertices.empty() || m_indices.empty())
			return;

		D3D11_MAPPED_SUBRESOURCE mapped_resource{};
		if (m_context.map(m_vertex_buffer, true, mapped_resource) && mapped_resource.pData)
		{
			memcpy(mapped_resource.pData, m_vertices.data(), m_vertices.size() * m_vertex_buffer_size);
			m_context.unmap(m_vertex_buffer);
		}

		if (m_context.map(m_index_buffer, true, mapped_resource) && mapped_resource.pData)
		{
			memcpy(mapped_resource.pData, m_indices.data(), m_indices.size() * sizeof(uint16_t));
			m_context.unmap(m_index_buffer);
		}

		m_context.set_index_buffer(m_index_buffer.get());
		m_context.set_vertex_buffers(m_vertex_buffer.get_address(), m_vertex_buffer_size);

		std::sort(m_draw_calls.begin(), m_draw_calls.end(), [](const draw_call& a, const draw_call& b) {
			return a.texture < b.texture;
		});

		ID3D11ShaderResourceView* last_texture{};

		for (auto& draw : m_draw_calls)
		{
			m_context.set_topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			if (draw.texture != last_texture)
			{
				// Bind the new texture
				m_context.set_shader_resource(draw.texture);
				last_texture = draw.texture;
			}

			m_context.draw_indexed(draw.index_count, draw.index_offset, draw.vertex_offset);
		}

		// Reset shader resource after drawing
		if (last_texture != nullptr)
		{
			ID3D11ShaderResourceView* dummy[1]{ nullptr };
			m_context.get()->PSSetShaderResources(0, 1, dummy);
		}
	}
}