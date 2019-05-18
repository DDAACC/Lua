#pragma once
#include "Include.h"

class LuaMgr
{
public:
	LuaMgr() {};
	void Init();
	bool Reload();
	bool LoadScript(const char* pszFileName);
	void DoString(const char* pszLuaString, int nLength, const char* pszFileName);
	void InitGlobalFunction();
	void CallTableFunction(const char* pszTableName, const char* pszFunName, const char* pszFormat, ...);
	void CallGlobalFunction(const char* pszFunName, const char* pszFormat, ...);
	lua_State* GetLuaState() { return L; };
private:
	void RegisterLuaClass();
	void _Execute(int nParams, int nResults);
	lua_State* L;
	typedef std::map<std::string, lua_CFunction> GlobalFunctionTable;
	GlobalFunctionTable m_MapGlobalFunction;
};

extern LuaMgr* g_LuaMgr;