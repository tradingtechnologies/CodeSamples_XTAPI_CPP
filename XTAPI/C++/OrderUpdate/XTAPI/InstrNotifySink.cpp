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
#include "InstrNotifySink.h"

/* Create the instance of the TTInstrNotify object
 */
CInstrNotifySink::CInstrNotifySink(CInstrNotifySink* pCallback)
{
	// Save the pointer of the calling class.
	m_pCallback = pCallback;

	// Create an instance of the TTInstrNotify object.
	HRESULT hr=m_pInstrNotifyObj.CreateInstance(__uuidof(XTAPI::TTInstrNotify));
	if (hr==S_OK)
	{
		// Advise the object for events.
		hr=DispEventAdvise(m_pInstrNotifyObj);
	}
}

/* Destroy the TTInstrNotify object 
 */
CInstrNotifySink::~CInstrNotifySink()
{
	if (m_pInstrNotifyObj)
	{
		DispEventUnadvise(m_pInstrNotifyObj);
	}

	m_pInstrNotifyObj=NULL;
}

/* Assign the OnNotifyFound event
 */
HRESULT CInstrNotifySink::OnNotifyFound(XTAPI::ITTInstrNotify* pNotify,XTAPI::ITTInstrObj* pInstr)
{
	// Call the derived class function to service the callback.
	m_pCallback->CInstrNotifySink_Found(pInstr);
	return S_OK;
}

/* Assign the OnNotifyUpdate event
 */
HRESULT CInstrNotifySink::OnNotifyUpdate(XTAPI::ITTInstrNotify* pNotify,XTAPI::ITTInstrObj* pInstr)
{
	// Call the derived class function to service the callback.
	m_pCallback->CInstrNotifySink_Update(pInstr);
	return S_OK;
}