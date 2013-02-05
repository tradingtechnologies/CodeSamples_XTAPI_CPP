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
#ifndef DROPNOTIFYSINK_H
#define DROPNOTIFYSINK_H
#define	DROPNOTIFYSINK	1

class CDropHandlerSink  
:
	// derive a ATL event handler class for our drop object
	public IDispEventImpl
	<
		DROPNOTIFYSINK, 
		CDropHandlerSink,
		&XTAPI::DIID__ITTDropHandlerEvents,
		&XTAPI::LIBID_XTAPI,
		1,0
	>	
{
public:
	// as this is a One-To-One connection, define an owner notification mechanism
	// (this class calls the callback functions upon recieving its events)
	class CNotify
	{
	protected:
		CNotify() { }
	public:
		// callback functions (One-to-One from the SINK_MAP)
		virtual void CDropHandlerSink_NotifyDrop(const CDropHandlerSink*pSink)	{};
	};

private:
	// notification class pointer
	CNotify*	m_pCallback;

	// smart pointer to the COM notification object
	XTAPI::ITTDropHandlerPtr	m_pDropHandlerObj;

public:
	CDropHandlerSink();
	virtual ~CDropHandlerSink();

	// attach the callback class
	void SetCallback(CNotify*pCallback);

	// allow direct access to our underlying COM object
	XTAPI::ITTDropHandlerPtr&	Obj() { return m_pDropHandlerObj;}

	// IDispEventImpl Implementation
public:
	// define the ATL connection point event functions
	BEGIN_SINK_MAP(CDropHandlerSink)
		SINK_ENTRY_EX(DROPNOTIFYSINK,XTAPI::DIID__ITTDropHandlerEvents, 1,OnNotifyDrop)
	END_SINK_MAP()

	// connection point callback functions
    HRESULT __stdcall OnNotifyDrop();
};

#endif /* DROPNOTIFYSINK_H */