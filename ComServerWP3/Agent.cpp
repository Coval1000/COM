#include "stdafx.h"
#include "Agent.h"
#include <stdio.h> 
#include "locks.h"
#include <iostream>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Agent::Agent() : m_refCount(0)
{
	Lock();
	m_pointsMatrix.points = nullptr;
	m_pointsMatrix.sizeX = 0;
	m_pointsMatrix.sizeY = 0;
	hwndRenderTarget = nullptr;
}

Agent::~Agent()
{
	UnLock();
	/*MessageBox(NULL,
		"Agent is being distructed. Make sure you see this message, if not, you might have memory leak!",
		"Destructor", MB_OK | MB_SETFOREGROUND);*/
}

// IUnknown
STDMETHODIMP Agent::QueryInterface(REFIID riid, void** pIFace)
{
	if (riid == IID_IUnknown)
	{
		*pIFace = (IUnknown*)(IDummy*)this;
	}
	else if (riid == IID_IMesh)
	{
		*pIFace = (IMesh*)this;
	}
	else if (riid == IID_IDummy)
	{
		*pIFace = (IDummy*)this;
	}
	else if (riid == IID_IRemote)
	{
		*pIFace = (IRemote*)this;
	}
	else
	{
		*pIFace = NULL;
		return E_NOINTERFACE;
	}

	((IUnknown*)(*pIFace))->AddRef();
	return S_OK;
}

STDMETHODIMP_(DWORD) Agent::AddRef()
{
	return ++m_refCount;
}

STDMETHODIMP_(DWORD) Agent::Release()
{
	if (--m_refCount == 0)
	{
		delete this;
		return 0;
	}
	else
	{
		return m_refCount;
	}
}

// IMesh
STDMETHODIMP Agent::SetData(Data *mesh)
{
	if (m_pointsMatrix.points)
	{
		delete[] m_pointsMatrix.points;
		m_pointsMatrix.points = nullptr;
	}
	m_pointsMatrix.points = new float[mesh->sizeX * mesh->sizeY];
	if (m_pointsMatrix.points == nullptr) return ERROR_BAD_STACK;
	m_arraysize = mesh->sizeX * mesh->sizeY;
	m_pointsMatrix.sizeX = mesh->sizeX;
	m_pointsMatrix.sizeY = mesh->sizeY;
	return S_OK;
}

STDMETHODIMP Agent::GetData(Data *mesh)
{
	std::shared_ptr<float> f = std::make_shared<float>();
	
	if(m_pointsMatrix.points == nullptr) return ERROR_BAD_STACK;
	if (mesh->points)
	{
		delete[] mesh->points;
		mesh->points = nullptr;
	}
	mesh->points = new float[m_arraysize];
	if (mesh->points == nullptr) return ERROR_BAD_STACK;
	std::memset(mesh->points, 0, sizeof(float) * m_arraysize);
	mesh->sizeX = m_pointsMatrix.sizeX;
	mesh->sizeY = m_pointsMatrix.sizeY;
	return S_OK;
}

STDMETHODIMP Agent::SetCell(float *value, int *index)
{
	if (*index > m_arraysize) return ERROR_BAD_STACK;
	m_pointsMatrix.points[*index] = *value;
	return S_OK;
}

STDMETHODIMP Agent::GetCell(int *index, float *value)
{
	if (*index > m_arraysize) return ERROR_BAD_STACK;
	*value = m_pointsMatrix.points[*index];
	return S_OK;
}

//IRemote
STDMETHODIMP Agent::SetHandle(Window* window)
{
	hwndRenderTarget = window;
	return S_OK;
}

STDMETHODIMP Agent::GetHandle(Window* window)
{
	window = hwndRenderTarget;
	return S_OK;
}

// ICreateAgent