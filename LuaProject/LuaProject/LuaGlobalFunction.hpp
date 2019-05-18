#pragma once
#include "LuaMgr.h"

int lua_Reload(lua_State* L)
{
	g_LuaMgr->Reload();
	return 0;
}

int lua_GetTickCount(lua_State* L)
{
	DWORD	dwTime;
	dwTime = GetTickCount();
	lua_pushnumber(L, dwTime);
	return 1;
}