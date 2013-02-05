// OrderSetNotifySink.h: interface for the COrderSetNotifySink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDERSETNOTIFYSINK_H__1C519627_DF8D_11D6_A2CF_00B0D0F3CCA6__INCLUDED_)
#define AFX_ORDERSETNOTIFYSINK_H__1C519627_DF8D_11D6_A2CF_00B0D0F3CCA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// OrderSet Object Conection point helper class
//////////////////////////////////////////////////////////////////////
// Creating this object creates an underlying XTAPI OrderSet
// object and attaches a connection point to it. 
// Use 'SetCallback(.) to connect into the events.
// Note:- NO other intialisation is performed, use Obj() to access
// the XTAPI object itself
//////////////////////////////////////////////////////////////////////

/////////////////////////////
// load the XTAPI definitions
/////////////////////////////

/////////////////////////////

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
	END_SINK_MAP()

	// connection point callback functions
    HRESULT __stdcall OnOrderSetUpdate		(XTAPI::ITTOrderSet* pOrderSet);
	HRESULT __stdcall OnOrderFillData		(XTAPI::ITTFillObj* pFillData);
	HRESULT __stdcall OnOrderFillBlockStart ();
	HRESULT __stdcall OnOrderFillBlockEnd	();
};

#endif // !defined(AFX_ORDERSETNOTIFYSINK_H__1C519627_DF8D_11D6_A2CF_00B0D0F3CCA6__INCLUDED_)
