// Filename: pixel_shader.fx
// Purpose: Provide a basic pixel blitting technique
// Written by: Vali, 23/05/2024
// Last updated: 04/07/2024
// Copyright: FoxyCheats Inc.
// Expiry: (indef)
#include "common.fx"

Texture2D<float4> texture2d : register(t0);
SamplerState samplerState : register(s0);

float4 PS_ShaderMain(PS_ShaderData input) : SV_TARGET
{
	if (input.ut == 1)
	{
		float4 tex_color = texture2d.Sample(samplerState, input.tx);
		return input.c * tex_color;
	}

	return input.c;
}
