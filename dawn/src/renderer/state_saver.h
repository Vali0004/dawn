#pragma once
#include <d3d11.h>

namespace dwn::directx
{
	class state_saver
	{
	public:
		state_saver(ID3D11DeviceContext* context) : m_feature_level(D3D_FEATURE_LEVEL_11_0), m_sample_mask(0xFFFFFFF)
		{
			save(context);
		}
		~state_saver()
		{
			release();
		}

		bool save(ID3D11DeviceContext* context)
		{
			if (m_saved_state)
				release();
			if (!context)
				return false;
			ID3D11Device* dev{};
			context->GetDevice(&dev);
			if (!dev)
			{
				m_feature_level = dev->GetFeatureLevel();
				dev->Release();
			}
			context->AddRef();
			m_p_context = context;
			m_p_context->IAGetPrimitiveTopology(&m_primitive_topology);
			m_p_context->IAGetInputLayout(&m_p_input_layout);
			m_p_context->OMGetBlendState(&m_p_blend_state, m_blend_factor, &m_sample_mask);
			m_p_context->OMGetDepthStencilState(&m_p_depth_stencil_state, &m_stencil_ref);
			m_p_context->RSGetState(&m_p_rasterizer_state);
			m_num_vs_class_instances = 256;
			m_p_context->VSGetShader(&m_p_vs, m_p_vs_class_instances, &m_num_vs_class_instances);
			m_p_context->VSGetConstantBuffers(0, 1, &m_p_vs_constant_buffer);
			m_num_ps_class_instances = 256;
			m_p_context->PSGetShader(&m_p_ps, m_p_ps_class_instances, &m_num_ps_class_instances);
			m_p_context->PSGetShaderResources(0, 1, &m_p_ps_srv);
			context->PSGetSamplers(0, 1, &m_p_sampler_state);
			if (m_feature_level >= D3D_FEATURE_LEVEL_10_0)
			{
				m_num_gs_class_instances = 256;
				m_p_context->GSGetShader(&m_p_gs, m_p_gs_class_instances, &m_num_gs_class_instances);
				m_p_context->GSGetConstantBuffers(0, 1, &m_p_gs_constant_buffer);
				m_p_context->GSGetShaderResources(0, 1, &m_p_gs_srv);
				if (m_feature_level >= D3D_FEATURE_LEVEL_11_0)
				{
					m_num_hs_class_instances = 256;
					m_p_context->HSGetShader(&m_p_hs, m_p_hs_class_instances, &m_num_hs_class_instances);
					m_num_ds_class_instances = 256;
					m_p_context->DSGetShader(&m_p_ds, m_p_ds_class_instances, &m_num_ds_class_instances);
				}
			}
			m_p_context->IAGetVertexBuffers(0, 1, &m_p_vb, &m_vertex_stride, &m_vertex_offset);
			m_p_context->IAGetIndexBuffer(&m_p_index_buffer, &m_index_format, &m_index_offset);
			m_saved_state = true;
			return true;
		}

		bool restore()
		{
			if (!m_saved_state)
				return false;
			m_p_context->IASetPrimitiveTopology(m_primitive_topology);
			m_p_context->IASetInputLayout(m_p_input_layout);
			m_p_context->OMSetBlendState(m_p_blend_state, m_blend_factor, m_sample_mask);
			m_p_context->OMSetDepthStencilState(m_p_depth_stencil_state, m_stencil_ref);
			m_p_context->RSSetState(m_p_rasterizer_state);
			m_p_context->VSSetShader(m_p_vs, m_p_vs_class_instances, m_num_vs_class_instances);
			m_p_context->VSSetConstantBuffers(0, 1, &m_p_vs_constant_buffer);
			m_p_context->PSSetShader(m_p_ps, m_p_ps_class_instances, m_num_ps_class_instances);
			m_p_context->PSSetShaderResources(0, 1, &m_p_ps_srv);
			m_p_context->PSSetSamplers(0, 1, &m_p_sampler_state);
			if (m_feature_level >= D3D_FEATURE_LEVEL_10_0)
			{
				m_p_context->GSSetShader(m_p_gs, m_p_gs_class_instances, m_num_gs_class_instances);
				m_p_context->GSSetConstantBuffers(0, 1, &m_p_gs_constant_buffer);
				m_p_context->GSSetShaderResources(0, 1, &m_p_gs_srv);
				if (m_feature_level >= D3D_FEATURE_LEVEL_11_0)
				{
					m_p_context->HSSetShader(m_p_hs, m_p_hs_class_instances, m_num_hs_class_instances);
					m_p_context->DSSetShader(m_p_ds, m_p_ds_class_instances, m_num_ds_class_instances);
				}
			}
			m_p_context->IASetVertexBuffers(0, 1, &m_p_vb, &m_vertex_stride, &m_vertex_offset);
			m_p_context->IASetIndexBuffer(m_p_index_buffer, m_index_format, m_index_offset);
			return true;
		}

		template <typename T>
		void release_object(T*& p)
		{
			if (p)
			{
				p->Release();
				p = nullptr;
			}
		}

		void release()
		{
			m_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
			release_object(m_p_input_layout);
			release_object(m_p_blend_state);
			for (int i{}; i != 4; ++i)
				m_blend_factor[i] = 0.f;
			m_sample_mask = 0xFFFFFFFF;
			release_object(m_p_depth_stencil_state);
			m_stencil_ref = 0;
			release_object(m_p_rasterizer_state);
			release_object(m_p_ps_srv);
			release_object(m_p_sampler_state);
			release_object(m_p_vs);
			for (UINT i{}; i != m_num_vs_class_instances; ++i)
				release_object(m_p_vs_class_instances[i]);
			m_num_vs_class_instances = 0;
			release_object(m_p_vs_constant_buffer);
			release_object(m_p_gs);
			for (UINT i{}; i != m_num_gs_class_instances; ++i)
				release_object(m_p_gs_class_instances[i]);
			m_num_gs_class_instances = 0;
			release_object(m_p_gs_constant_buffer);
			release_object(m_p_gs_srv);
			release_object(m_p_ps);
			for (UINT i{}; i != m_num_ps_class_instances; ++i)
				release_object(m_p_ps_class_instances[i]);
			m_num_ps_class_instances = 0;
			release_object(m_p_hs);
			for (UINT i{}; i != m_num_hs_class_instances; ++i)
				release_object(m_p_hs_class_instances[i]);
			m_num_hs_class_instances = 0;
			release_object(m_p_ds);
			for (UINT i{}; i != m_num_ds_class_instances; ++i)
				release_object(m_p_ds_class_instances[i]);
			m_num_ds_class_instances = 0;
			release_object(m_p_vb);
			m_vertex_stride = 0;
			m_vertex_offset = 0;
			release_object(m_p_index_buffer);
			m_index_format = DXGI_FORMAT_UNKNOWN;
			m_index_offset = 0;
			release_object(m_p_context);
			m_feature_level = D3D_FEATURE_LEVEL_11_0;
			m_saved_state = false;
		}
	private:
		bool m_saved_state{};
		D3D_FEATURE_LEVEL m_feature_level{};
		ID3D11DeviceContext* m_p_context{};
		D3D11_PRIMITIVE_TOPOLOGY m_primitive_topology{};
		ID3D11InputLayout* m_p_input_layout{};
		ID3D11BlendState* m_p_blend_state{};
		FLOAT m_blend_factor[4]{};
		UINT m_sample_mask{};
		ID3D11DepthStencilState* m_p_depth_stencil_state{};
		UINT m_stencil_ref{};
		ID3D11RasterizerState* m_p_rasterizer_state{};
		ID3D11ShaderResourceView* m_p_ps_srv{};
		ID3D11SamplerState* m_p_sampler_state{};
		ID3D11VertexShader* m_p_vs{};
		ID3D11ClassInstance* m_p_vs_class_instances[256]{};
		UINT m_num_vs_class_instances{};
		ID3D11Buffer* m_p_vs_constant_buffer{};
		ID3D11GeometryShader* m_p_gs{};
		ID3D11ClassInstance* m_p_gs_class_instances[256]{};
		UINT m_num_gs_class_instances{};
		ID3D11Buffer* m_p_gs_constant_buffer{};
		ID3D11ShaderResourceView* m_p_gs_srv{};
		ID3D11PixelShader* m_p_ps{};
		ID3D11ClassInstance* m_p_ps_class_instances[256]{};
		UINT m_num_ps_class_instances{};
		ID3D11HullShader* m_p_hs{};
		ID3D11ClassInstance* m_p_hs_class_instances[256]{};
		UINT m_num_hs_class_instances{};
		ID3D11DomainShader* m_p_ds{};
		ID3D11ClassInstance* m_p_ds_class_instances[256]{};
		UINT m_num_ds_class_instances{};
		ID3D11Buffer* m_p_vb{};
		UINT m_vertex_stride{};
		UINT m_vertex_offset{};
		ID3D11Buffer* m_p_index_buffer{};
		DXGI_FORMAT m_index_format{};
		UINT m_index_offset{};
	};
}