#pragma once
#include "device.h"
#include "buffer.h"
#include "../vectors.h"
#include "../color.h"

namespace dwn::directx
{
	class context : public com_ptr<ID3D11DeviceContext>
	{
	public:
		void create(device& dev)
		{
			dev.get_context(get_address());
		}

		void draw(size_t count, size_t start)
		{
			get()->Draw((uint32_t)(count - start), (uint32_t)start);
		}

		void draw_indexed(size_t count, size_t start, size_t base)
		{
			get()->DrawIndexed((uint32_t)(count - start), (uint32_t)start, (int32_t)base);
		}

		void set_viewport(Vector2 size)
		{
			D3D11_VIEWPORT viewport{ 0.f, 0.f, size.x, size.y, 0.f, 1.f };
			get()->RSSetViewports(1, &viewport);
		}

		void set_render_targets(ID3D11RenderTargetView** view)
		{
			get()->OMSetRenderTargets(1, view, NULL);
		}

		void clear_render_targets(ID3D11RenderTargetView* target, Color col)
		{
			float clearCol[4]{ col.r_f(), col.g_f(), col.b_f(), col.a_f() };
			get()->ClearRenderTargetView(target, clearCol);
		}

		void set_blend(ID3D11BlendState* state, int mask)
		{
			float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
			get()->OMSetBlendState(state, blendFactor, mask);
		}

		void set_depth_stencil(ID3D11DepthStencilState* state)
		{
			get()->OMSetDepthStencilState(state, 0);
		}

		void set_rasterizer(ID3D11RasterizerState* state)
		{
			get()->RSSetState(state);
		}

		void set_vertex_shader(ID3D11VertexShader* shader)
		{
			get()->VSSetShader(shader, nullptr, NULL);
		}

		void set_pixel_shader(ID3D11PixelShader* shader)
		{
			get()->PSSetShader(shader, nullptr, NULL);
		}

		void set_sampler_state(ID3D11SamplerState* sampler)
		{
			get()->PSSetSamplers(NULL, 1, &sampler);
		}

		void set_input_layout(ID3D11InputLayout* layout)
		{
			get()->IASetInputLayout(layout);
		}

		void set_scissor_rects(Vector4 rect)
		{
			const D3D11_RECT r = { (LONG)rect.x, (LONG)rect.y, (LONG)rect.z, (LONG)rect.w };
			get()->RSSetScissorRects(1, &r);
		}

		void set_index_buffer(ID3D11Buffer* buffer)
		{
			get()->IASetIndexBuffer(buffer, DXGI_FORMAT_R16_UINT, 0);
		}

		void set_vertex_buffers(ID3D11Buffer** buffer, size_t size)
		{
			const uint32_t stride{ (uint32_t)size };
			constexpr uint32_t offset{ 0 };
			get()->IASetVertexBuffers(0, 1, buffer, &stride, &offset);
		}

		void set_constant_buffers(ID3D11Buffer* buffer)
		{
			get()->VSSetConstantBuffers(0, 1, &buffer);
		}

		void set_topology(D3D11_PRIMITIVE_TOPOLOGY topology)
		{
			get()->IASetPrimitiveTopology(topology);
		}

		void set_shader_resource(ID3D11ShaderResourceView*& resource)
		{
			get()->PSSetShaderResources(0, 1, &resource);
		}

		void get_shader_resource(ID3D11ShaderResourceView*& resource)
		{
			get()->PSGetShaderResources(0, 1, &resource);
		}

		void update_subresource(ID3D11Resource*& resource, const void* data, Vector2 size, uint8_t bpp)
		{
			const D3D11_BOX box{ 0, 0, 0, static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y), 1 };
			get()->UpdateSubresource(resource, 0, &box, data, static_cast<uint32_t>(size.x * bpp), 0);
		}

		bool map(buffer& buffer, bool discard, D3D11_MAPPED_SUBRESOURCE& resource)
		{
			const D3D11_MAP type{ discard ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE };
			return get()->Map(buffer.get(), 0, type, 0, &resource) == S_OK;
		}

		bool map(buffer& buffer, size_t pos, size_t& basePos, D3D11_MAPPED_SUBRESOURCE& resource)
		{
			bool ret{ map(buffer, pos == 0, resource) };
			basePos = pos;
			return ret;
		}

		void unmap(buffer& buffer)
		{
			get()->Unmap(buffer.get(), 0);
		}

		D3D11_DEVICE_CONTEXT_TYPE get_type()
		{
			return get()->GetType();
		}
	};
}