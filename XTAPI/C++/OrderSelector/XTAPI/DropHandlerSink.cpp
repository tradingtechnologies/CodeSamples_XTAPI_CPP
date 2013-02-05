/// <summary>
/// DropHandlerSink.cpp: implementation of the CDropHandlerSink class.
/// </summary>

#include "../stdafx.h"
#include "DropHandlerSink.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/// <summary>
/// CDropHandlerSink
/// </summary>
CDropHandlerSink::CDropHandlerSink()
{
	// clear out our callback till we pass it in
	m_pCallback=NULL;

	// create the notification object
	HRESULT hr=m_pDropHandlerObj.CreateInstance(__uuidof(XTAPI::TTDropHandler));

	if (hr==S_OK)
	{
		// created OK, connect to it
		hr=DispEventAdvise(m_pDropHandlerObj);
	}
}
CDropHandlerSink::~CDropHandlerSink()
{
	if (m_pDropHandlerObj)
	{
		// remove our connection point
		DispEventUnadvise(m_pDropHandlerObj);
	}

	// de-ref the SRC
	m_pDropHandlerObj=NULL;
}
void CDropHandlerSink::SetCallback(CNotify*pCallback)
{
	// save the callback pointer
	m_pCallback=pCallback;
}
HRESULT CDropHandlerSink::OnNotifyDrop()
{
	// Connection point callback function
	if (m_pCallback)
	{
		// pass this onto the external event consumer
		m_pCallback->CDropHandlerSink_NotifyDrop(this);
	}
	return S_OK;
}