#pragma once
#include "memory/pointers.h"
#include "renderer/renderer.h"
#include "hooking/etc/detour.h"
#include "hooking/etc/vmt.h"
#include "exception_handler/handler.h"
#include <memory>
// I might have to start making this into a C++ file or seperate it, as it's getting too large logic-wise.
// we love macros!!
#define make_hook(nm, fn) dwn::hooking::g_##fn = new dwn::hooking::etc::hook<dwn::pointers::types::fn##T>(nm, dwn::pointers::g_##fn, &dwn::hooking::##fn)
#define make_hook_vtbl(nm, vtbl, sz, ...) dwn::hooking::g_##nm = new dwn::hooking::etc::hook_vtbl(#nm, vtbl, sz); [](dwn::hooking::etc::hook_vtbl* _this)##__VA_ARGS__##(dwn::hooking::g_##nm); (dwn::hooking::g_##nm)->create()
#define make_hook_time_critical(nm, fn) dwn::hooking::g_##fn = new dwn::hooking::etc::hook<dwn::pointers::types::fn##T>(nm, dwn::pointers::g_##fn, &dwn::hooking::##fn, false)

namespace dwn::hooks
{

}