#pragma once
#include "device.h"
#include "blob.h"
#include <d3dcompiler.h>
#include <string>
#include <fstream>

namespace dwn::directx
{
	class pixel_shader : public com_ptr<ID3D11PixelShader>
	{
	public:
		void create(device& dev)
		{
			if (!m_blob.get())
				return;
			dev.create_pixel_shader(m_blob.get_buffer(), m_blob.get_buffer_size(), get_address());
		}

		template <size_t size>
		bool compile(const char(&shader)[size])
		{
			return D3DCompile((LPCVOID)shader, size, nullptr, nullptr, nullptr, "PS_ShaderMain", "ps_5_0", NULL, NULL, m_blob.get_address(), nullptr) == S_OK;
		}

		bool compile_file(const stdfs::path& path)
		{
			std::ifstream file{ path };
			std::string contents{ (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>() };
			return D3DCompile((LPCVOID)contents.data(), contents.size(), nullptr, nullptr, nullptr, "PS_ShaderMain", "ps_5_0", NULL, NULL, m_blob.get_address(), nullptr) == S_OK;
		}

		bool precompiled(const stdfs::path& path)
		{
			std::ifstream file{ path, std::ios_base::binary | std::ios_base::ate };

			size_t size{ static_cast<size_t>(file.tellg()) };
			file.seekg(0, std::ios_base::beg);

			bool success{ file.is_open() };

			if (success)
			{
				m_blob.create(size);
				file.read(reinterpret_cast<char*>(m_blob.get_buffer()), m_blob.get_buffer_size());
				file.close();
			}

			return success;
		}

		blob m_blob{};
	};
}