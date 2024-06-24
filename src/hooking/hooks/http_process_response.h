#pragma once
#include "hooks_base.h"

namespace cs::hooking
{
	inline etc::hook<pointers::types::rlRosHttpTaskProcessResponseT>* g_rlRosHttpTaskProcessResponse{};
	inline bool rlRosHttpTaskProcessResponse(rage::rlRosHttpTask* _this, const char* response, int* resultCode)
	{
		__try
		{
			if (response)
			{
				LOG_TO_STREAM("Response (GAME): " << cs::endl << response);
			}
		}
		__except(exception::handle_exception(GetExceptionInformation()))
		{}

		return g_rlRosHttpTaskProcessResponse->original()(_this, response, resultCode);
	}

	inline etc::hook<pointers::types::rlRosHttpTaskProcessResponseScT>* g_rlRosHttpTaskProcessResponseSc{};
	inline bool rlRosHttpTaskProcessResponseSc(rage::rlRosHttpTask* _this, const char* response, int* resultCode) {
		__try
		{
			if (response)
			{
				LOG_TO_STREAM("Response (SC): " << cs::endl << response);
			}
		}
		__except(exception::handle_exception(GetExceptionInformation()))
		{}

		return g_rlRosHttpTaskProcessResponseSc->original()(_this, response, resultCode);
	}

	inline etc::hook<pointers::types::GameTransactionBaseHttpTaskProcessResponseT>* g_GameTransactionBaseHttpTaskProcessResponse{};
	inline bool GameTransactionBaseHttpTaskProcessResponse(void* _This, const char* response)
	{
		__try
		{
			if (response)
			{
				LOG_TO_STREAM("Response (GameTransaction): " << cs::endl << response);
			}
		}
		__except(exception::handle_exception(GetExceptionInformation()))
		{}

		return g_GameTransactionBaseHttpTaskProcessResponse->original()(_This, response);
	}
}