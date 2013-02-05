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
#include "PriceUpdateManual.h"
#include "PriceUpdateManualDialog.h"

/* PriceUpdateManual dialog constructor.  Initialize the data mappings and
 * instantiate the instrument notifier.
 */
CPriceUpdateManualDialog::CPriceUpdateManualDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPriceUpdateManualDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//{{AFX_DATA_INIT(CPriceUpdateManualDialog)
	m_BidBox = _T("");
	m_BidQtyBox = _T("");
	m_AskBox = _T("");
	m_AskQtyBox = _T("");
	m_LastBox = _T("");
	m_LastQty = _T("");
	m_LastQtyChange = _T("");
	m_StatusBar = _T("Complete the instrument information and click the Connect button.");
	//}}AFX_DATA_INIT

	// XTAPI - Instantiate the InstrNotifySink Class
	m_pInstrNotify = new CInstrNotifySink(this);
}

/* Called by the framework.
 * @see http://msdn.microsoft.com/en-us/library/x4d0y489%28v=vs.80%29.aspx
 */
void CPriceUpdateManualDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CPriceUpdateManualDialog)
	DDX_Control(pDX, IDC_EXCHANGEBOX, m_ExchangeBox);
	DDX_Control(pDX, IDC_PRODUCTBOX, m_ProductBox);
	DDX_Control(pDX, IDC_PRODTYPEBOX, m_ProdTypeBox);
	DDX_Control(pDX, IDC_CONTRACTBOX, m_ContractBox);
	DDX_Text(pDX, IDC_BIDQTYBOX, m_BidQtyBox);
	DDX_Text(pDX, IDC_ASKBOX, m_AskBox);
	DDX_Text(pDX, IDC_ASKQTYBOX, m_AskQtyBox);
	DDX_Text(pDX, IDC_BIDBOX, m_BidBox);
	DDX_Text(pDX, IDC_LASTBOX, m_LastBox);
	DDX_Text(pDX, IDC_LASTQTYBOX, m_LastQty);
	DDX_Text(pDX, IDC_LASTQTYCHGBOX, m_LastQtyChange);
	DDX_Text(pDX, IDC_STATUSBAR, m_StatusBar);
	//}}AFX_DATA_MAP
}

/* Begins the definition of your message map.
 * @see http://msdn.microsoft.com/en-us/library/9x1t4dy1%28v=vs.80%29.aspx
 */
BEGIN_MESSAGE_MAP(CPriceUpdateManualDialog, CDialog)
	//{{AFX_MSG_MAP(CPriceUpdateManualDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTNCONNECT, &CPriceUpdateManualDialog::OnBnClickedBtnconnect)
END_MESSAGE_MAP()

/* Called when the dialog is loaded, in response to the WM_INITDIALOG message.
 */
BOOL CPriceUpdateManualDialog::OnInitDialog()
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

	// Set a template for contract specs
	m_ExchangeBox.SetWindowTextA(_T("CME"));
	m_ProductBox.SetWindowTextA(_T("ES"));
	m_ProdTypeBox.SetWindowTextA(_T("FUTURE"));
	m_ContractBox.SetWindowTextA(_T("Mar13"));

	// Delete the font object
	delete pFont;
	pFont = NULL;
	
	return TRUE; // return TRUE unless you set the focus to a control
}

/* Called by the framework when the application terminates.  Clean
 * up all XTAPI objects.
 */
void CPriceUpdateManualDialog::OnClose() 
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
	}
	catch (_com_error err) { }

	CDialog::OnClose();
}

/* Called when an instrument is located after calling m_pInstrObj->Open()
 * @param pInstr Instrument located
 */
void CPriceUpdateManualDialog::CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr)
{		
	m_StatusBar = (LPCSTR)"Instrument Found.";

	// Call after updating the AFX_DATA fields
	UpdateData(false);

	ProcessPriceData(pInstr);
}

/* Called when an instrument cannot be located after calling m_pInstrObj->Open()
 * @param pInstr Instrument located
 */
void CPriceUpdateManualDialog::CInstrNotifySink_NotFound(XTAPI::ITTInstrObj* pInstr)
{		
	MessageBox("The Contract: " + pInstr->Contract + " was not found", "Error", 0);
}

/* Called when an instrument update occurs (i.e. LTP changes).
 * @param pInstr Instrument updated
 */
void CPriceUpdateManualDialog::CInstrNotifySink_Update(XTAPI::ITTInstrObj* pInstr)
{
	ProcessPriceData(pInstr);
}

/* Private method which extracts information from the instrument object
 * and publishes it to the UI.
 * @param pInstr Instrument updated
 */
void CPriceUpdateManualDialog::ProcessPriceData(XTAPI::ITTInstrObj* pInstr)
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

/* Called when the Connect button is clicked.  Attempt to connect to an 
 * instrument using the text fields supplied in the form.
 */
void CPriceUpdateManualDialog::OnBnClickedBtnconnect()
{
	m_StatusBar = (LPCSTR)"Connecting to instrument...";

	// Call after updating the AFX_DATA fields
	UpdateData(false);

	try
	{
		// detach instrument if it already exists
		if (m_pInstrObj)
		{
			if (FAILED(m_pInstrNotify->Obj()->DetachInstrument(m_pInstrObj)))
			{
				MessageBox("TTInstrObj could not be detached from the notifier.", "Error", MB_ICONERROR | MB_OK);
				return;
			}

			m_pInstrObj = NULL;
		}

		if (SUCCEEDED(m_pInstrObj.CreateInstance(__uuidof(XTAPI::TTInstrObj))))
		{
			CString input;

			m_ExchangeBox.GetWindowText(input);
			m_pInstrObj->Exchange = (LPCSTR)input;

			m_ProductBox.GetWindowText(input);
			m_pInstrObj->Product = (LPCSTR)input;

			m_ProdTypeBox.GetWindowText(input);
			m_pInstrObj->ProdType = (LPCSTR)input;

			m_ContractBox.GetWindowText(input);
			m_pInstrObj->Contract = (LPCSTR)input;

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
	}
	catch (_com_error err)
	{
		MessageBox(err.Description(), _T("Exception"), 0);
		return;
	}
}