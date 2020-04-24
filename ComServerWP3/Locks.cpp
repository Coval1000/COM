#include "stdafx.h"
extern DWORD g_allLocks;

// Called by cocar ctor and LockServer(TRUE)
void Lock()
{
	++g_allLocks;
}

// Called by cocar dtor and LockServer(FALSE)
void UnLock()
{
	--g_allLocks;

	if (g_allLocks == 0)
		PostQuitMessage(0);
}
