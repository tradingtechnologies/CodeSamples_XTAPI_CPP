/***************************************************************************
 *    
 *      Copyright (c) 2012 Trading Technologies International, Inc.
 *                     All Rights Reserved Worldwide
 *
 *        * * *   S T R I C T L Y   P R O P R I E T A R Y   * * *
 *
 * WARNING:  This file is the confidential property of Trading Technologies
 * International, Inc. and is to be maintained in strict confidence.  For
 * use only by those with the express written permission and license from
 * Trading Technologies International, Inc.  Unauthorized reproduction,
 * distribution, use or disclosure of this file or any program (or document)
 * derived from it is prohibited by State and Federal law, and by local law
 * outside of the U.S. 
 *
 ***************************************************************************/

#include "../stdafx.h"
#include "OrderSetNotifySink.h"

/* Create the instance of the TTOrderSet
 */
COrderSetNotifySink::COrderSetNotifySink()
{
	// clear out our callback till we pass it in
	m_pCallback = NULL;

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
	m_pOrderSetObj = NULL;
}

void COrderSetNotifySink::SetCallback(CNotify* pCallback)
{
	// save the callback pointer
	m_pCallback = pCallback;
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
