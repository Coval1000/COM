#if !defined Agent_H_
#define Agent_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ComServerWP3TypeInfo_h.h"
#include <string>
#include <memory>
class Agent :
	public IDummy,
	public IMesh
{
public:
	Agent();
	virtual ~Agent();

	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, void** pIFace);
	STDMETHODIMP_(DWORD)AddRef();
	STDMETHODIMP_(DWORD)Release();

	// IMesh
	STDMETHODIMP SetData(Data *mesh);
	STDMETHODIMP GetData(Data *mesh);
	STDMETHODIMP SetCell(float *value, int *index);
	STDMETHODIMP GetCell(int *index, float *value);

	//IRemote
	STDMETHODIMP SetHandle(Window *window);
	STDMETHODIMP GetHandle(Window *window);

private:
	DWORD m_refCount;
	Data m_pointsMatrix;
	long m_arraysize;
	Window *hwndRenderTarget;
};

#endif // !defined(AFX_MyCar_H__86A1A88D_D36B_11D2_B8CF_0020781238D4__INCLUDED_)
