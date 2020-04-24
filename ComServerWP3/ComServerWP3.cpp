#include "stdafx.h"
#include "ComServerWP3TypeInfo_i.c"
#include "AgentClassFactory.h"	
#include <iostream>	

#include <atlbase.h>
#include "resource.h"
#include <string.h>

// global counter for locks & active objects.
DWORD g_allLocks;
// for showing the possible mistakes
void ShowErrorMessage(LPCTSTR header, HRESULT);

void RegisterServer(wchar_t* module, bool reg)
{
	CRegObject ro;
	ro.FinalConstruct(); // line needed in newer versions of Visual Studio otherwise assertion failure occurs
	ro.AddReplacement(L"Module", module);
	reg ? ro.ResourceRegister(module, IDR_REGISTRY, L"REGISTRY") :
		ro.ResourceUnregister(module, IDR_REGISTRY, L"REGISTRY");
}

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	// Init COM.
	HRESULT hr;
	/*MessageBox(NULL, "Agent Server [LOCAL] is initilizing", "[LOCAL]EXE Message!", MB_OK | MB_SETFOREGROUND);*/
	CoInitialize(NULL);

	// Let's register the type lib to get the 'free' type library marsahler.
	ITypeLib* pTLib = NULL;
	hr = LoadTypeLibEx(L"x64\\Release\\ComServerWP3.tlb", REGKIND_REGISTER, &pTLib);

	if (FAILED(hr))
	{
		ShowErrorMessage("LoadTypeLibEx()", hr);
		exit(1);
	}
	pTLib->Release();

	if (lpCmdLine) 
	{
		if (!_stricmp(lpCmdLine, "-RegServer") || !_stricmp(lpCmdLine, "/RegServer"))
		{
			char ansiPath[MAX_PATH];
			GetModuleFileName(hInstance, ansiPath, MAX_PATH);
			wchar_t module[MAX_PATH];
			//MultiByteToWideChar(CP_ACP, 0, ansiPath, lstrlen(ansiPath) + 1, module, MAX_PATH);
			RegisterServer(module, true);
			CComPtr<ITypeLib> pTypeLib;
			LoadTypeLib(module, &pTypeLib);
			HRESULT hr(RegisterTypeLib(pTypeLib, module, 0));
			return hr ? SELFREG_E_TYPELIB : S_OK;
		}
		if (!_stricmp(lpCmdLine, "-UnregServer") || !_stricmp(lpCmdLine, "/UnregServer"))
		{
			char ansiPath[MAX_PATH];
			GetModuleFileName(hInstance, ansiPath, MAX_PATH);
			wchar_t module[MAX_PATH];
			//MultiByteToWideChar(CP_ACP, 0, ansiPath, lstrlen(ansiPath) + 1, module, MAX_PATH);			
			RegisterServer(module, false);
			HRESULT hr(UnRegisterTypeLib(LIBID_ComServerWP3Lib, 1, 0, 0, SYS_WIN32));
			return hr ? SELFREG_E_TYPELIB : S_OK;
		}
	}


	// Let's see if we were started by SCM.
	if (strstr(lpCmdLine, "/Embedding") || strstr(lpCmdLine, "-Embedding"))
	{
		/*MessageBox(NULL, "Agent Server[LOCAL] is registering the classes", "[LOCAL]EXE Message!", MB_OK | MB_SETFOREGROUND);*/

		AgentClassFactory agentClassFactory;

		DWORD regID = 0;
		hr = CoRegisterClassObject(CLSID_Agent, (IClassFactory*)&agentClassFactory,
			CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &regID);
		if (FAILED(hr))
		{
			ShowErrorMessage("CoRegisterClassObject()", hr);
			CoUninitialize();
			exit(1);
		}

		MSG ms;
		while (GetMessage(&ms, 0, 0, 0))
		{
			TranslateMessage(&ms);
			DispatchMessage(&ms);
		}

		CoRevokeClassObject(regID);
	}

	// Terminate COM.
	CoUninitialize();
	/*MessageBox(NULL, "[LOCAL] Server is dying", "[LOCAL]EXE Message!", MB_OK | MB_SETFOREGROUND);*/
	return 0;
}

void ShowErrorMessage(LPCTSTR header, HRESULT hr)
{
	void* pMsg;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&pMsg, 0, NULL);

	std::cout << header << ": Error(" << std::hex << hr << "): "
		<< (LPTSTR)pMsg << std::endl;

	LocalFree(pMsg);
}

