#pragma once
#include "pch/framework.h"
#define LOG_TO_STREAM(...) \
	(*dwn::g_console) << __VA_ARGS__ << dwn::endl
#define LOG_TO_STREAM_NO_IO_MANIP(...) \
	(*dwn::g_console) << __VA_ARGS__
#define LOG_NEWLINE() \
	(*dwn::g_console) << dwn::endl
#define LOG_TO_FILE(str) \
	(*dwn::g_console).log_to_stream_file(str);
#define LOG(prefix, ...) \
	LOG_TO_STREAM(std::format(#prefix " | " __VA_ARGS__));
#define LOG_FILE(prefix, ...) \
	LOG_TO_FILE(std::format(#prefix " | " __VA_ARGS__));

namespace dwn
{
	inline HANDLE g_handle{};
	inline HWND g_console_hwnd{};
	inline PROCESS_INFORMATION g_proc_info{};

	inline void open_remote_console()
	{
		stdfs::path p{};
		p = p.append(std::getenv("appdata")).append(BASE_NAME).append("ConsoleHost.exe");
		STARTUPINFOA start_info{ sizeof(STARTUPINFOA) };
		BOOL success = CreateProcessA(NULL, const_cast<char*>(p.string().data()), nullptr, nullptr, TRUE, NULL, nullptr, nullptr, &start_info, &g_proc_info);
		if (!success) {
			return;
		}
	}

	inline bool make_remote_console()
	{
		open_remote_console();
		g_handle = CreateFileA("\\\\.\\pipe\\consolePipe", GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
		g_console_hwnd = FindWindowA(NULL, "ConsoleHost");
		if (!g_console_hwnd || g_handle == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		return true;
	}

	inline void destroy_remote_console()
	{
		if (g_proc_info.hProcess != INVALID_HANDLE_VALUE)
		{
			CloseHandle(g_proc_info.hProcess);
			CloseHandle(g_proc_info.hThread);
		}
		if (g_handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(g_handle);
		}
	}
	inline void reconnect_to_remote_console()
	{
		destroy_remote_console();
		make_remote_console();
	}

	inline class konsole* g_console{};
	class konsole
	{
	public:
		void init_wapi(bool remote_console)
		{
			set_remote_console(remote_console);
			if (remote_console)
			{
				m_remote_console = make_remote_console();
			}
			if (!m_remote_console)
			{
				if (!AttachConsole(ATTACH_PARENT_PROCESS))
					AllocConsole();
				SetConsoleCP(CP_UTF8);
				SetConsoleOutputCP(CP_UTF8);
				ul32 mode{};
				GetConsoleMode(get_handle(), &mode);
				mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
				SetConsoleMode(get_handle(), mode);
				freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
				freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
			}
		}
		void init_cout()
		{
			if (!m_remote_console)
			{
				m_cout.open("CONOUT$");
			}
		}
		void init_filestream(const std::string& file_out)
		{
			stdfs::path path(stdfs::path().append(std::getenv("appdata")).append(BASE_NAME));

			if (!stdfs::exists(path))
			{
				stdfs::create_directories(path);
			}

			m_filestream.open((path).append(file_out), std::ios_base::out);
		}

		void close_handles(bool preseve_file = false)
		{
			if (m_remote_console)
			{
				destroy_remote_console();
			}
			else
			{
				fclose(stdin);
				fclose(stdout);
				m_cout.close();
				FreeConsole();
			}

			if (!preseve_file && m_filestream.is_open())
			{
				m_filestream.close();
			}
		}

		void set_remote_console(bool v)
		{
			m_remote_console = v;
		}

		~konsole()
		{
			close_handles();
			g_console = nullptr;
			delete reinterpret_cast<konsole*>(g_console);
		}

		konsole(bool remote_console)
		{
			g_console = this;
			init_wapi(remote_console);
			init_cout();
		}
		konsole(const std::string& file_out, bool remote_console) : konsole(remote_console)
		{
			init_filestream(file_out);
		}

		HANDLE get_handle()
		{
			return GetStdHandle(STD_OUTPUT_HANDLE);
		}

		void set_console_title(const std::string& title)
		{
			m_title = title;
			if (!m_remote_console)
			{
				SetConsoleTitleA(title.c_str());
			}
			else
			{
				SetWindowTextA(g_console_hwnd, title.c_str());
			}
		}

		void set_text(u16 color)
		{
			if (!m_remote_console)
			{
				SetConsoleTextAttribute(get_handle(), color);
			}
		}

		void send_text_to_handle(std::string str)
		{
			if (!m_remote_console)
			{
				m_cout << str;
			}
			else
			{
				ul32 bytes_written{};
				WriteFile(g_handle, str.c_str(), static_cast<ul32>(str.length()), &bytes_written, nullptr);
			}
		}

		void log_to_stream_console(std::string str)
		{
			if (m_operators.uppercase)
			{
				std::transform(str.cbegin(), str.cend(), str.begin(), &toupper);
			}

			if (m_operators.hex)
			{
				m_operators.hex = false;
			}

			send_text_to_handle(str);
		}
		template <typename T>
		void log_to_stream_int(T v)
		{
			std::string str{};
			if (m_operators.hex)
			{
				if (m_operators.uppercase)
				{
					str = std::format("{:X}", v);
				}
				else
				{
					str = std::format("{:x}", v);
				}
				m_operators.hex = false;
			}
			else
			{
				str = std::format("{}", v);
			}

			send_text_to_handle(str);

			if (m_filestream.is_open())
			{
				m_filestream << str;
			}
		}
		void log_to_stream_file(std::string str)
		{
			if (m_filestream.is_open())
			{
				m_filestream << str;
			}
		}
		void log_to_stream(std::string str)
		{
			log_to_stream_console(str);
			log_to_stream_file(str);
		}

		static void create(konsole* handle, const std::string& title, const std::string& file_out, bool remote_console = true)
		{
			konsole* out{ new konsole(file_out, remote_console) };
			out->set_console_title(title);
			handle = out;
		}

		static void destroy(konsole* handle)
		{
			handle->~konsole();
		}

		// STD parity
		void flush()
		{
			if (!m_remote_console)
			{
				m_cout.flush();
			}
		}
	public:
		struct
		{
			bool hex{}, uppercase{};
		} m_operators{};
		std::string m_title{};
	private:
		bool m_remote_console{};
		std::ofstream m_cout{};
		std::ofstream m_filestream{};
	};

	inline konsole& endl(konsole& output)
	{
		output.log_to_stream("\n");
		output.flush();
		return output;
	}
}
inline dwn::konsole& operator<<(dwn::konsole& output, std::ios_base&(*manip)(std::ios_base&))
{
	if (manip == std::uppercase)
	{
		output.m_operators.uppercase = true;
	}
	else if (manip == std::nouppercase)
	{
		output.m_operators.uppercase = false;
	}
	else if (manip == std::hex)
	{
		output.m_operators.hex = true;
	}
	else if (manip == std::dec)
	{
		output.m_operators = {};
	}
	return output;
}
inline dwn::konsole& operator<<(dwn::konsole& output, dwn::konsole&(*manip)(dwn::konsole&))
{
	return (*manip)(output);
}
template <typename T>
inline dwn::konsole& operator<<(dwn::konsole& output, T numerial)
{
	if (std::is_arithmetic_v<T>)
	{
		output.log_to_stream_int<T>(numerial);
	}

	return output << std::nouppercase;
}
inline dwn::konsole& operator<<(dwn::konsole& output, f64 numerial)
{
	output.log_to_stream(std::format("{}", numerial));
	return output;
}
inline dwn::konsole& operator<<(dwn::konsole& output, f32 numerial)
{
	output.log_to_stream(std::format("{}", numerial));
	return output;
}
inline dwn::konsole& operator<<(dwn::konsole& output, const char* str)
{
	output.log_to_stream(str);
	return output;
}
inline dwn::konsole& operator<<(dwn::konsole& output, char* str)
{
	output.log_to_stream(str);
	return output;
}
inline dwn::konsole& operator<<(dwn::konsole& output, const std::string& str)
{
	output.log_to_stream(str);
	return output;
}
inline dwn::konsole& operator<<(dwn::konsole& output, const std::string_view& str)
{
	output.log_to_stream(str.data());
	return output;
}
struct color
{
	color(u8 r, u8 g, u8 b, bool bg = false) :
		r(r),
		g(g),
		b(b),
		bg(bg)
	{}
	color() {}

	u8 r{}, g{}, b{};
	bool bg{};
};

inline dwn::konsole& operator<<(dwn::konsole& output, color color)
{
	output.log_to_stream_console(std::format("\x1b[{};2;{};{};{}m", color.bg ? 48 : 38, color.r, color.g, color.b));
	return output;
}

namespace dwn::ansi
{
	inline konsole& reset(konsole& output)
	{
		output.log_to_stream_console("\x1b[0m");
		return output;
	}
}