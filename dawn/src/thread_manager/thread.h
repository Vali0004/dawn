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

        struct data
        {
            struct
            {
                fptr<u32(void*)> fn;
                void* arg;
            } callback;
            void* handle;
            u32 id;
        };
    public:
        nodisc explicit thread(fptr<u32(void*)> fn, void* arg = nullptr)
        {
            _start_debug(fn, arg);
        }
        nodisc explicit thread(fptr<u32(void*)> fn, args<2>* args = nullptr)
        {
            _start_debug(fn, args);
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

        data m_data{};
        keepalive_data m_keepalive{};
    private:
        void _start(fptr<u32(void*)> fn, void* arg)
        {
            m_data.callback.fn = fn;
            m_data.callback.arg = arg;
            fptr<u32(void*)> thread_callback = [](void* a) -> u32 {
                thread* _this{ reinterpret_cast<thread*>(a) };
                fptr<u32(void*)> function{ _this->m_data.callback.fn };
                void* function_arg{ _this->m_data.callback.arg };
                keepalive_data* keepalive{ reinterpret_cast<keepalive_data*>(&_this->m_keepalive) };
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
                        conditional = keepalive->conditional ? *keepalive->conditional : true;
                    }
                }
                if (_this)
                {
                    _this->set_id(0);
                }
                _endthreadex(0);
                return 0;
            };
            m_data.handle = reinterpret_cast<void*>(_beginthreadex(nilptr, nil, thread_callback, this, nil, &m_data.id));
            if (!m_data.handle)
            {
                m_data.id = 0;
            }
        }

        void _start_debug(fptr<u32(void*)> fn, void* arg)
        {
            m_data.callback.fn = fn;
            m_data.callback.arg = arg;
            fptr<u32(void*)> thread_callback = [](void* a) -> u32 {
                thread* _this{ reinterpret_cast<thread*>(a) };
                LOG_TO_STREAM("Set _this.");
                fptr<u32(void*)> function{ _this->m_data.callback.fn };
                void* function_arg{ _this->m_data.callback.arg };
                keepalive_data* keepalive{ reinterpret_cast<keepalive_data*>(&_this->m_keepalive) };
                LOG_TO_STREAM("Set all data from this.");
                if (!function)
                {
                    _endthreadex(0);
                    return 0;
                }
                u32 result{ function(function_arg) };
                if (g_running)
                {
                    LOG_TO_STREAM("Called callback, result is " << result << ".");
                }
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
                        conditional = keepalive->conditional ? *keepalive->conditional : true;
                    }
                }
                if (_this)
                {
                    _this->set_id(0);
                }
                if (g_running)
                {
                    LOG_TO_STREAM("Deallocated thread and returned 0.");
                }
                _endthreadex(0);
                return 0;
            };
            m_data.handle = reinterpret_cast<void*>(_beginthreadex(nilptr, nil, thread_callback, this, nil, &m_data.id));
            if (!m_data.handle)
            {
                m_data.id = 0;
            }
        }
    };
}