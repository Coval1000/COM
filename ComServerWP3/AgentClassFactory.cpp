#include "stdafx.h"
#include "Agent.h"
#include "AgentClassFactory.h"
#include "locks.h"

AgentClassFactory::AgentClassFactory()
{
	m_refCount = 0;
	m_pAgent = nullptr;
}

AgentClassFactory::~AgentClassFactory()
{
	/*MessageBox(NULL,
		"AgentClassFactory is being distructed. Make sure you see this message, if not, you might have memory leak!",
		"Destructor", MB_OK | MB_SETFOREGROUND);*/
}

STDMETHODIMP_(ULONG) AgentClassFactory::AddRef()
{
	return ++m_refCount;
}

STDMETHODIMP_(ULONG) AgentClassFactory::Release()
{
	if (--m_refCount == 0)
	{
		delete this;
		return 0;
	}
	return m_refCount;
}

STDMETHODIMP AgentClassFactory::QueryInterface(REFIID riid, void** pIFace)
{
	if (riid == IID_IUnknown)
		*pIFace = (IUnknown*)this;
	else if (riid == IID_IClassFactory)
		*pIFace = (IClassFactory*)this;
	else
	{
		*pIFace = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*pIFace))->AddRef();
	return S_OK;
}

STDMETHODIMP AgentClassFactory::LockServer(BOOL fLock)
{
	if (fLock) Lock();
	else UnLock();
	return S_OK;
}

STDMETHODIMP AgentClassFactory::CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void** ppv)
{
	if (pUnkOuter != NULL) return CLASS_E_NOAGGREGATION;

	HRESULT hr;
	if (m_pAgent == nullptr) m_pAgent = new Agent();
	hr = ((Agent*)m_pAgent)->QueryInterface(riid, ppv);
	if (FAILED(hr))
	{ 
		if (hr == E_NOINTERFACE) return hr;
		delete m_pAgent;
		m_pAgent = nullptr;
	}
	return hr;
}
