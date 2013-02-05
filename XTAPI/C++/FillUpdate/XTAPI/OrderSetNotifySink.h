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
#ifndef ORDERSETNOTIFYSINK_H
#define ORDERSETNOTIFYSINK_H
#define	ORDERSETNOTIFYSINK	1

class COrderSetNotifySink 
:
	// derive a ATL event handler class for our order set object
	public IDispEventImpl
	<
		ORDERSETNOTIFYSINK, 
		COrderSetNotifySink,
		&XTAPI::DIID__ITTOrderSetEvents,
		&XTAPI::LIBID_XTAPI,
		1,0
	>	
{
public:
	// as this is a One-To-One connection, define an owner notification mechanism
	// (this class calls the callback functions upon recieving its events)
	class CNotify
	{
		// allow the sink access to our flags
		friend COrderSetNotifySink;
	protected:
		// individual event enables
		bool	m_bUpdate;				// allow update events
		bool	m_bFills;				// allow fill events

		CNotify(bool bUpdate=false,bool bFills=true)
		{
			// save the flags
			m_bUpdate=bUpdate;
			m_bFills=bFills;
		}

	public:
		// callback functions (One-to-One from the SINK_MAP)
		virtual void COrderSetNotifySink_Update(XTAPI::ITTOrderSet * pOrderSet)		{};
		virtual void COrderSetNotifySink_FillData(XTAPI::ITTFillObj * pFill)		{};
		virtual void COrderSetNotifySink_FillBlockStart()							{};
		virtual void COrderSetNotifySink_FillBlockEnd()								{};
	};

private:
	// notification class pointer
	CNotify*	m_pCallback;

	// smart pointer to the notification object
	XTAPI::ITTOrderSetPtr	m_pOrderSetObj;

public:
	COrderSetNotifySink();
	virtual ~COrderSetNotifySink();

	// attatch the callback class
	void SetCallback(CNotify* pCallback);

	// allow direct access to our underlying COM object
	XTAPI::ITTOrderSetPtr&	Obj() { return m_pOrderSetObj;}

	// IDispEventImpl Implementation
public:
	// define the ATL connection point event functions
	BEGIN_SINK_MAP(COrderSetNotifySink)
		SINK_ENTRY_EX(ORDERSETNOTIFYSINK,XTAPI::DIID__ITTOrderSetEvents, 1,OnOrderSetUpdate)
		SINK_ENTRY_EX(ORDERSETNOTIFYSINK,XTAPI::DIID__ITTOrderSetEvents, 2,OnOrderFillData)
		SINK_ENTRY_EX(ORDERSETNOTIFYSINK,XTAPI::DIID__ITTOrderSetEvents, 3,OnOrderFillBlockStart)
		SINK_ENTRY_EX(ORDERSETNOTIFYSINK,XTAPI::DIID__ITTOrderSetEvents, 4,OnOrderFillBlockEnd)
	END_SINK_MAP()

	// connection point callback functions
    HRESULT __stdcall OnOrderSetUpdate		(XTAPI::ITTOrderSet* pOrderSet);
	HRESULT __stdcall OnOrderFillData		(XTAPI::ITTFillObj* pFillData);
	HRESULT __stdcall OnOrderFillBlockStart ();
	HRESULT __stdcall OnOrderFillBlockEnd	();
};

#endif /* ORDERSETNOTIFYSINK_H */
