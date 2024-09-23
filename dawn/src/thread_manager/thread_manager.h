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
                delete thr;
            }
            m_threads.clear();
        }

        nodisc void add_task(std::function<void()> task)
        {
            if (task)
            {
                m_threads.push_back(create_thread_wrapper(task));
                thread* t{ m_threads.back() };
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
                    delete thr;
                    m_threads.erase(m_threads.begin() + i);
                    break;
                }
            }
        }

        thread* create_thread(std::function<void(thread*)> callback)
        {
            thread* thr{ create_thread_wrapper(callback) };
            m_threads.push_back(thr);
            thread* ret{ m_threads.back() };
            ret->start();
            return ret;
        }

        thread* create_thread(std::function<void()> callback)
        {
            thread* t{ create_thread_wrapper(callback) };
            m_threads.push_back(t);
            thread* ret{ m_threads.back() };
            ret->start();
            return ret;
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
            thread* thr = new thread([](void* cb) -> u32 {
                std::function<void()>* callback_ptr{ reinterpret_cast<decltype(callback_ptr)>(cb) };
                if (callback_ptr && *callback_ptr)
                {
                    (*callback_ptr)();
                }
                return 0;
            }, &callback);
            return thr;
        }

        nodisc thread* create_thread_wrapper(std::function<void(thread*)> callback)
        {
            if (!callback)
            {
                return nullptr;
            }
            args<2> thread_args{};
            thread_args.set_arg(0, &callback);
            thread* thr{};
            thread_args.set_arg(1, thr);
            thr = new thread([](void* a) -> u32 {
                args<2>* args{ reinterpret_cast<decltype(args)>(a) };
                std::function<void(thread*)>* function{ reinterpret_cast<decltype(function)>(args->get_arg(0)) };
                if (!function)
                {
                    return 0;
                }
                thread* _this{ reinterpret_cast<decltype(_this)>(args->get_arg(1)) };
                (*function)(_this);
                return 0;
            }, &thread_args);
            thread_args.set_arg(1, thr);
            return thr;
        }

        std::vector<thread*> m_threads{};
    };

    inline thread_manager g_thread_manager{};
}