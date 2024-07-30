#pragma once
#include "common/macros.h"
#include "common/data/types.h"
#include <corecrt_terminate.h>
#include <thread>

namespace dwn
{
	template <u8 a>
	class args
	{
	public:
		void* get_arg(u8 idx)
		{
			return data[idx];
		}
		void set_arg(u8 idx, void* arg)
		{
			data[idx] = arg;
		}
	private:
		void* data[a];
	};
	class thread
	{
		struct keepalive_data
		{
			bool set{};
			bool* conditional{};
			fptr<void()> callback{};
			std::optional<std::chrono::high_resolution_clock::duration> sleep_time{};
		};
	public:
		nodisc explicit thread(fptr<u32(void*)> fn, void* arg = nullptr)
		{
			_start(fn, arg);
		}
		nodisc explicit thread(fptr<u32(void*)> fn, args<2>* args = nullptr)
		{
			_start(fn, args);
		}
		thread() noexcept : m_data{}
		{}

		~thread() noexcept
		{
			if (m_data.id != nil)
			{
				std::terminate();
			}
		}

		// I am not typing this out, I hate it just as much as the next
		nodisc void keepalive(bool* condition = nullptr, fptr<void()> callback = nullptr, decltype(keepalive_data::sleep_time) time = std::nullopt) noexcept
		{
			m_keepalive.set = true;
			m_keepalive.conditional = condition;
			m_keepalive.callback = callback;
			m_keepalive.sleep_time = time;
		}

		nodisc void start(bool join = {})
		{
			if (m_data.id == nil)
			{
				return;
			}
			else
			{
				if (join)
				{
					if (m_data.id == _Thrd_id())
					{
						return;
					}
					else
					{
						_Thrd_join({ m_data.handle, m_data.id }, nil);
					}
				}
			}
		}

		nodisc void set_id(u32 value) noexcept
		{
			m_data.id = value;
		}

		nodisc u32 get_id() noexcept
		{
			return m_data.id;
		}

		nodisc void* handle()
		{
			return m_data.handle;
		}

	private:
		void _start(fptr<u32(void*)> fn, void* arg)
		{
			args<4> thread_args{};
			thread_args.set_arg(0, &fn);
			thread_args.set_arg(1, arg);
			thread_args.set_arg(2, &m_keepalive);
			thread_args.set_arg(3, this);
			fptr<u32(void*)> thread_callback = [](void* a) -> u32 {
				args<4>* args{ reinterpret_cast<decltype(args)>(a) };
				fptr<u32(void*)> function{ *reinterpret_cast<decltype(function)*>(args->get_arg(0)) };
				void* function_arg{ args->get_arg(1) };
				keepalive_data* keepalive{ reinterpret_cast<keepalive_data*>(args->get_arg(2)) };
				thread* _this{ reinterpret_cast<thread*>(args->get_arg(3)) };
				if (!function)
				{
					_endthreadex(0);
					return 0;
				}
				u32 result{ function(function_arg) };
				if (keepalive && keepalive->set)
				{
					bool conditional{ keepalive->conditional ? *keepalive->conditional : true };
					while (conditional)
					{
						if (keepalive->callback)
						{
							keepalive->callback();
						}
						if (keepalive->sleep_time.has_value())
						{
							std::this_thread::sleep_for(keepalive->sleep_time.value());
						}
					}
				}
				if (_this)
				{
					_this->set_id(0);
				}
				_endthreadex(0);
				return 0;
			};
			m_data.handle = reinterpret_cast<void*>(_beginthreadex(nilptr, nil, thread_callback, &thread_args, nil, &m_data.id));
			if (!m_data.handle)
			{
				m_data.id = 0;
			}
		}
		void _start_debug(fptr<u32(void*)> fn, void* arg)
		{
			args<4> thread_args{};
			thread_args.set_arg(0, fn);
			thread_args.set_arg(1, arg);
			thread_args.set_arg(2, &m_keepalive);
			thread_args.set_arg(3, this);
			fptr<u32(void*)> thread_callback = [](void* a) -> u32 {
				LOG_TO_STREAM("Thread callback called.");
				args<4>* args{ reinterpret_cast<decltype(args)>(a) };
				LOG_TO_STREAM("Set args.");
				fptr<u32(void*)> function{ reinterpret_cast<decltype(function)>(args->get_arg(0)) };
				LOG_TO_STREAM("Set function.");
				void* function_arg{ args->get_arg(1) };
				LOG_TO_STREAM("Set function_arg.");
				keepalive_data* keepalive{ reinterpret_cast<keepalive_data*>(args->get_arg(2)) };
				LOG_TO_STREAM("Set keepalive.");
				thread* _this{ reinterpret_cast<thread*>(args->get_arg(3)) };
				LOG_TO_STREAM("Set _this.");
				if (!function)
				{
					_endthreadex(0);
					return 0;
				}
				u32 result{ function(function_arg) };
				LOG_TO_STREAM("Called callback, result is " << result << ".");
				if (keepalive && keepalive->set)
				{
					bool conditional{ keepalive->conditional ? *keepalive->conditional : true };
					while (conditional)
					{
						if (keepalive->callback)
						{
							keepalive->callback();
						}
						if (keepalive->sleep_time.has_value())
						{
							std::this_thread::sleep_for(keepalive->sleep_time.value());
						}
					}
				}
				if (_this)
				{
					_this->set_id(0);
				}
				LOG_TO_STREAM("Deallocated thread and returned 0.");
				_endthreadex(0);
				return 0;
			};
			m_data.handle = reinterpret_cast<void*>(_beginthreadex(nilptr, nil, thread_callback, &thread_args, nil, &m_data.id));
			if (!m_data.handle)
			{
				m_data.id = 0;
			}
		}
		struct data
		{
			void* handle;
			u32 id;
		};

		data m_data;
		keepalive_data m_keepalive{};
	};
}