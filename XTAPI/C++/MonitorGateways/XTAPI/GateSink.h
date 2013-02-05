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

#pragma once
#ifndef GATESINK_H
#define GATESINK_H

class CGateSink :
	public IDispEventImpl<1, CGateSink, &XTAPI::DIID__ITTGateEvents, &XTAPI::LIBID_XTAPI, 1, 0>
{
private:
	CGateSink* m_pCallback;
	XTAPI::ITTGatePtr m_pGateObj;

public:
	CGateSink() {}
	CGateSink(CGateSink* pCallback);
	virtual ~CGateSink();

	XTAPI::ITTGatePtr& Obj()
	{
		return m_pGateObj;
	}

	BEGIN_SINK_MAP(CGateSink)
		SINK_ENTRY_EX(1, XTAPI::DIID__ITTGateEvents, 1, OnStatusUpdate)
		SINK_ENTRY_EX(1, XTAPI::DIID__ITTGateEvents, 2, OnExchangeStateUpdate)
		SINK_ENTRY_EX(1, XTAPI::DIID__ITTGateEvents, 3, OnExchangeMessage)
		//SINK_ENTRY_EX(1, XTAPI::DIID__ITTGateEvents, 101, OnLoginFailed)
		//SINK_ENTRY_EX(1, XTAPI::DIID__ITTGateEvents, 102, OnSessionRollMessage)
	END_SINK_MAP()

	HRESULT __stdcall OnStatusUpdate(long lHintMask, BSTR sText);
	HRESULT __stdcall OnExchangeStateUpdate(BSTR sExchange, BSTR sText, long bOpened, long bServerUp);
	HRESULT __stdcall OnExchangeMessage(BSTR sExchange, BSTR sTimeStamp, BSTR sInfo, BSTR sText);
	//HRESULT __stdcall OnLoginFailed(BSTR sAdminExchange, BSTR sMember, BSTR sGroup, BSTR sTrader, BSTR sMsg, XTAPI::enumLoginSource eLoginSrc);
	//HRESULT __stdcall OnSessionRollMessage(BSTR sExchange, BSTR sMessage, XTAPI::enumSessionRollState eMessage);

protected:
	virtual void CGateSink_OnStatusUpdate(long lHintMask, BSTR sText) {};
	virtual void CGateSink_OnExchangeStateUpdate(BSTR sExchange, BSTR sText, long bOpened, long bServerUp) {};
	virtual void CGateSink_OnExchangeMessage(BSTR sExchange, BSTR sTimeStamp, BSTR sInfo, BSTR sText) {};
	//virtual void CGateSink_OnLoginFailed(BSTR sAdminExchange, BSTR sMember, BSTR sGroup, BSTR sTrader, BSTR sMsg, XTAPI::enumLoginSource eLoginSrc) {};
	//virtual void CGateSink_OnSessionRollMessage(BSTR sExchange, BSTR sMessage, XTAPI::enumSessionRollState eMessage) {};
};

#endif