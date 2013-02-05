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
#include "PriceUpdateMultiple.h"
#include "PriceUpdateMultipleDialog.h"

/* PriceUpdateMultiple dialog constructor.  Initialize the data mappings and
 * instantiate the instrument notifier.
 */
CPriceUpdateMultipleDialog::CPriceUpdateMultipleDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPriceUpdateMultipleDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//{{AFX_DATA_INIT(CPriceUpdateMultipleDialog)
	m_ExchangeBox1 = _T("");
	m_ProductBox1 = _T("");
	m_ProdTypeBox1 = _T("");
	m_ContractBox1 = _T("");
	m_BidBox1 = _T("");
	m_AskBox1 = _T("");
	m_LastBox1 = _T("");
	m_ExchangeBox2 = _T("");
	m_ProductBox2 = _T("");
	m_ProdTypeBox2 = _T("");
	m_ContractBox2 = _T("");
	m_BidBox2 = _T("");
	m_AskBox2 = _T("");
	m_LastBox2 = _T("");
	m_ExchangeBox3 = _T("");
	m_ProductBox3 = _T("");
	m_ProdTypeBox3 = _T("");
	m_ContractBox3 = _T("");
	m_BidBox3 = _T("");
	m_AskBox3 = _T("");
	m_LastBox3 = _T("");
	m_ExchangeBox4 = _T("");
	m_ProductBox4 = _T("");
	m_ProdTypeBox4 = _T("");
	m_ContractBox4 = _T("");
	m_BidBox4 = _T("");
	m_AskBox4 = _T("");
	m_LastBox4 = _T("");
	m_StatusBar = _T("Please Drag and Drop an instrument from the Market Grid in X_TRADER to this window.");
	//}}AFX_DATA_INIT

	//XTAPI - Instantiate the InstrNotifySink Class
	m_pInstrNotify = new CInstrNotifySink(this);
}

/* Called by the framework.
 * @see http://msdn.microsoft.com/en-us/library/x4d0y489%28v=vs.80%29.aspx
 */
void CPriceUpdateMultipleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CPriceUpdateMultipleDialog)
	DDX_Text(pDX, IDC_EXCHANGEBOX1, m_ExchangeBox1);
	DDX_Text(pDX, IDC_PRODUCTBOX1, m_ProductBox1);
	DDX_Text(pDX, IDC_PRODTYPEBOX1, m_ProdTypeBox1);
	DDX_Text(pDX, IDC_CONTRACTBOX1, m_ContractBox1);
	DDX_Text(pDX, IDC_BIDBOX1, m_BidBox1);
	DDX_Text(pDX, IDC_ASKBOX1, m_AskBox1);
	DDX_Text(pDX, IDC_LASTBOX1, m_LastBox1);
	DDX_Text(pDX, IDC_EXCHANGEBOX2, m_ExchangeBox2);
	DDX_Text(pDX, IDC_PRODUCTBOX2, m_ProductBox2);
	DDX_Text(pDX, IDC_PRODTYPEBOX2, m_ProdTypeBox2);
	DDX_Text(pDX, IDC_CONTRACTBOX2, m_ContractBox2);
	DDX_Text(pDX, IDC_BIDBOX2, m_BidBox2);
	DDX_Text(pDX, IDC_ASKBOX2, m_AskBox2);
	DDX_Text(pDX, IDC_LASTBOX2, m_LastBox2);
	DDX_Text(pDX, IDC_EXCHANGEBOX3, m_ExchangeBox3);
	DDX_Text(pDX, IDC_PRODUCTBOX3, m_ProductBox3);
	DDX_Text(pDX, IDC_PRODTYPEBOX3, m_ProdTypeBox3);
	DDX_Text(pDX, IDC_CONTRACTBOX3, m_ContractBox3);
	DDX_Text(pDX, IDC_BIDBOX3, m_BidBox3);
	DDX_Text(pDX, IDC_ASKBOX3, m_AskBox3);
	DDX_Text(pDX, IDC_LASTBOX3, m_LastBox3);
	DDX_Text(pDX, IDC_EXCHANGEBOX4, m_ExchangeBox4);
	DDX_Text(pDX, IDC_PRODUCTBOX4, m_ProductBox4);
	DDX_Text(pDX, IDC_PRODTYPEBOX4, m_ProdTypeBox4);
	DDX_Text(pDX, IDC_CONTRACTBOX4, m_ContractBox4);
	DDX_Text(pDX, IDC_BIDBOX4, m_BidBox4);
	DDX_Text(pDX, IDC_ASKBOX4, m_AskBox4);
	DDX_Text(pDX, IDC_LASTBOX4, m_LastBox4);
	DDX_Text(pDX, IDC_STATUSBAR, m_StatusBar);
	//}}AFX_DATA_MAP
}

/* Begins the definition of your message map.
 * @see http://msdn.microsoft.com/en-us/library/9x1t4dy1%28v=vs.80%29.aspx
 */
BEGIN_MESSAGE_MAP(CPriceUpdateMultipleDialog, CDialog)
	//{{AFX_MSG_MAP(CPriceUpdateMultipleDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/* Called when the dialog is loaded, in response to the WM_INITDIALOG message.
 */
BOOL CPriceUpdateMultipleDialog::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/* Called by the framework when the application terminates.  Clean
 * up all XTAPI objects.
 */
void CPriceUpdateMultipleDialog::OnClose() 
{
	try
	{
		// Terminate the XTAPI
		XTAPI::ITTGatePtr gate;
		gate.CreateInstance(__uuidof(XTAPI::TTGate));

		gate->XTAPITerminate();
		gate = NULL;

		for (long i = 0; i < 4; i++)
		{
			// Destroy the TTInstrObj objects if they exist.
			if (m_pInstrObj[i])
			{
				// Remove the instrument and reset the alias
				m_pInstrNotify->Obj()->DetachInstrument(m_pInstrObj[i]);
				m_pInstrObj[i]->Alias = "";
				m_pInstrObj[i] = NULL;
			}
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
void CPriceUpdateMultipleDialog::CDropHandlerSink_NotifyDrop(const CDropHandlerSink* pSink)
{
	m_StatusBar = (LPCSTR)"Drag & Drop detected.  Initializing instrument...";

	// Call after updating the AFX_DATA fields
	UpdateData(false);

	try
	{
		// Obtain the number of instruments dropped.
		long instrCount = m_pDropHandler->Obj()->Count;

		//Only support a maximum of four instruments
		if (instrCount > 4)
		{
			instrCount = 4;
		}

		for (long i = 0; i < instrCount; i++)
		{
			// Destroy the TTInstrObj objects if they exist.
			if (m_pInstrObj[i])
			{
				// Remove the instrument and reset the alias
				m_pInstrNotify->Obj()->DetachInstrument(m_pInstrObj[i]);
				m_pInstrObj[i]->Alias = "";
				m_pInstrObj[i] = NULL;
			}

			VARIANT vTmp;
			VariantInit(&vTmp);  

			// Obtain the TTInstrObj from the TTDropHandler object.
			// The index for the drop handler begins at 1.
			if (SUCCEEDED(m_pDropHandler->Obj()->get_Item(_variant_t(i + 1), &vTmp)))
			{
				_variant_t vInstr;
				vInstr.Attach(vTmp);

				// Assign the instrument from what was passed through the drop handler
				m_pInstrObj[i] = XTAPI::ITTInstrObjPtr(vInstr);

				// Attach the TTInstrObj to the TTInstrNotify object.
				if (FAILED(m_pInstrNotify->Obj()->AttachInstrument(m_pInstrObj[i])))
				{
					MessageBox("TTInstrObj could not be attached to the notifier.", "Error", MB_ICONERROR | MB_OK);
					return;
				}

				// Cast the index to a CString to use in the Alias
				CString strAlias;
				strAlias.Format("%d", i + 1);

				// Set the TTInstrObj Alias to the index for identification
				m_pInstrObj[i]->Alias = (_bstr_t)strAlias;

				// Open the TTInstrObj.
				if (FAILED(m_pInstrObj[i]->Open(false)))
				{
					MessageBox("TTInstrObj could not be opened.", "Error", MB_ICONERROR | MB_OK);
					return;
				}
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
		MessageBox(err.Description(), (LPCSTR)"Exception", 0);
	}
}

/* Called when an instrument is located after calling m_pInstrObj->Open()
 * @param pInstr Instrument located
 */
void CPriceUpdateMultipleDialog::CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr)
{		
	m_StatusBar = (LPCSTR)"Instrument Found.";

	DisplayInstrumentInformation(pInstr);
}

/* Called when an instrument update occurs (i.e. LTP changes).
 * @param pInstr Instrument updated
 */
void CPriceUpdateMultipleDialog::CInstrNotifySink_Update(XTAPI::ITTInstrObj* pInstr)
{
	DisplayInstrumentInformation(pInstr);
}

/* Publish instrument information to the GUI.
 * @param pInstr Instrument updated
 */
void CPriceUpdateMultipleDialog::DisplayInstrumentInformation(XTAPI::ITTInstrObj* pInstr)
{
	long index = 0L;
	_variant_t vData;  

	// Extract an array containing all of our required fields.
	//
	// Note:    For simplicity, the Exchange, Product, ProdType and Contract request 
    //          is redundant for market data updates.  Ideally you would only need
    //          to request this information in the OnNotifyFound event.
	COleSafeArray* lpData = new COleSafeArray(pInstr->GetGet((LPCSTR)"Exchange,Product,ProdType,Contract,Bid$,Ask$,Last$"));

	// Switch is based on Alias property set during the OnNotifyDrop event
	long alias = atol((LPCSTR)pInstr->Alias);
	switch (alias)
	{
	case 1:
		lpData->GetElement(&index, &vData);	
		m_ExchangeBox1 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_ProductBox1 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_ProdTypeBox1 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_ContractBox1 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_BidBox1 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_AskBox1 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_LastBox1 = CString(vData); 

		break;
	case 2:
		lpData->GetElement(&index, &vData);	
		m_ExchangeBox2 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_ProductBox2 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_ProdTypeBox2 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_ContractBox2 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_BidBox2 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_AskBox2 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_LastBox2 = CString(vData); 

		break;
	case 3:
		lpData->GetElement(&index, &vData);	
		m_ExchangeBox3 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_ProductBox3 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_ProdTypeBox3 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_ContractBox3 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_BidBox3 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_AskBox3 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_LastBox3 = CString(vData); 

		break;
	case 4:
		lpData->GetElement(&index, &vData);	
		m_ExchangeBox4 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_ProductBox4 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_ProdTypeBox4 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_ContractBox4 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_BidBox4 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_AskBox4 = CString(vData); 

		lpData->GetElement(&++index, &vData);	
		m_LastBox4 = CString(vData); 

		break;
	default:
		break;	
	}

	// Call after updating the AFX_DATA fields
	UpdateData(false);

	// delete the array
	delete lpData;
	lpData = NULL;
}