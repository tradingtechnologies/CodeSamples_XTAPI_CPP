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
#include "ProfitLoss.h"
#include "ProfitLossDialog.h"

/* ProfitLoss dialog constructor.  Initialize the data mappings and
 * instantiate the instrument notifier.
 */
CProfitLossDialog::CProfitLossDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProfitLossDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//{{AFX_DATA_INIT(CProfitLossDialog)
	m_ExchangeBox1 = _T("");
	m_ProductBox1 = _T("");
	m_ProdTypeBox1 = _T("");
	m_ContractBox1 = _T("");
	m_PLBox1 = _T("");
	m_NetPosBox1 = _T("");
	m_ExchangeBox2 = _T("");
	m_ProductBox2 = _T("");
	m_ProdTypeBox2 = _T("");
	m_ContractBox2 = _T("");
	m_PLBox2 = _T("");
	m_NetPosBox2 = _T("");
	m_ExchangeBox3 = _T("");
	m_ProductBox3 = _T("");
	m_ProdTypeBox3 = _T("");
	m_ContractBox3 = _T("");
	m_PLBox3 = _T("");
	m_NetPosBox3 = _T("");
	m_ExchangeBox4 = _T("");
	m_ProductBox4 = _T("");
	m_ProdTypeBox4 = _T("");
	m_ContractBox4 = _T("");
	m_PLBox4 = _T("");
	m_NetPosBox4 = _T("");
	m_StatusBar = _T("Please Drag and Drop an instrument from the Market Grid in X_TRADER to this window.");
	//}}AFX_DATA_INIT
	
	// Instantiate the InstrNotifySink Class
	m_pInstrNotify = new CInstrNotifySink(this);
}

/* Called by the framework.
 * @see http://msdn.microsoft.com/en-us/library/x4d0y489%28v=vs.80%29.aspx
 */
void CProfitLossDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CProfitLossDialog)
	DDX_Text(pDX, IDC_EXCHANGEBOX1, m_ExchangeBox1);
	DDX_Text(pDX, IDC_PRODUCTBOX1, m_ProductBox1);
	DDX_Text(pDX, IDC_PRODTYPEBOX1, m_ProdTypeBox1);
	DDX_Text(pDX, IDC_CONTRACTBOX1, m_ContractBox1);
	DDX_Text(pDX, IDC_PLBOX1, m_PLBox1);
	DDX_Text(pDX, IDC_NETPOSBOX1, m_NetPosBox1);
	DDX_Text(pDX, IDC_EXCHANGEBOX2, m_ExchangeBox2);
	DDX_Text(pDX, IDC_PRODUCTBOX2, m_ProductBox2);
	DDX_Text(pDX, IDC_PRODTYPEBOX2, m_ProdTypeBox2);
	DDX_Text(pDX, IDC_CONTRACTBOX2, m_ContractBox2);
	DDX_Text(pDX, IDC_PLBOX2, m_PLBox2);
	DDX_Text(pDX, IDC_NETPOSBOX2, m_NetPosBox2);
	DDX_Text(pDX, IDC_EXCHANGEBOX3, m_ExchangeBox3);
	DDX_Text(pDX, IDC_PRODUCTBOX3, m_ProductBox3);
	DDX_Text(pDX, IDC_PRODTYPEBOX3, m_ProdTypeBox3);
	DDX_Text(pDX, IDC_CONTRACTBOX3, m_ContractBox3);
	DDX_Text(pDX, IDC_PLBOX3, m_PLBox3);
	DDX_Text(pDX, IDC_NETPOSBOX3, m_NetPosBox3);
	DDX_Text(pDX, IDC_EXCHANGEBOX4, m_ExchangeBox4);
	DDX_Text(pDX, IDC_PRODUCTBOX4, m_ProductBox4);
	DDX_Text(pDX, IDC_PRODTYPEBOX4, m_ProdTypeBox4);
	DDX_Text(pDX, IDC_CONTRACTBOX4, m_ContractBox4);
	DDX_Text(pDX, IDC_PLBOX4, m_PLBox4);
	DDX_Text(pDX, IDC_NETPOSBOX4, m_NetPosBox4);
	DDX_Text(pDX, IDC_STATUSBAR, m_StatusBar);
	//}}AFX_DATA_MAP
}

/* Begins the definition of your message map.
 * @see http://msdn.microsoft.com/en-us/library/9x1t4dy1%28v=vs.80%29.aspx
 */
BEGIN_MESSAGE_MAP(CProfitLossDialog, CDialog)
	//{{AFX_MSG_MAP(CProfitLossDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/* Called when the dialog is loaded, in response to the WM_INITDIALOG message.
 */
BOOL CProfitLossDialog::OnInitDialog()
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
void CProfitLossDialog::OnClose() 
{
	try
	{
		// Terminate the XTAPI
		XTAPI::ITTGatePtr gate;
		gate.CreateInstance(__uuidof(XTAPI::TTGate));

		gate->XTAPITerminate();
		gate = NULL;

		// Destroy the TTInstrObj objects if they exist
		for (long i = 0; i < 4; i++)
		{
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

		// Destroy the sending TTOrderSet object if it exists.
		if(m_pOrderSet)
		{
			m_pOrderSet = NULL;
		}
	}
	catch (_com_error err) { }

	CDialog::OnClose();
}

/* Called when an instrument is dragged and dropped from the 
 * Market Grid in X_TRADER.
 */
void CProfitLossDialog::CDropHandlerSink_NotifyDrop(const CDropHandlerSink* pSink)
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
				strAlias.Format("%d", i);

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
void CProfitLossDialog::CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr)
{		
	long index = 0L;
	_variant_t vData;  

	// Update the Status Bar text.
	m_StatusBar = (LPCSTR)"Instrument Found.";

	try
	{
		// Retrieve and display the instrument information.
		COleSafeArray* lpData = new COleSafeArray(pInstr->GetGet((LPCSTR)"Exchange,Product,ProdType,Contract"));

		// Test based on Alias property
		int alias = atoi((LPCSTR)pInstr->Alias);

		// Instantiate the TTOrderSet object. 
		m_pOrderSet.CreateInstance(__uuidof(XTAPI::TTOrderSet));

		// Set the TTOrderSelector to the Instrument so that P/L per contract
		// will be displayed.  Otherwise the overall P/L per user will be displayed.
		m_pOrderSet->OrderSelector = m_pInstrObj[alias]->CreateOrderSelector;

		m_pOrderSet->EnableFillCaching = 1;
		// Full qualified name: XTAPI::enumAvgOpenPriceMode::LIFO;
		m_pOrderSet->AvgOpenPriceMode = XTAPI::LIFO;

		// Attach the TTOrderSet to the TTInstrObj.
		m_pInstrObj[alias]->OrderSet = m_pOrderSet;
		// Open the TTOrderSet.
		m_pInstrObj[alias]->OrderSet->Open(0L);

		switch (alias)
		{
		case 0:
			lpData->GetElement(&index, &vData);	
			m_ExchangeBox1 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_ProductBox1 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_ProdTypeBox1 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_ContractBox1 = CString(vData); 

			break;
		case 1:
			lpData->GetElement(&index, &vData);	
			m_ExchangeBox2 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_ProductBox2 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_ProdTypeBox2 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_ContractBox2 = CString(vData); 

			break;
		case 2:
			lpData->GetElement(&index, &vData);	
			m_ExchangeBox3 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_ProductBox3 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_ProdTypeBox3 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_ContractBox3 = CString(vData); 

			break;
		case 3:
			lpData->GetElement(&index, &vData);	
			m_ExchangeBox4 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_ProductBox4 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_ProdTypeBox4 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_ContractBox4 = CString(vData); 

			break;
		default:
			break;	
		}

		// delete the array
		delete lpData;
		lpData = NULL;
	}
	catch (_com_error err)
	{
		MessageBox(err.Description(), (LPCSTR)"Exception", 0);
	}

	// Call after updating the AFX_DATA fields
	UpdateData(false);
}

/* Called when an instrument update occurs (i.e. LTP changes).
 * @param pInstr Instrument updated
 */
void CProfitLossDialog::CInstrNotifySink_Update(XTAPI::ITTInstrObj* pInstr)
{
	DisplayPLInformation(pInstr);
}

/* Called when a change in the OrderSet occurs which is attached to the TTInstrNotify object
 * @param pInstr Instrument updated
 */
void CProfitLossDialog::CInstrNotifySink_OrderSet(XTAPI::ITTInstrObj* pInstr, XTAPI::ITTOrderSet* pOrderSet)
{
	DisplayPLInformation(pInstr);
}

/* Publish instrument information to the GUI.
 * @param pInstr Instrument updated
 */
void CProfitLossDialog::DisplayPLInformation(XTAPI::ITTInstrObj* pInstr)
{
	long index = 0L;
	_variant_t vData;  

	try
	{
		// extract an array containing all of our required fields.
		COleSafeArray* lpData = new COleSafeArray(pInstr->GetGet((LPCSTR)"PL$,NetPos"));

		// Test based on Alias property
		int alias = atoi((LPCSTR)pInstr->Alias);
		switch (alias)
		{
		case 0:
			lpData->GetElement(&index, &vData);	
			m_PLBox1 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_NetPosBox1 = CString(vData); 

			break;
		case 1:
			lpData->GetElement(&index, &vData);	
			m_PLBox2 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_NetPosBox2 = CString(vData); 

			break;
		case 2:
			lpData->GetElement(&index, &vData);	
			m_PLBox3 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_NetPosBox3 = CString(vData); 

			break;
		case 3:
			lpData->GetElement(&index, &vData);	
			m_PLBox4 = CString(vData); 

			lpData->GetElement(&++index, &vData);	
			m_NetPosBox4 = CString(vData); 

			break;
		default:
			break;	
		}

		// delete the array
		delete lpData;
		lpData = NULL;
	}
	catch (_com_error err)
	{
		MessageBox(err.Description(), (LPCSTR)"Exception", 0);
	}

	// Call after updating the AFX_DATA fields
	UpdateData(false);
}