// OrderSetNotifySink.cpp: implementation of the COrderSetNotifySink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OrderSetNotifySink.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// COrderSetNotifySink
//////////////////////////////////////////////////////////////////////
COrderSetNotifySink::COrderSetNotifySink()
{
	// clear out our callback till we pass it in
	m_pCallback=NULL;

	// create the notification object
	HRESULT hr=m_pOrderSetObj.CreateInstance(__uuidof(XTAPI::TTOrderSet));

	if (hr==S_OK)
	{
		// created OK, connect to it
		hr=DispEventAdvise(m_pOrderSetObj);
	}
}
COrderSetNotifySink::~COrderSetNotifySink()
{
	if (m_pOrderSetObj)
	{
		// remove our connection point
		DispEventUnadvise(m_pOrderSetObj);
	}

	// de-ref the SRC
	m_pOrderSetObj=NULL;
}
void COrderSetNotifySink::SetCallback(CNotify*pCallback)
{
	// save the callback pointer
	m_pCallback=pCallback;
}
HRESULT COrderSetNotifySink::OnOrderSetUpdate(XTAPI::ITTOrderSet* pOrderSet)
{
	// Connection point callback function
	if (m_pCallback && m_pCallback->m_bUpdate)
	{
		// pass this onto the external event consumer
		m_pCallback->COrderSetNotifySink_Update(pOrderSet);
	}
	return S_OK;
}
HRESULT COrderSetNotifySink::OnOrderFillData(XTAPI::ITTFillObj* pFillData)
{
	// Connection point callback function
	if (m_pCallback && m_pCallback->m_bFills)
	{
		// pass this onto the external event consumer
		m_pCallback->COrderSetNotifySink_FillData(pFillData);
	}
	return S_OK;
}
HRESULT COrderSetNotifySink::OnOrderFillBlockStart()
{
	// Connection point callback function
	if (m_pCallback && m_pCallback->m_bFills)
	{
		// pass this onto the external event consumer
		m_pCallback->COrderSetNotifySink_FillBlockStart();
	}
	return S_OK;
}
HRESULT COrderSetNotifySink::OnOrderFillBlockEnd()
{
	// Connection point callback function
	if (m_pCallback && m_pCallback->m_bFills)
	{
		// pass this onto the external event consumer
		m_pCallback->COrderSetNotifySink_FillBlockEnd();
	}
	return S_OK;
}
