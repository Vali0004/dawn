#pragma once
#include "hooks_base.h"
#include "helpers/ragesec.h"
// FUCK YOU PATRICK GOLDMEN YOUR CODE FUCKING SUCKS I HOPE YOUR FAMILY FUCKING DIES IN A FIRE

namespace cs::hooking {
	inline etc::hook_vtbl* g_gameSkeleton{};
	inline void gameSkeletonInit(rage::gameSkeleton* _This, u32 initMode) {
		const rage::gameSkeleton::mode* theMode = _This->GetMode(_This->m_InitModes, initMode);

		if (theMode) {
			const rage::gameSkeleton::dependency* currDep = theMode->head;

			while (currDep) {
				int numSystems = currDep->sysData.size();
				if (numSystems > 0) {
					for (int systemIndex{}; systemIndex < numSystems; systemIndex++) {
						int index = systemIndex;
						if (currDep->sysData[index] < static_cast<unsigned>(_This->m_RegisteredSystemData.size())) {
							const rage::gameSkeleton::systemData& sysData = _This->m_RegisteredSystemData[currDep->sysData[index]];
							if (!ragesec::invalid_hash(sysData.m_SystemName.m_hash)) {
								(*sysData.m_InitFunction)(initMode);
							}
						}
					}
				}

				currDep = currDep->next;
			}
		}
	}

	inline void gameSkeletonUpdate(rage::gameSkeleton* _This, u32 updateModeType) {
		rage::gameSkeleton::updateMode* updateMode = _This->m_UpdateModes;
		rage::gameSkeleton::updateBase* updateBase = updateMode->m_Head;

		while (updateMode && updateMode->m_ModeType != updateModeType) {
			updateMode = updateMode->m_Next;
		}

		if (!ragesec::invalid_hash(updateBase->m_Name.m_hash)) {
			if (updateMode) {
				updateMode->Update();
			}
		}
	}
}