/// <summary>
/// InstrNotifySink.h: interface for the CInstrNotifySink class.
/// </summary>

#if !defined(AFX_InstrNotifySink_H__17240D41_B077_11D6_A2AE_00B0D0F3CCA6__INCLUDED_)
#define AFX_InstrNotifySink_H__17240D41_B077_11D6_A2AE_00B0D0F3CCA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/// <summary>
/// InstrNotify Object Conection point helper class
/// </summary>
class CInstrNotifySink :
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
	END_SINK_MAP()

	// connection point callback functions
    HRESULT __stdcall OnNotifyFound(XTAPI::ITTInstrNotify* pNotify,XTAPI::ITTInstrObj* pInstr);
	HRESULT __stdcall OnNotifyUpdate(XTAPI::ITTInstrNotify* pNotify,XTAPI::ITTInstrObj* pInstr);

protected:
	// callback functions (One-to-One from the SINK_MAP)
	virtual void CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr)	{};
	virtual void CInstrNotifySink_Update(XTAPI::ITTInstrObj* pInstr) {};
};

#endif // !defined(AFX_InstrNotifySink_H__17240D41_B077_11D6_A2AE_00B0D0F3CCA6__INCLUDED_)
