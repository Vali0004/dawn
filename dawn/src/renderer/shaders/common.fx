// Filename: common.fx
// Purpose: Unify macros and techniques between shaders.
// Written by Vali, 23/05/2024
// Last updated: 04/07/2024
// Copyright: FoxyCheats Inc.
// Expiry: (indef)

// Shader type defines
#define __DIRECTX_BUILD 1
// This is planned, and so is DirectX 9
#define __OPENGL 0
#if __DIRECTX_BUILD
	// These will probably need to be made preprocessor in the future. Oh well
	#define __DIRECTX_VERSION 11
#endif
// Internal shader defines
#define Stringify(x) #x
#define Append(a, b) a##b
#define spc " "
// Shader helper defines
#ifdef __DIRECTX_BUILD
	#ifdef __DIRECTX_VERSION
		#if __DIRECTX_VERSION == 11 || __DIRECTX_VERSION == 10 
			#define Color(varname) float4 varname : COLOR
			#define PositionImpl(type, varname, sv) Append(float, type) varname : Append(sv, ITION)
			#define Position(varname) PositionImpl(2, varname, POS)
			#define SVPosition(varname) PositionImpl(4, varname, SV_POS)
			#define TexCoord(varname) float2 varname : TEXCOORD
			#define UseTexture(varname) uint varname : TEXFLAG
		#else
			#error "DirectX Build is set without a valid DirectX Version!" spc"DirectX"spc Stringify(x) spc"is unsupported!"
		#endif
	#else
		#error "DirectX Build is set without a DirectX Version!"
	#endif
#endif
// Shader structures
struct PS_ShaderData
{
	SVPosition(p);
	Color(c);
	TexCoord(tx);
	UseTexture(ut);
};
struct VS_ShaderData
{
	Position(p);
	Color(c);
	TexCoord(tx);
	UseTexture(ut);
};
