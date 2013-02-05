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
#include "PriceUpdateDepth.h"
#include "PriceUpdateDepthDialog.h"

/* PriceUpdateDepth dialog constructor.  Initialize the data mappings and
 * instantiate the instrument notifier.
 */
CPriceUpdateDepthDialog::CPriceUpdateDepthDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPriceUpdateDepthDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//{{AFX_DATA_INIT(CPriceUpdateDepthDialog)
	m_ExchangeBox = _T("");
	m_ProdTypeBox = _T("");
	m_ProductBox = _T("");
	m_ContractBox = _T("");
	m_StatusBar = _T("Please Drag and Drop an instrument from the Market Grid in X_TRADER to this window.");
	m_bidDepthValue = _T("BidDepth(0)");
	m_askDepthValue = _T("AskDepth(0)");
	//}}AFX_DATA_INIT
	
	// XTAPI - Instantiate the InstrNotifySink Class
	m_pInstrNotify = new CInstrNotifySink(this);
}

/* Called by the framework.
 * @see http://msdn.microsoft.com/en-us/library/x4d0y489%28v=vs.80%29.aspx
 */
void CPriceUpdateDepthDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CPriceUpdateDepthDialog)
	DDX_Text(pDX, IDC_EXCHANGEBOX, m_ExchangeBox);
	DDX_Text(pDX, IDC_PRODTYPEBOX, m_ProdTypeBox);
	DDX_Text(pDX, IDC_PRODUCTBOX, m_ProductBox);
	DDX_Text(pDX, IDC_CONTRACTBOX, m_ContractBox);
	DDX_Text(pDX, IDC_STATUSBAR, m_StatusBar);
	DDX_Control(pDX, IDC_BIDDEPTHLIST, m_BidDepthList);
	DDX_Control(pDX, IDC_ASKDEPTHLIST, m_AskDepthList);
	DDX_Control(pDX, IDC_COMBOBIDDEPTH, m_BidDepthCombo);
	DDX_Control(pDX, IDC_COMBOASKDEPTH, m_AskDepthCombo);
	DDX_Control(pDX, IDC_CHKENABLEDEPTH, m_EnableDepth);
	//}}AFX_DATA_MAP
}

/* Begins the definition of your message map.
 * @see http://msdn.microsoft.com/en-us/library/9x1t4dy1%28v=vs.80%29.aspx
 */
BEGIN_MESSAGE_MAP(CPriceUpdateDepthDialog, CDialog)
	//{{AFX_MSG_MAP(CPriceUpdateDepthDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHKENABLEDEPTH, &CPriceUpdateDepthDialog::OnBnClickedChkenabledepth)
	ON_CBN_SELCHANGE(IDC_COMBOBIDDEPTH, &CPriceUpdateDepthDialog::OnCbnSelchangeCombobiddepth)
	ON_CBN_SELCHANGE(IDC_COMBOASKDEPTH, &CPriceUpdateDepthDialog::OnCbnSelchangeComboaskdepth)
END_MESSAGE_MAP()

/* Called when the dialog is loaded, in response to the WM_INITDIALOG message.
 */
BOOL CPriceUpdateDepthDialog::OnInitDialog()
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
	
	// Init form controls
	m_EnableDepth.SetCheck(BST_CHECKED);
	m_AskDepthCombo.SetCurSel(0);
	m_BidDepthCombo.SetCurSel(0);
	UpdateData(false);

	// Initialize the TTDropHandler event sink.
	m_pDropHandler = new CDropHandlerSink();	
	// Set this class to receive the XTAPI TTDropHandler events.
	m_pDropHandler->SetCallback(this);	
	// Register the application window to receive drag and drop. 
	m_pDropHandler->Obj()->raw_RegisterDropWindow((int)m_hWnd);
	// Enable depth
	m_pInstrNotify->Obj()->EnableDepthUpdates = 1;

	// Delete the font object
	delete pFont;
	pFont = NULL;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/* Called by the framework when the application terminates.  Clean
 * up all XTAPI objects.
 */
void CPriceUpdateDepthDialog::OnClose() 
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
void CPriceUpdateDepthDialog::CDropHandlerSink_NotifyDrop(const CDropHandlerSink* pSink)
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
			if (FAILED(m_pInstrObj->Open(true)))	// true enables depth
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
void CPriceUpdateDepthDialog::CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr)
{		
	m_StatusBar = (LPCSTR)"Instrument Found.";

	// Populate the UI with the instrument information.
	m_ExchangeBox = (LPCSTR)pInstr->Exchange;
	m_ProductBox =(LPCSTR)pInstr->Product;
	m_ContractBox = (LPCSTR)pInstr->Contract;
	m_ProdTypeBox = (LPCSTR)pInstr->ProdType;

	// Call after updating the AFX_DATA fields
	UpdateData(false);
}

/* Called when an instrument depth update occurs.
 * @param pInstr Instrument updated
 */
void CPriceUpdateDepthDialog::CInstrNotifySink_Depth(XTAPI::ITTInstrObj* pInstr)
{
	// clear out the current values in the list boxes
	m_BidDepthList.ResetContent();
	m_AskDepthList.ResetContent();

	try
	{
		// Get the bid depth array
		COleSafeArray* lpData = new COleSafeArray(pInstr->GetGet((LPCSTR)m_bidDepthValue));

		if (lpData)
		{
			CString bidPrice, bidQty;
			long uBound = 0L;
			long index[2];
			_variant_t vData; 

			lpData->GetUBound(1, &uBound);
			for (long i = 0L; i <= uBound; i++)
			{
				// set the multi-dimensional index for the value
				index[0] = i;

				index[1] = 0;
				lpData->GetElement(index, &vData);	
				bidPrice = CString(vData); 

				index[1] = 1;
				lpData->GetElement(index, &vData);	
				bidQty = CString(vData); 

				CString out = CString("BidPrice: ") + bidPrice + 
							  CString(" | BidQty: ") + bidQty;

				m_BidDepthList.AddString(out);
			}
		}

		delete lpData;
		lpData = NULL;

		// Get the ask depth array
		lpData = new COleSafeArray(pInstr->GetGet((LPCSTR)m_askDepthValue));

		if (lpData)
		{
			CString askPrice, askQty;
			long uBound = 0L;
			long index[2];
			_variant_t vData; 

			lpData->GetUBound(1, &uBound);
			for (long i = 0L; i <= uBound; i++)
			{
				// set the multi-dimensional index for the value
				index[0] = i;

				index[1] = 0;
				lpData->GetElement(index, &vData);	
				askPrice = CString(vData); 

				index[1] = 1;
				lpData->GetElement(index, &vData);	
				askQty = CString(vData); 

				CString out = CString("BidPrice: ") + askPrice + 
							  CString(" | BidQty: ") + askQty;

				m_AskDepthList.AddString(out);
			}
		}

		delete lpData;
		lpData = NULL;
	}
	catch (_com_error err)
	{
		MessageBox(err.Description(), _T("Exception"), 0);
		return;
	}

	// Call after updating the AFX_DATA fields
	UpdateData(false);
}

/* Called when the EnableDepth box is checked/unchecked
 */
void CPriceUpdateDepthDialog::OnBnClickedChkenabledepth()
{
	if (m_EnableDepth.GetCheck() != 0)
	{
		m_pInstrNotify->Obj()->EnableDepthUpdates = 1;
	}
	else
	{
		m_pInstrNotify->Obj()->EnableDepthUpdates = 0;

		// Clear out the current data
		m_BidDepthList.ResetContent();
		m_AskDepthList.ResetContent();

		// Call after updating the AFX_DATA fields
		UpdateData(false);
	}
}

/* Called when the BidDepth dropdown changes selection
 */
void CPriceUpdateDepthDialog::OnCbnSelchangeCombobiddepth()
{
	CString selectedDepth;

	selectedDepth.Format("%d", m_BidDepthCombo.GetCurSel());
	m_bidDepthValue = CString("BidDepth(") + selectedDepth + CString(")");
}

/* Called when the AskDepth dropdown changes selection
 */
void CPriceUpdateDepthDialog::OnCbnSelchangeComboaskdepth()
{
	CString selectedDepth;

	selectedDepth.Format("%d", m_AskDepthCombo.GetCurSel());
	m_askDepthValue = CString("AskDepth(") + selectedDepth + CString(")");
}