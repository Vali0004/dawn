#pragma once
#include "pch/framework.h"
#include "commands/list.h"
#include "memory/pointers.h"
#include "util.h"

namespace dwn::features
{
	struct features
	{
		struct no_clip
		{
			float m_speed{ 2.f };
			bool m_toggle{};
			operator bool() { return m_toggle; }
		} m_no_clip{};
	} g_features{};

	inline void no_clip()
	{
		if (g_features.m_no_clip)
		{
			Ped ped{ PLAYER::PLAYER_PED_ID() };
			Entity entity{ ped };
			if (PED::IS_PED_IN_ANY_VEHICLE(ped, false))
			{
				entity = PED::GET_VEHICLE_PED_IS_IN(ped, false);
			}

			Vector3 rotation{ CAM::GET_GAMEPLAY_CAM_ROT(0) };

			Vector3 playerPosition{ ENTITY::GET_ENTITY_COORDS(ped, true) };
			Vector3 cameraRotation{ util::math::rotation_to_direction(rotation) };

			if (PED::IS_PED_IN_ANY_VEHICLE(ped, false))
			{
				VEHICLE::SET_VEHICLE_FIXED(entity);
				ENTITY::SET_ENTITY_ROTATION(entity, rotation.x, rotation.y, rotation.z, 1, false);
			}

			Vector3 coords{ playerPosition + cameraRotation * g_features.m_no_clip.m_speed };
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, playerPosition, true, true, false);

			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 21)) // ControlSprint
			{
				coords = { playerPosition + cameraRotation * g_features.m_no_clip.m_speed * 3.f };
			}

			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 32)) // ControlMoveUpOnly
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, coords, true, true, false);
			}
		}
	}

	inline void on_tick()
	{
		no_clip();
	}
}