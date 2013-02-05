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

		CNotify(bool bUpdate=true,bool bFills=false)
		{
			// save the flags
			m_bUpdate=bUpdate;
			m_bFills=bFills;
		}

	public:
		// callback functions (One-to-One from the SINK_MAP)
		virtual void COrderSetNotifySink_Update(XTAPI::ITTOrderSet* pOrderSet) {};
		virtual void COrderSetNotifySink_FillData(XTAPI::ITTFillObj* pFill) {};
		virtual void COrderSetNotifySink_FillBlockStart() {};
		virtual void COrderSetNotifySink_FillBlockEnd() {};
		virtual void COrderSetNotifySink_OnOrderRejected(XTAPI::ITTOrderObj* pRejectedOrderObj)	{};
		virtual void COrderSetNotifySink_OnOrderSubmitted(XTAPI::ITTOrderObj* pNewOrderObj,
														  XTAPI::ITTOrderObj* pOldOrderObj,
														  BSTR sSiteOrderKey,
														  XTAPI::enumOrderAction eOrderAction,
														  long wrkQty,
														  BSTR sOrderType,
														  BSTR sOrderTraits) {};
		virtual void COrderSetNotifySink_OnOrderHeld(XTAPI::ITTOrderObj* pNewOrderObj,
													 XTAPI::ITTOrderObj* pOldOrderObj,
													 BSTR sSiteOrderKey,
													 XTAPI::enumOrderAction eOrderAction,
													 long ordQty) {};
		virtual void COrderSetNotifySink_OnOrderUpdated(XTAPI::ITTOrderObj* pNewOrderObj,
														XTAPI::ITTOrderObj* pOldOrderObj,
														BSTR sSiteOrderKey,
														XTAPI::enumOrderNotifyState eOrderState,
														XTAPI::enumOrderAction eOrderAction,
														long updQty, 
														BSTR sOrderType, 
														BSTR sOrderTraits) {};
		virtual void COrderSetNotifySink_OnOrderDeleted(XTAPI::ITTOrderObj* pNewOrderObj,
														XTAPI::ITTOrderObj* pOldOrderObj,
														BSTR sSiteOrderKey,
														XTAPI::enumOrderNotifyState eOrderState,
														XTAPI::enumOrderAction eOrderAction,
														long delQty) {};
		virtual void COrderSetNotifySink_OnOrderFilled(XTAPI::ITTOrderObj* pNewOrderObj,
													   XTAPI::ITTOrderObj* pOldOrderObj,
													   BSTR sSiteOrderKey,
													   XTAPI::enumOrderNotifyState eOrderState,
													   long fillQty) {};
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
	void SetCallback(CNotify*pCallback);

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
		SINK_ENTRY_EX(ORDERSETNOTIFYSINK,XTAPI::DIID__ITTOrderSetEvents, 101,OnOrderRejected)
		SINK_ENTRY_EX(ORDERSETNOTIFYSINK,XTAPI::DIID__ITTOrderSetEvents, 102,OnOrderSubmitted)
		SINK_ENTRY_EX(ORDERSETNOTIFYSINK,XTAPI::DIID__ITTOrderSetEvents, 103,OnOrderHeld)
		SINK_ENTRY_EX(ORDERSETNOTIFYSINK,XTAPI::DIID__ITTOrderSetEvents, 104,OnOrderUpdated)
		SINK_ENTRY_EX(ORDERSETNOTIFYSINK,XTAPI::DIID__ITTOrderSetEvents, 105,OnOrderDeleted)
		SINK_ENTRY_EX(ORDERSETNOTIFYSINK,XTAPI::DIID__ITTOrderSetEvents, 107,OnOrderFilled)
	END_SINK_MAP()

	// connection point callback functions
    HRESULT __stdcall OnOrderSetUpdate		(XTAPI::ITTOrderSet* pOrderSet);
	HRESULT __stdcall OnOrderFillData		(XTAPI::ITTFillObj* pFillData);
	HRESULT __stdcall OnOrderFillBlockStart ();
	HRESULT __stdcall OnOrderFillBlockEnd	();
	HRESULT __stdcall OnOrderRejected		(XTAPI::ITTOrderObj* pRejectedOrderObj);
	HRESULT __stdcall OnOrderSubmitted		(XTAPI::ITTOrderObj* pNewOrderObj,
											 XTAPI::ITTOrderObj* pOldOrderObj,
											 BSTR sSiteOrderKey,
											 XTAPI::enumOrderAction eOrderAction,
											 long wrkQty,
											 BSTR sOrderType,
											 BSTR sOrderTraits);
	HRESULT __stdcall OnOrderHeld			(XTAPI::ITTOrderObj* pNewOrderObj,
											 XTAPI::ITTOrderObj* pOldOrderObj,
											 BSTR sSiteOrderKey,
											 XTAPI::enumOrderAction eOrderAction,
											 long ordQty);
	HRESULT __stdcall OnOrderUpdated		(XTAPI::ITTOrderObj* pNewOrderObj,
											 XTAPI::ITTOrderObj* pOldOrderObj,
											 BSTR sSiteOrderKey,
											 XTAPI::enumOrderNotifyState eOrderState,
											 XTAPI::enumOrderAction eOrderAction,
											 long updQty, 
											 BSTR sOrderType, 
											 BSTR sOrderTraits);
	HRESULT __stdcall OnOrderDeleted		(XTAPI::ITTOrderObj* pNewOrderObj,
											 XTAPI::ITTOrderObj* pOldOrderObj,
											 BSTR sSiteOrderKey,
											 XTAPI::enumOrderNotifyState eOrderState,
											 XTAPI::enumOrderAction eOrderAction,
											 long delQty);
	HRESULT __stdcall OnOrderFilled			(XTAPI::ITTOrderObj* pNewOrderObj,
											 XTAPI::ITTOrderObj* pOldOrderObj,
											 BSTR sSiteOrderKey,
											 XTAPI::enumOrderNotifyState eOrderState,
											 long fillQty);
};

#endif /* ORDERSETNOTIFYSINK_H */
