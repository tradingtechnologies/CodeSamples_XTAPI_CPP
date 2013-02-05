/// <summary>
/// DropHandlerSink.h: interface for the CDropHandlerSink class.
/// </summary>

#if !defined(AFX_DROPHANDLERSINK_H__211B26C5_D6D6_11D6_A2C2_00B0D0F3CCA6__INCLUDED_)
#define AFX_DROPHANDLERSINK_H__211B26C5_D6D6_11D6_A2C2_00B0D0F3CCA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


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
		CNotify()
		{
		}
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

#endif // !defined(AFX_DROPHANDLERSINK_H__211B26C5_D6D6_11D6_A2C2_00B0D0F3CCA6__INCLUDED_)
