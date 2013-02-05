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
#ifndef INSTRNOTIFYSINK_H
#define INSTRNOTIFYSINK_H

/* Sink class for receiving events through the TTInstrNotify object
 */
class CInstrNotifySink :
	// Inherit the ATL class that implements the IDispatch methods.
	public IDispEventImpl<1, CInstrNotifySink, &XTAPI::DIID__ITTInstrNotifyEvents, &XTAPI::LIBID_XTAPI,1,0>	
{
private:
	CInstrNotifySink* m_pCallback;					// notification class pointer
	XTAPI::ITTInstrNotifyPtr m_pInstrNotifyObj;		// smart pointer to the notification object

public:
	CInstrNotifySink() {}
	CInstrNotifySink(CInstrNotifySink* pCallback);
	virtual ~CInstrNotifySink();

	XTAPI::ITTInstrNotifyPtr& Obj()					
	{ 
		// allow direct access to our underlying COM object
		return m_pInstrNotifyObj;
	}

	// define the ATL connection point event functions
	BEGIN_SINK_MAP(CInstrNotifySink)
		SINK_ENTRY_EX(1,XTAPI::DIID__ITTInstrNotifyEvents, 1,OnNotifyFound)
		SINK_ENTRY_EX(1,XTAPI::DIID__ITTInstrNotifyEvents, 2,OnNotifyUpdate)
		SINK_ENTRY_EX(1,XTAPI::DIID__ITTInstrNotifyEvents, 3,OnNotifyDepthData)
	END_SINK_MAP()

	// connection point callback functions
    HRESULT __stdcall OnNotifyFound(XTAPI::ITTInstrNotify* pNotify,XTAPI::ITTInstrObj* pInstr);
	HRESULT __stdcall OnNotifyUpdate(XTAPI::ITTInstrNotify* pNotify,XTAPI::ITTInstrObj* pInstr);
	HRESULT __stdcall OnNotifyDepthData(XTAPI::ITTInstrNotify* pNotify,XTAPI::ITTInstrObj* pInstr);

protected:
	// callback functions
	virtual void CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr)	{};
	virtual void CInstrNotifySink_Update(XTAPI::ITTInstrObj* pInstr) {};
	virtual void CInstrNotifySink_Depth(XTAPI::ITTInstrObj* pInstr)	{};	
};

#endif /* INSTRNOTIFYSINK_H */