#pragma once
#include <d3d11.h>
#include "renderer/com_ptr.h"
#include "renderer/shaders/vertex.h"

namespace dwn::directx
{
	inline uint32_t format_size(DXGI_FORMAT format)
	{
		switch (format)
		{
			case DXGI_FORMAT_R32G32B32A32_FLOAT:
			{
				return sizeof(float) * 4;
			} break;
			case DXGI_FORMAT_R32G32B32_FLOAT:
			{
				return sizeof(float) * 3;
			} break;
			case DXGI_FORMAT_R32G32_FLOAT:
			{
				return sizeof(float) * 2;
			} break;
			case DXGI_FORMAT_R32_FLOAT:
			{
				return sizeof(float);
			} break;
		}
		return 0;
	}
	class device : public com_ptr<ID3D11Device>
	{
	public:
		void create(IDXGISwapChain* sc)
		{
			sc->GetDevice(__uuidof(ID3D11Device), (void**)get_address());
		}

		void get_context(ID3D11DeviceContext** ctx)
		{
			get()->GetImmediateContext(ctx);
		}

		void create_rasterizer_state(D3D11_RASTERIZER_DESC& desc, ID3D11RasterizerState** state)
		{
			get()->CreateRasterizerState(&desc, state);
		}

		void create_render_target(ID3D11Resource* resource, ID3D11RenderTargetView** view)
		{
			get()->CreateRenderTargetView(resource, nullptr, view);
		}

		void create_input_layout(const void* bytes, size_t size, ID3D11InputLayout** layout)
		{
			D3D11_INPUT_ELEMENT_DESC elements[4]{};
			elements[0] = { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			elements[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, format_size(elements[0].Format), D3D11_INPUT_PER_VERTEX_DATA, 0 };
			elements[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, format_size(elements[0].Format) + format_size(elements[1].Format), D3D11_INPUT_PER_VERTEX_DATA, 0 };
			elements[3] = { "TEXFLAG", 0, DXGI_FORMAT_R8_UINT, 0, format_size(elements[0].Format) + format_size(elements[1].Format) + format_size(elements[2].Format), D3D11_INPUT_PER_VERTEX_DATA, 0 };
			get()->CreateInputLayout(elements, sizeof(elements) / sizeof(D3D11_INPUT_ELEMENT_DESC), (const void*)bytes, size, layout);
		}

		void create_blend_state(D3D11_BLEND_DESC& desc, ID3D11BlendState** state)
		{
			get()->CreateBlendState(&desc, state);
		}

		void create_depth_stencil_state(D3D11_DEPTH_STENCIL_DESC& desc, ID3D11DepthStencilState** state)
		{
			get()->CreateDepthStencilState(&desc, state);
		}

		void create_pixel_shader(const void* bytes, size_t size, ID3D11PixelShader** shader)
		{
			get()->CreatePixelShader(bytes, size, nullptr, shader);
		}

		void create_vertex_shader(const void* bytes, size_t size, ID3D11VertexShader** shader)
		{
			get()->CreateVertexShader(bytes, size, nullptr, shader);
		}

		void create_buffer(D3D11_BUFFER_DESC& desc, ID3D11Buffer** buffer)
		{
			get()->CreateBuffer(&desc, nullptr, buffer);
		}

		void create_sampler_state(D3D11_SAMPLER_DESC& desc, ID3D11SamplerState** state)
		{
			get()->CreateSamplerState(&desc, state);
		}

		void create_texture_2d(D3D11_TEXTURE2D_DESC& desc, D3D11_SUBRESOURCE_DATA& subResource, ID3D11Texture2D*& texture)
		{
			get()->CreateTexture2D(&desc, &subResource, &texture);
		}

		void create_shader_resource_view(ID3D11Texture2D* texture, DXGI_FORMAT format, ID3D11ShaderResourceView*& view)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc{ format, D3D11_SRV_DIMENSION_TEXTURE2D, { 0u, 1u } };
			get()->CreateShaderResourceView(texture, &desc, &view);
		}
	};
}