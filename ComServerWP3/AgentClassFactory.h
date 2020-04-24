// the class object (class factory) for CoMyCar class

#if !defined(AgnetClassFactory_H)
#define AgnetClassFactory_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Unknwn.h>
#include <windows.h>

class AgentClassFactory : public IClassFactory
{
public:
	AgentClassFactory();
	virtual ~AgentClassFactory();

	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, void** pIFace);
	STDMETHODIMP_(ULONG)AddRef();
	STDMETHODIMP_(ULONG)Release();

	// IClassFactory
	STDMETHODIMP LockServer(BOOL fLock);
	STDMETHODIMP CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void** ppv);

private:

	ULONG m_refCount;
	void *m_pAgent;
};

#endif