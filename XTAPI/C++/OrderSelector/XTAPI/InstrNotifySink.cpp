/// <summary>
/// InstrNotifySink.cpp: implementation of the CInstrNotifySink class.
/// </summary>

#include "../stdafx.h"
#include "InstrNotifySink.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/// <summary>
/// CInstrNotifySink
/// </summary>
CInstrNotifySink::CInstrNotifySink(CInstrNotifySink* pCallback)
{
	m_pCallback=pCallback;

	HRESULT hr=m_pInstrNotifyObj.CreateInstance(__uuidof(XTAPI::TTInstrNotify));
	if (hr==S_OK)
	{
		hr=DispEventAdvise(m_pInstrNotifyObj);
	}
}
CInstrNotifySink::~CInstrNotifySink()
{
	if (m_pInstrNotifyObj)
	{
		DispEventUnadvise(m_pInstrNotifyObj);
	}

	m_pInstrNotifyObj=NULL;
}

HRESULT CInstrNotifySink::OnNotifyFound(XTAPI::ITTInstrNotify* pNotify,XTAPI::ITTInstrObj* pInstr)
{
	m_pCallback->CInstrNotifySink_Found(pInstr);
	return S_OK;
}
HRESULT CInstrNotifySink::OnNotifyUpdate(XTAPI::ITTInstrNotify* pNotify,XTAPI::ITTInstrObj* pInstr)
{
	m_pCallback->CInstrNotifySink_Update(pInstr);
	return S_OK;
}
