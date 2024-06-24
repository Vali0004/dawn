#pragma once
#include "memory/pointers.h"
#include "hooking/renderer.h"
#include "exception_handler/handler.h"
#include <memory>
// I might have to start making this into a C++ file or seperate it, as it's getting too large logic-wise.
// we love macros!!
#define make_hook(nm, fn) cs::hooking::g_##fn = new cs::hooking::etc::hook<cs::pointers::types::fn##T>(nm, cs::pointers::g_##fn, &cs::hooking::##fn)
#define make_hook_vtbl(nm, vtbl, sz, ...) cs::hooking::g_##nm = new cs::hooking::etc::hook_vtbl(#nm, vtbl, sz); [](cs::hooking::etc::hook_vtbl* _this)##__VA_ARGS__##(cs::hooking::g_##nm); (cs::hooking::g_##nm)->create()
#define make_hook_time_critical(nm, fn) cs::hooking::g_##fn = new cs::hooking::etc::hook<cs::pointers::types::fn##T>(nm, cs::pointers::g_##fn, &cs::hooking::##fn, false)

namespace cs::hooks
{

}