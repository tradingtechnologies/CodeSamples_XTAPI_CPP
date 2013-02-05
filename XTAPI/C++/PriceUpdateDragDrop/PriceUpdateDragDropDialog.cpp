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
#include "PriceUpdateDragDrop.h"
#include "PriceUpdateDragDropDialog.h"

/* PriceUpdateDragDrop dialog constructor.  Initialize the data mappings and
 * instantiate the instrument notifier.
 */
CPriceUpdateDragDropDialog::CPriceUpdateDragDropDialog(CWnd* pParent /*=NULL*/) 
	: CDialog(CPriceUpdateDragDropDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//{{AFX_DATA_INIT(CPriceUpdateDragDropDialog)
	m_BidQtyBox = _T("");
	m_AskBox = _T("");
	m_AskQtyBox = _T("");
	m_BidBox = _T("");
	m_ContractBox = _T("");
	m_ExchangeBox = _T("");
	m_LastBox = _T("");
	m_LastQty = _T("");
	m_LastQtyChange = _T("");
	m_ProdTypeBox = _T("");
	m_ProductBox = _T("");
	m_StatusBar = _T("Please Drag and Drop an instrument from the Market Grid in X_TRADER to this window.");
	//}}AFX_DATA_INIT

	// XTAPI - Instantiate the InstrNotifySink Class
	m_pInstrNotify = new CInstrNotifySink(this);
}

/* Called by the framework.
 * @see http://msdn.microsoft.com/en-us/library/x4d0y489%28v=vs.80%29.aspx
 */
void CPriceUpdateDragDropDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CPriceUpdateDragDropDialog)
	DDX_Text(pDX, IDC_EXCHANGEBOX, m_ExchangeBox);
	DDX_Text(pDX, IDC_PRODUCTBOX, m_ProductBox);
	DDX_Text(pDX, IDC_PRODTYPEBOX, m_ProdTypeBox);
	DDX_Text(pDX, IDC_CONTRACTBOX, m_ContractBox);
	DDX_Text(pDX, IDC_STATUSBAR, m_StatusBar);
	DDX_Text(pDX, IDC_BIDQTYBOX, m_BidQtyBox);
	DDX_Text(pDX, IDC_ASKBOX, m_AskBox);
	DDX_Text(pDX, IDC_ASKQTYBOX, m_AskQtyBox);
	DDX_Text(pDX, IDC_BIDBOX, m_BidBox);
	DDX_Text(pDX, IDC_CONTRACTBOX, m_ContractBox);
	DDX_Text(pDX, IDC_LASTBOX, m_LastBox);
	DDX_Text(pDX, IDC_LASTQTYBOX, m_LastQty);
	DDX_Text(pDX, IDC_LASTQTYCHGBOX, m_LastQtyChange);
	DDX_Control(pDX, IDC_CHECKBID, m_chkBid);
	DDX_Control(pDX, IDC_CHECKLTP, m_chkLTP);
	//}}AFX_DATA_MAP
}

/* Begins the definition of your message map.
 * @see http://msdn.microsoft.com/en-us/library/9x1t4dy1%28v=vs.80%29.aspx
 */
BEGIN_MESSAGE_MAP(CPriceUpdateDragDropDialog, CDialog)
	//{{AFX_MSG_MAP(CPriceUpdateDragDropDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECKBID, &CPriceUpdateDragDropDialog::OnBnClickedCheckbid)
	ON_BN_CLICKED(IDC_CHECKLTP, &CPriceUpdateDragDropDialog::OnBnClickedCheckltp)
END_MESSAGE_MAP()

/* Called when the dialog is loaded, in response to the WM_INITDIALOG message.
 */
BOOL CPriceUpdateDragDropDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Create warning font and display text
	CFont* pFont = new CFont;
    LOGFONT lf = { 0 };
	CDC* dc = GetDC();
	HDC hdc = dc->GetSafeHdc();
	lf.lfHeight = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 40);
	lf.lfWeight = FW_BOLD;
    strcpy_s(lf.lfFaceName, "Microsoft Sans Serif");
    pFont->CreateFontIndirect(&lf);
    GetDlgItem(IDC_WARNING)->SetFont(pFont);
	
	// Initialize the TTDropHandler event sink.
	m_pDropHandler = new CDropHandlerSink();

	// Set this class to receive the XTAPI TTDropHandler events.
	m_pDropHandler->SetCallback(this);	

	// Register the application window to receive drag and drop. 
	m_pDropHandler->Obj()->raw_RegisterDropWindow((int)m_hWnd);

	// Delete the font object
	delete pFont;
	pFont = NULL;

	return TRUE;  // return TRUE unless you set the focus to a control
}

/* Called by the framework when the application terminates.  Clean
 * up all XTAPI objects.
 */
void CPriceUpdateDragDropDialog::OnClose() 
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
void CPriceUpdateDragDropDialog::CDropHandlerSink_NotifyDrop(const CDropHandlerSink* pSink)
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
		MessageBox(err.Description(), "Exception", 0);
		return;
	}
}

/* Called when an instrument is located after calling m_pInstrObj->Open()
 * @param pInstr Instrument located
 */
void CPriceUpdateDragDropDialog::CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr)
{		
	m_StatusBar = (LPCSTR)"Instrument Found.";

	// Populate the UI with the instrument information.
	m_ExchangeBox = (LPCSTR)pInstr->Exchange;
	m_ProductBox = (LPCSTR)pInstr->Product;
	m_ContractBox = (LPCSTR)pInstr->Contract;
	m_ProdTypeBox = (LPCSTR)pInstr->ProdType;

	// Call after updating the AFX_DATA fields
	UpdateData(false);
}

/* Called when an instrument update occurs (i.e. LTP changes).
 * @param pInstr Instrument updated
 */
void CPriceUpdateDragDropDialog::CInstrNotifySink_Update(XTAPI::ITTInstrObj* pInstr)
{
	long index = 0L;
	_variant_t vData;  

	// extract an array containing all of our required fields.
	COleSafeArray* lpData = new COleSafeArray(pInstr->GetGet((LPCSTR)"Bid,BidQty,Ask,AskQty,Last,LastQty,~LastQty"));

	lpData->GetElement(&index, &vData);	
	m_BidBox = CString(vData); 
	
	lpData->GetElement(&++index, &vData);	
	m_BidQtyBox = CString(vData); 
	
	lpData->GetElement(&++index, &vData);	
	m_AskBox = CString(vData); 

	lpData->GetElement(&++index, &vData);	
	m_AskQtyBox = CString(vData); 

	lpData->GetElement(&++index, &vData);	
	m_LastBox = CString(vData); 

	lpData->GetElement(&++index, &vData);	
	m_LastQty = CString(vData); 

	lpData->GetElement(&++index, &vData);	
	m_LastQtyChange = CString(vData); 

	// Call after updating the AFX_DATA fields
	UpdateData(false);

	// delete the array
	delete lpData;
	lpData = NULL;
}

/* Called when the Checkbid box is checked/unchecked
 */
void CPriceUpdateDragDropDialog::OnBnClickedCheckbid()
{
	InstrumentFilter();
}

/* Called when the Checkltp box is checked/unchecked
 */
void CPriceUpdateDragDropDialog::OnBnClickedCheckltp()
{
	InstrumentFilter();
}

/* Update the notify filter
 */
void CPriceUpdateDragDropDialog::InstrumentFilter()
{
	CString filterString = L"";

	if ((m_chkBid.GetCheck() == 1) && (m_chkLTP.GetCheck() == 1))
	{
		filterString = L"Bid,Last";
	}
	else if (m_chkBid.GetCheck() == 1)
	{
		filterString = L"Bid";
	}
	else if (m_chkLTP.GetCheck() == 1)
	{
		filterString = L"Last";
	}

	m_pInstrNotify->Obj()->UpdateFilter = (_bstr_t)filterString;
}