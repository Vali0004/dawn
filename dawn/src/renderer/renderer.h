#pragma once
#include <d3dcompiler.h>
#include <thread>
#include <time.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "state_saver.h"
#include "vectors.h"
#include "color.h"
#include "dxgi/swapchain.h"
#include "d3d/device.h"
#include "d3d/context.h"
#include "d3d/target_view.h"
#include "d3d/blend_state.h"
#include "d3d/input_layout.h"
#include "d3d/rasterizer_state.h"
#include "d3d/depth_stencil_state.h"
#include "d3d/pixel_shader.h"
#include "d3d/vertex_shader.h"
#include "d3d/input_layout.h"
#include "d3d/shader_resource_view.h"
#include "d3d/texture_2d.h"
#include "d3d/sampler_state.h"
#include "batch/batch.h"
#include "font.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#endif
#include "memory/pointers.h"

namespace dwn::renderer
{
	inline float g_drawbase{};
	inline directx::font iconfont_22{};
	inline directx::font iconfont_24{};
	inline directx::font arial_22{};
	inline directx::font arial_23{};
	inline directx::font arialbd_22{};
	inline directx::font arialbd_23{};
	inline directx::font ariali_22{};
	inline directx::font ariali_23{};
	inline directx::font arialbi_22{};
	inline directx::font arialbi_23{};
	using namespace directx;
	inline std::unique_ptr<class renderer> g_renderer{};
	constexpr long double M_PI{ 3.141592653589793238462643383279502884L };
	constexpr long double M_PI_2{ 3.141592653589793238462643383279502884L / 2 };
	enum class eJustify : u8
	{
		Left,
		Center,
		Right
	};
	class renderer final
	{
	public:
		void create(IDXGISwapChain* sc)
		{
			m_wndproc = WNDPROC(SetWindowLongPtrA(pointers::g_hwnd, GWLP_WNDPROC, LONG_PTR(&wndproc)));
			m_swapchain.set(sc, m_device, m_context);
			m_context.create(m_device);
			m_view.create(m_swapchain, m_device);
			m_blend.create(m_device);
			m_rasterizer.create(m_device, D3D11_FILL_SOLID);
			m_depth_stencil.create(m_device, true, true, D3D11_COMPARISON_ALWAYS);
			if (!m_vertex_shader.precompiled(R"(X:\patches\super_patchset\bin\Release\vertex_shader.fxc)"))
				LOG_TO_STREAM("Failed to compile");
			if (!m_pixel_shader.precompiled(R"(X:\patches\super_patchset\bin\Release\pixel_shader.fxc)"))
				LOG_TO_STREAM("Failed to compile");
			m_vertex_shader.create(m_device);
			m_pixel_shader.create(m_device);
			m_sampler_state.create(m_device, {});
			m_input_layout.create(m_device, m_vertex_shader.m_blob);
			m_batch.create(m_device, m_context, 2048);
			m_image_batch.create(m_device, m_context, 2048);
			RECT rect{};
			GetClientRect(pointers::g_hwnd, &rect);
			m_display_size = { static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top) };
			m_state_saver = std::make_unique<state_saver>(m_context.get());
		}

		void destroy(bool resize = false)
		{
			if (resize)
			{
				m_state_saver.release();
				m_blend.release();
				m_view.release();
				m_rasterizer.release();
				m_depth_stencil.release();
				m_pixel_shader.release();
				m_vertex_shader.release();
				m_sampler_state.release();
				m_input_layout.release();
				m_batch.destroy();
				m_image_batch.destroy();
			}
			SetWindowLongPtrA(pointers::g_hwnd, GWLP_WNDPROC, LONG_PTR(m_wndproc));
		}

		void draw_rect(Vector2 pos, const Vector2& size, const Color& color)
		{
			static const uint16_t indices[6]{ 0, 1, 2, 1, 3, 2 };
			static vertex vertices[4]{};
			// Convert position and size from NDC to screen coordinates
			const Vector2 screen_pos{ (pos.x * 2.f) - 1.f,  1.f - (pos.y * 2.f) };
			const Vector2 screen_size{ ((pos.x + size.x) * 2.f) - 1.f,  1.f - ((pos.y + size.y) * 2.f) };
			// Create each corner using the normalized coordinates
			const Vector2 top_left{ screen_pos };
			const Vector2 top_right{ screen_size.x, screen_pos.y };
			const Vector2 bottom_left{ screen_pos.x, screen_size.y };
			const Vector2 bottom_right{ screen_size };
			// Define the vertices
			vertices[0].set(top_left, color);
			vertices[1].set(top_right, color);
			vertices[2].set(bottom_left, color);
			vertices[3].set(bottom_right, color);
			m_batch.begin();
			m_batch.draw(vertices, indices);
			m_batch.end();
		}

		shader_resource_view create_image(const char* path, Vector2& size)
		{
			int image_width{}, image_height{};
			const uint8_t* image_data{ stbi_load(path, &image_width, &image_height, NULL, 4) };

			size = { static_cast<float>(image_width), static_cast<float>(image_height) };

			shader_resource_view texture_srv{};
			texture_2d texture{};
			texture.create(m_device, size, image_data);
			texture_srv.create(m_device, texture);
			texture.release();

			return texture_srv;
		}

		void draw_image(shader_resource_view view, Vector2 pos, Vector2 size, const Color color, const Vector2& uv_min = { 0.f, 0.f }, const Vector2& uv_max = { 1.f, 0.f }, const Vector2& uv2_min = { 0.f, 1.f }, const Vector2& uv2_max = { 1.f, 1.f }, const bool useDC = false)
		{
			static const uint16_t indices[6]{ 0, 1, 2, 1, 3, 2 };
			static vertex vertices[4]{};
			pos /= (useDC ? m_display_size : Vector2());
			size /= (useDC ? m_display_size : Vector2());
			// Convert position and size from NDC to screen coordinates
			const Vector2 screen_pos{ (pos.x * 2.f) - 1.f,  1.f - (pos.y * 2.f) };
			const Vector2 screen_size{ ((pos.x + size.x) * 2.f) - 1.f,  1.f - ((pos.y + size.y) * 2.f) };
			// Create each corner using the normalized coordinates
			const Vector2 top_left{ screen_pos };
			const Vector2 top_right{ screen_size.x, screen_pos.y };
			const Vector2 bottom_left{ screen_pos.x, screen_size.y };
			const Vector2 bottom_right{ screen_size };
			// Define the vertices
			vertices[0].set(top_left, color, uv_min, true);
			vertices[1].set(top_right, color, uv_max, true);
			vertices[2].set(bottom_left, color, uv2_min, true);
			vertices[3].set(bottom_right, color, uv2_max, true);
			m_image_batch.begin();
			m_image_batch.draw(vertices, indices, view.get());
			m_image_batch.end();
		}

		void draw_text(font& f, const char* text, Vector2 position, const Color& color, eJustify justify = eJustify::Left)
		{
			Vector2 justified_pos{ position };
			switch (justify)
			{
				case eJustify::Center:
				{
				} break;
				case eJustify::Right:
				{
					const Vector2 string_size{ get_string_size(f, text) / m_display_size };
					justified_pos = { position.x - string_size.x - 0.0005f, position.y };
				} break;
				default:
				{
				} break;
			}
			Vector2 pos{ justified_pos * m_display_size };
			for (size_t i{}; text[i] >= f.m_first_codepoint && text[i] < (f.m_first_codepoint + f.m_num_characters); ++i)
			{
				auto& c{ f.m_characters[text[i] - f.m_first_codepoint] };
				const Vector2 string_size{ directx::get_string_size(f, text) };
				const Vector4 top_bottom{ c.top_bottom(pos) };
				const Vector4 left_right{ c.left_right() };
				const Vector2 top{ top_bottom.x, top_bottom.y };
				const Vector2 bottom{ top_bottom.z, top_bottom.w };
				const Vector2 left{ left_right.x, left_right.y };
				const Vector2 right{ left_right.z, left_right.w };
				draw_image(f.m_texture,
					{ top.x, top.y - string_size.y },
					{ bottom.x - top.x, bottom.y - top.y },
					color,
					{ left.x, right.x },
					{ left.y, right.x },
					{ left.x, right.y },
					{ left.y, right.y },
					true);
				pos.x += (c.m_codepoint_metrics.x * f.m_scale);
			}
		}

		void begin_frame()
		{
			m_frame_end_time = std::chrono::high_resolution_clock::now();
			m_frame_duration = m_frame_end_time - m_frame_start_time;
			m_context.set_viewport(m_display_size);
			m_context.set_render_targets(m_view.get_address());
			m_state_saved = m_state_saver->save(m_context.get());
			m_context.set_blend(m_blend.get(), 0xFFFFFFFF);
			m_context.set_rasterizer(m_rasterizer.get());
			m_context.set_depth_stencil(m_depth_stencil.get());
			m_context.set_vertex_shader(m_vertex_shader.get());
			m_context.set_pixel_shader(m_pixel_shader.get());
			m_context.set_sampler_state(m_sampler_state.get());
			m_context.set_input_layout(m_input_layout.get());
		}

		void end_frame()
		{
			if (!m_state_saved)
				m_state_saver->restore();
			m_frame_start_time = m_frame_end_time;
			m_frame_time_accumulator += m_frame_duration.count();
			++m_frame_count, ++m_frame_state;
			if (m_frame_state >= 4)
			{
				// Calculate frame time
				m_frame_time_accumulator = 0.f;
				m_frame_state = 0;
			}
		}

		static LRESULT wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		swapchain m_swapchain{};
		device m_device{};
		context m_context{};
		std::unique_ptr<state_saver> m_state_saver{};
		target_view m_view{};
		blend_state m_blend{};
		rasterizer_state m_rasterizer{};
		depth_stencil_state m_depth_stencil{};
		pixel_shader m_pixel_shader{};
		vertex_shader m_vertex_shader{};
		sampler_state m_sampler_state{};
		input_layout m_input_layout{};
		batch m_batch{ m_context };
		batch m_image_batch{ m_context };
		Vector2 m_display_size{};
		std::chrono::duration<float> m_frame_duration{};
		std::chrono::steady_clock::time_point m_frame_end_time{};
		std::chrono::high_resolution_clock::time_point m_frame_start_time{};
		float m_frame_time_accumulator{};
		int m_frame_state{};
		int m_frame_count{};
		bool m_state_saved{};
		WNDPROC m_wndproc{};
	};

	inline void create()
	{
		g_renderer = std::make_unique<renderer>();
		g_renderer->create(*pointers::g_pSwapChain);
	}

	inline renderer* get()
	{
		return g_renderer.get();
	}

	inline void destroy(bool resize = false)
	{
		g_renderer->destroy(resize);
		g_renderer.reset();
	}
}