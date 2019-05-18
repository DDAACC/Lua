#include "Include.h"
#include "LuaMgr.h"

int main()
{
	g_LuaMgr = new LuaMgr();
	g_LuaMgr->Init();
	int nLength;

	while (1)
	{
		std::string line;
		getline(std::cin, line);
		const char* str = line.c_str();
		nLength = strlen(str);
		g_LuaMgr->DoString(str, nLength, "cmdInput");
	}
	return 0;
}