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
#include "GateSink.h"

/* Create the instance of the TTGate object
 */
CGateSink::CGateSink(CGateSink* pCallback)
{
	m_pCallback = pCallback;

	HRESULT hr = m_pGateObj.CreateInstance(__uuidof(XTAPI::TTGate));
	if (hr == S_OK)
	{
		hr = DispEventAdvise(m_pGateObj);
	}
}

/* Destroy the TTGate object 
 */
CGateSink::~CGateSink()
{
	if (m_pGateObj)
	{
		DispEventUnadvise(m_pGateObj);
	}

	m_pGateObj = NULL;
}

/* Assign the OnStatusUpdate event
 */
HRESULT CGateSink::OnStatusUpdate(long lHintMask, BSTR sText)
{
	m_pCallback->CGateSink_OnStatusUpdate(lHintMask, sText);
	return S_OK;
}

/* Assign the OnExchangeStateUpdate event
 */
HRESULT CGateSink::OnExchangeStateUpdate(BSTR sExchange, BSTR sText, long bOpened, long bServerUp)
{
	m_pCallback->CGateSink_OnExchangeStateUpdate(sExchange, sText, bOpened, bServerUp);
	return S_OK;
}

/* Assign the OnExchangeMessage event
 */
HRESULT CGateSink::OnExchangeMessage(BSTR sExchange, BSTR sTimeStamp, BSTR sInfo, BSTR sText)
{
	m_pCallback->CGateSink_OnExchangeMessage(sExchange, sTimeStamp, sInfo, sText);
	return S_OK;
}

/* Assign the OnLoginFailed event
 */
//HRESULT CGateSink::OnLoginFailed(BSTR sAdminExchange, BSTR sMember, BSTR sGroup, BSTR sTrader, BSTR sMsg, XTAPI::enumLoginSource eLoginSrc)
//{
//	m_pCallback->CGateSink_OnLoginFailed(BSTR sAdminExchange, BSTR sMember, BSTR sGroup, BSTR sTrader, BSTR sMsg, XTAPI::enumLoginSource eLoginSrc);
//	return S_OK;
//}

/* Assign the OnSessionRollMessage event
 */
//HRESULT CGateSink::OnSessionRollMessage(BSTR sExchange, BSTR sMessage, XTAPI::enumSessionRollState eMessage)
//{
//	m_pCallback->CGateSink_OnSessionRollMessage(BSTR sExchange, BSTR sMessage, XTAPI::enumSessionRollState eMessage);
//	return S_OK;
//}