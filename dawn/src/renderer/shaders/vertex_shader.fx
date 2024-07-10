// Filename: vertex_shader.fx
// Purpose: Provide a basic way to handle and map verticies
// Written by: Vali, 02/07/2024
// Last updated: 04/07/2024
// Copyright: FoxyCheats Inc.
// Expiry: (indef)
#include "common.fx"

PS_ShaderData VS_ShaderMain(VS_ShaderData input)
{
	PS_ShaderData ps_data;
	ps_data.p = float4(input.p, 0.f, 1.f);
	ps_data.c = input.c;
	ps_data.tx = input.tx;
	ps_data.ut = input.ut;
	return ps_data;
}
