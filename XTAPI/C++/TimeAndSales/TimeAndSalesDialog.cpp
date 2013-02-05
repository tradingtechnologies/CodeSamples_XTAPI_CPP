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

#include "stdafx.h"
#include "TimeAndSales.h"
#include "TimeAndSalesDialog.h"

/* PriceUpdateDragDrop dialog constructor.  Initialize the data mappings and
 * instantiate the instrument notifier.
 */
CTimeAndSalesDialog::CTimeAndSalesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeAndSalesDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//{{AFX_DATA_INIT(CTimeAndSalesDialog)
	m_StatusBar = _T("Please drag a contract from the Market Grid.");
	//}}AFX_DATA_INIT
	
	// XTAPI - Instantiate the InstrNotifySink Class
	m_pInstrNotify = new CInstrNotifySink(this);
}

/* Called by the framework.
 * @see http://msdn.microsoft.com/en-us/library/x4d0y489%28v=vs.80%29.aspx
 */
void CTimeAndSalesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTimeAndSalesDialog)
	DDX_Text(pDX, IDC_STATUSBAR, m_StatusBar);
	DDX_Control(pDX, IDC_LBPRICELIST, m_LBPriceList);
	//}}AFX_DATA_MAP
}

/* Begins the definition of your message map.
 * @see http://msdn.microsoft.com/en-us/library/9x1t4dy1%28v=vs.80%29.aspx
 */
BEGIN_MESSAGE_MAP(CTimeAndSalesDialog, CDialog)
	//{{AFX_MSG_MAP(CTimeAndSalesDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/* Called when the dialog is loaded, in response to the WM_INITDIALOG message.
 */
BOOL CTimeAndSalesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Create warning font and display text
	CFont* pFont = new CFont;
	CPaintDC dc(this);
    LOGFONT lf = { 0 };
	lf.lfHeight = -MulDiv(8, GetDeviceCaps(dc.GetSafeHdc(), LOGPIXELSY), 40);
	lf.lfWeight = FW_BOLD;
    strcpy_s(lf.lfFaceName, "Microsoft Sans Serif");
    pFont->CreateFontIndirect(&lf);
    GetDlgItem(IDC_WARNING)->SetFont(pFont);

	//XTAPI - Initialize the TTDropHandler event sink.
	m_pDropHandler = new CDropHandlerSink();	
	// Set this class to receive the XTAPI TTDropHandler events.
	m_pDropHandler->SetCallback(this);	
	// Register the application window to receive drag and drop. 
	m_pDropHandler->Obj()->raw_RegisterDropWindow((int)m_hWnd);
	// set our app so we receive price updates
	m_pInstrNotify->Obj()->DeliverAllPriceUpdates = 1L;
	// set an update filter to stop receiving OnPriceListUpdate for volume
	m_pInstrNotify->Obj()->UpdateFilter = "Last,LastQty,HitTake";

	// Delete the font object
	delete pFont;
	pFont = NULL;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/* Called by the framework when the application terminates.  Clean
 * up all XTAPI objects.
 */
void CTimeAndSalesDialog::OnClose() 
{
	try
	{
		// Terminate the XTAPI
		XTAPI::ITTGatePtr gate;
		gate.CreateInstance(__uuidof(XTAPI::TTGate));

		gate->XTAPITerminate();
		gate = NULL;

		// Destroy the TTInstrObj object if it exists.
		if (m_pInstrObj)
		{
			// Detach the instrument from the TTInstrNotify object.
			m_pInstrNotify->Obj()->DetachInstrument(m_pInstrObj);
			m_pInstrObj = NULL;
		}

		// Destroy the TTInstrNotify object if it exists.
		if (m_pInstrNotify)
		{
			delete m_pInstrNotify;
			m_pInstrNotify = NULL;
		}

		// Destroy the TTDropHandler object if it exists.
		if (m_pDropHandler)
		{
			delete m_pDropHandler;
			m_pDropHandler = NULL;
		}
	}
	catch (_com_error err) { }

	CDialog::OnClose();
}

/* Called when an instrument is dragged and dropped from the 
 * Market Grid in X_TRADER.
 */
void CTimeAndSalesDialog::CDropHandlerSink_NotifyDrop(const CDropHandlerSink* pSink)
{
	m_StatusBar = (LPCSTR)"Drag & Drop detected.  Initializing instrument...";

	// Call after updating the AFX_DATA fields
	UpdateData(false);

	try
	{
		// detach instrument if it already exists
		if (m_pInstrObj)
		{
			m_pInstrNotify->Obj()->DetachInstrument(m_pInstrObj);
			m_pInstrObj = NULL;
		}

		VARIANT vTmp;
		VariantInit(&vTmp);  
		if (SUCCEEDED(m_pDropHandler->Obj()->get_Item(_variant_t(1L), &vTmp)))
		{
			_variant_t vInstr;
			vInstr.Attach(vTmp);

			// Assign the instrument from what was passed through the drop handler
			m_pInstrObj = XTAPI::ITTInstrObjPtr(vInstr);

			// Attach the TTInstrObj to the TTInstrNotify object.
			if (FAILED(m_pInstrNotify->Obj()->AttachInstrument(m_pInstrObj)))
			{
				MessageBox("TTInstrObj could not be attached to the notifier.", "Error", MB_ICONERROR | MB_OK);
				return;
			}

			// Open the TTInstrObj.
			if (FAILED(m_pInstrObj->Open(false)))
			{
				MessageBox("TTInstrObj could not be opened.", "Error", MB_ICONERROR | MB_OK);
				return;
			}
		}

		// Reset the drop handler object
		if (FAILED(m_pDropHandler->Obj()->Reset()))
		{
			MessageBox("TTDropHandler could not be reset.", "Error", MB_ICONERROR | MB_OK);
			return;
		}
	}
	catch (_com_error err)
	{
		MessageBox(err.Description(), _T("Exception"), 0);
		return;
	}
}

/* Called when an instrument is located after calling m_pInstrObj->Open()
 * @param pInstr Instrument located
 */
void CTimeAndSalesDialog::CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr)
{		
	m_StatusBar = (LPCSTR)"Instrument Found.";

	// Call after updating the AFX_DATA fields
	UpdateData(false);
}

/* Called when an instrument trade update occurs.
 * @param pInstr Instrument updated
 */
void CTimeAndSalesDialog::CInstrNotifySink_PriceList(XTAPI::ITTInstrNotify *pNotify)
{
	if (pNotify == NULL)
		return;

	double ltp = 0.0f;
	long ltq = 0L;
	CString direction;

	// Retrieve the ITTPriceList collection
	XTAPI::ITTPriceListPtr priceList = pNotify->GetPriceList();
	CComQIPtr<IEnumVARIANT, &IID_IEnumVARIANT> updateEnum = priceList->Get_NewEnum();

	// Loop through each ITTPriceUpdate in the ITTPriceList.  The list will contain
    // all entries which occurred since the last time the OnPriceListUpdate was fired.
	for (_variant_t vPriceUpdate; updateEnum->Next(1, &vPriceUpdate, NULL) == S_OK;)
	{
		if(vPriceUpdate.vt != VT_DISPATCH)
		{
			VariantClear(&vPriceUpdate);
			continue;
		}

		// Retrieve the ITTPriceEntry collection
		XTAPI::ITTPriceUpdatePtr priceUpdate = vPriceUpdate.pdispVal;
		CComQIPtr<IEnumVARIANT, &IID_IEnumVARIANT> entryEnum = priceUpdate->Get_NewEnum();

		//Iterate through priceEntry variants in priceUpdate
		for (_variant_t vPriceEntry; entryEnum->Next(1, &vPriceEntry, NULL) == S_OK;)
		{
			if(vPriceEntry.vt != VT_DISPATCH)
			{
				VariantClear(&vPriceEntry);
				continue;
			}

			//Create PriceEntry from variant
			XTAPI::ITTPriceEntryPtr priceEntry = vPriceEntry.pdispVal;

			// Process and assign each PriceEntry
			//
			// Note:	The full enum path is omitted due to the compiler warning C4482.
			//			The full paths are listed above the enum values, which can be 
			//			used, but if so it's suggested the warning be disabled.  To 
			//			disable the warning use the following:
			//
			//			#pragma warning(push)
			//			#pragma warning(disable : 4482)
			//			{code where the warning is suppressed}
			//			#pragma warning(pop)
			//
			//			@see http://msdn.microsoft.com/en-us/library/2c8f766e%28v=vs.80%29.aspx
			switch (priceEntry->PriceID)
			{

			// XTAPI::enumPriceID::ttLastTradedPrice
			case XTAPI::ttLastTradedPrice:
				ltp = (double)priceEntry->toDouble();
				break;

			// XTAPI::enumPriceID::ttLastTradedQty
			case XTAPI::ttLastTradedQty:
				ltq = (int)priceEntry->toDouble();
				break;

			// XTAPI::enumPriceID::ttHitTake
			case XTAPI::ttHitTake:

				// XTAPI::enumTradeIndicator::TRADE_INDICATOR_HIT
				if (priceEntry->toDouble() == (double)XTAPI::TRADE_INDICATOR_HIT)
				{
					direction = "Hit"; //Sell
				}

				// XTAPI::enumTradeIndicator::TRADE_INDICATOR_TAKE
				else if (priceEntry->toDouble() == (double)XTAPI::TRADE_INDICATOR_TAKE)
				{
					direction = "Take"; //Buy
				}

				// XTAPI::enumTradeIndicator::TRADE_INDICATOR_UNKNOWN
				else if (priceEntry->toDouble() == (double)XTAPI::TRADE_INDICATOR_UNKNOWN)
				{
					direction = "Indeterminate"; //Indeterminate
				}
				break;
			}
		}

		// Format LTP and LTQ as strings
		CString sLtp, sLtq;
		sLtp.Format("%f", ltp);
		sLtq.Format("%ld", ltq);

		CString timeStamp = CString((LPCSTR)priceUpdate->TimeStamp);
		CString exchange = CString((LPCSTR)priceUpdate->Instrument->Exchange);
		CString product = CString((LPCSTR)priceUpdate->Instrument->Product);
		CString expiry = CString(priceUpdate->Instrument->GetGet((LPCSTR)"Expiry"));
		
		// Output to GUI
		m_LBPriceList.AddString(timeStamp + CString(",") + 
							    exchange + CString(",") + 
								product + CString(",") + 
								expiry + CString(",") + 
								direction + CString(",") + 
								sLtp + CString(",") + 
								sLtq);
	}
}