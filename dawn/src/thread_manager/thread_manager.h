#pragma once
#include "pch/framework.h"
#include "thread.h"

namespace dwn
{
	class thread_manager
	{
	public:
		thread_manager()
		{}
		~thread_manager()
		{
			for (auto& thr : m_threads)
			{
				thr.release();
			}
			m_threads.clear();
		}

		nodisc void add_task(std::function<void()> task)
		{
			if (task)
			{
				m_threads.push_back(std::unique_ptr<thread>(create_thread_wrapper(task)));
				auto& t{ m_threads.back() };
				t->start();
			}
		}

		nodisc void update_thread_status()
		{
			for (u64 i{}; i != m_threads.size(); ++i)
			{
				auto& thr{ m_threads[i] };
				if (thr && !thr->get_id())
				{
					thr.release();
					m_threads.erase(m_threads.begin() + i);
					break;
				}
			}
		}

		auto create_thread(std::function<void(thread*)> callback)
		{
			thread* thr{ create_thread_wrapper(callback) };
			m_threads.push_back(std::unique_ptr<thread>(thr));
			auto& ret{ m_threads.back() };
			ret->start();
			return ret.get();
		}

		auto create_thread(std::function<void()> callback)
		{
			thread* thr{ create_thread_wrapper(callback) };
			m_threads.push_back(std::unique_ptr<thread>(thr));
			auto& ret{ m_threads.back() };
			ret->start();
			return ret.get();
		}

		const bool empty() noexcept
		{
			return m_threads.empty();
		}

	private:
		nodisc thread* create_thread_wrapper(std::function<void()> callback)
		{
			if (!callback)
			{
				return nullptr;
			}
			auto thr = std::make_unique<thread>([](void* a) -> u32 {
				std::function<void()>* callback_ptr{ reinterpret_cast<decltype(callback_ptr)>(a) };
				if (callback_ptr && *callback_ptr)
				{
					(*callback_ptr)();
				}
				return 0;
			}, &callback);
			return thr.get();
		}

		nodisc thread* create_thread_wrapper(std::function<void(thread*)> callback)
		{
			if (!callback)
			{
				return nullptr;
			}
			args<2> thread_args{};
			thread_args.set_arg(0, &callback);
			std::unique_ptr<thread> thr{};
			thread_args.set_arg(1, thr.get());
			thr = std::make_unique<thread>([](void* a) -> u32 {
				args<2>* args{ reinterpret_cast<decltype(args)>(a) };
				std::function<void(thread*)>* function{ reinterpret_cast<decltype(function)>(args->get_arg(0)) };
				thread* _this{ reinterpret_cast<decltype(_this)>(args->get_arg(1)) };
				(*function)(_this);
				return 0;
			}, &thread_args);
			thread_args.set_arg(1, thr.get());
			return thr.get();
		}

		std::vector<std::unique_ptr<thread>> m_threads{};
	};

	inline thread_manager g_thread_manager{};
}