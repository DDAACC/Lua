#include "File.h"
#include "LuaGlobalFunction.hpp"

#define RegisterGlobalFunction(name, pFunction)\
	m_MapGlobalFunction[#name] = pFunction;\

LuaMgr* g_LuaMgr = NULL;

int LuaError(lua_State * L)
{
	lua_getglobal(L, "debug");
	if (!lua_istable(L, -1)) {
		lua_settop(L, 1);
		return 1;
	}
	lua_getfield(L, -1, "traceback");
	if (!lua_isfunction(L, -1)) {
		lua_settop(L, 1);
		return 1;
	}
	lua_pushvalue(L, 1);
	lua_pushinteger(L, 2);
	lua_call(L, 2, 1);
	return 1;
}

void LuaMgr::InitGlobalFunction()
{
	for (auto iter = m_MapGlobalFunction.begin(); iter != m_MapGlobalFunction.end(); ++iter)
	{
		lua_CFunction pPushFuction = iter->second;
		const char* pszFunctionName = iter->first.c_str();
		lua_pushcfunction(L, pPushFuction);
		lua_setglobal(L, pszFunctionName);
	}
}

void LuaMgr::_Execute(int nParams, int nResults)
{
	int nFunIdx = lua_gettop(L) - nParams;

	lua_pushcfunction(L, LuaError);
	lua_insert(L, nFunIdx);
	lua_pcall(L, nParams, nResults, nFunIdx);
	lua_remove(L, nFunIdx);
}

void LuaMgr::CallTableFunction(const char * pszTableName, const char * pszFunName, const char * pszFormat, ...)
{
	int nArgNums = 0;

	lua_getglobal(L, pszTableName);
	lua_getfield(L, -1, pszFunName);

	lua_insert(L, -2);
	va_list vlist;
	va_start(vlist, pszFormat);
	while (*pszFormat)
	{

		switch (*pszFormat++)
		{
		case's':lua_pushstring(L, va_arg(vlist, char*)); break;
		case'd':lua_pushnumber(L, va_arg(vlist, int)); break;
		case'n':lua_pushnumber(L, va_arg(vlist, int)); break;
		default:
			break;
		}
		nArgNums++;
	}
	va_end(vlist);
	_Execute(nArgNums + 1, 0);
}

void LuaMgr::CallGlobalFunction(const char* pszFunName, const char* pszFormat, ...)
{
	int nArgNums = 0;
	lua_getglobal(L, pszFunName);

	va_list vlist;
	va_start(vlist, pszFormat);
	while (*pszFormat)
	{

		switch (*pszFormat++)
		{
		case's':lua_pushstring(L, va_arg(vlist, char*)); break;
		case'd':lua_pushnumber(L, va_arg(vlist, int)); break;
		case'n':lua_pushnumber(L, va_arg(vlist, int)); break;
		default:
			break;
		}
		nArgNums++;
	}
	va_end(vlist);
	_Execute(nArgNums, 0);
}

void LuaMgr::Init()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	RegisterLuaClass();

	RegisterGlobalFunction(Reload, lua_Reload);
	RegisterGlobalFunction(GetTickCount, lua_GetTickCount);

	InitGlobalFunction();
	Reload();
}

void LuaMgr::DoString(const char* pszLuaString, int nLength, const char* pszFileName)
{
	luaL_loadbuffer(L, pszLuaString, nLength, pszFileName);
	if (!lua_isfunction(L, -1))
	{
		return;
	}
	int nTop = lua_gettop(L);
	lua_pushcfunction(L, LuaError);
	lua_insert(L, nTop);
	int nRet = lua_pcall(L, 0, 0, nTop);
	lua_remove(L, nTop);

	if (nRet == 0)
		return;

	const char* pszMsg = lua_tostring(L, -1);
	if (!pszMsg)
	{
		printf("error occured! in DoString\n");
		return;
	}
	printf("%s\n", pszMsg);
	lua_pop(L, 1);
}

bool LuaMgr::LoadScript(const char* pszFileName)
{
	int nFileSize;
	unsigned char* pFileBuffer;

	pFileBuffer = LoadFile(pszFileName, &nFileSize);
	DoString((const char*)pFileBuffer, nFileSize, pszFileName);

	delete pFileBuffer;
	return true;
}

bool LuaMgr::Reload()
{
	LoadScript("Lua/main.lua");
	return true;
}

void LuaMgr::RegisterLuaClass()
{
	
}