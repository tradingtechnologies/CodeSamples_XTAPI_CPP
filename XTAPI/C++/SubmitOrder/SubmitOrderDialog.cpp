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
#include "SubmitOrder.h"
#include "SubmitOrderDialog.h"

/* SubmitOrder dialog constructor.  Initialize the data mappings and
 * instantiate the instrument notifier.
 */
CSubmitOrderDialog::CSubmitOrderDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSubmitOrderDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//{{AFX_DATA_INIT(CSubmitOrderDialog)
	m_ExchangeBox = _T("");
	m_ProductBox = _T("");
	m_ProdTypeBox = _T("");
	m_ContractBox = _T("");
	m_BidPriceBox = _T("");
	m_AskPriceBox = _T("");
	m_LTPBox = _T("");
	m_ChangeBox = _T("");
	m_OrderBookBox = _T("");
	m_StatusBar = _T("Please drag a contract from the Market Grid");
	//}}AFX_DATA_INIT

	// Instantiate the InstrNotifySink Class
	m_pInstrNotify = new CInstrNotifySink(this);
}

/* Called by the framework.
 * @see http://msdn.microsoft.com/en-us/library/x4d0y489%28v=vs.80%29.aspx
 */
void CSubmitOrderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CSubmitOrderDialog)
	DDX_Text(pDX, IDC_EXCHANGEBOX, m_ExchangeBox);
	DDX_Text(pDX, IDC_PRODUCTBOX, m_ProductBox);
	DDX_Text(pDX, IDC_PRODTYPEBOX, m_ProdTypeBox);
	DDX_Text(pDX, IDC_CONTRACTBOX, m_ContractBox);
	DDX_Text(pDX, IDC_BIDBOX, m_BidPriceBox);
	DDX_Text(pDX, IDC_ASKBOX, m_AskPriceBox);
	DDX_Text(pDX, IDC_LTPBOX, m_LTPBox);
	DDX_Text(pDX, IDC_CHANGEBOX, m_ChangeBox);
	DDX_Text(pDX, IDC_ORDERBOOKBOX, m_OrderBookBox);
	DDX_Text(pDX, IDC_STATUSBAR, m_StatusBar);
	DDX_Control(pDX, IDC_PRICEBOX, m_PriceControl);
	DDX_Control(pDX, IDC_QUANTITYBOX, m_QuantityControl);
	DDX_Control(pDX, IDC_STOPPRICEBOX, m_StopPriceControl);
	DDX_Control(pDX, IDC_CUSTOMERCOMBO, m_CustomerCombo);
	DDX_Control(pDX, IDC_ORDERTYPECOMBO, m_OrderTypeCombo);
	DDX_Control(pDX, IDC_BUYBUTTON, m_BuyButton);
	DDX_Control(pDX, IDC_SELLBUTTON, m_SellButton);
	//}}AFX_DATA_MAP
}

/* Begins the definition of your message map.
 * @see http://msdn.microsoft.com/en-us/library/9x1t4dy1%28v=vs.80%29.aspx
 */
BEGIN_MESSAGE_MAP(CSubmitOrderDialog, CDialog)
	//{{AFX_MSG_MAP(CSubmitOrderDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUYBUTTON, OnBuyButton)
	ON_BN_CLICKED(IDC_SELLBUTTON, OnSellButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/* Called when the dialog is loaded, in response to the WM_INITDIALOG message.
 */
BOOL CSubmitOrderDialog::OnInitDialog()
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

	// Initialize the TTDropHandler event sink.
	m_pDropHandler = new CDropHandlerSink();	
	// Set this class to receive the XTAPI TTDropHandler events.
	m_pDropHandler->SetCallback(this);	
	// Register the application window to receive drag and drop. 
	m_pDropHandler->Obj()->raw_RegisterDropWindow((int)m_hWnd);

	m_OrderTypeCombo.SetCurSel(0);

	// Delete the font object
	delete pFont;
	pFont = NULL;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/* Called by the framework when the application terminates.  Clean
 * up all XTAPI objects.
 */
void CSubmitOrderDialog::OnClose() 
{
	try
	{
		// Terminate the XTAPI
		XTAPI::ITTGatePtr gate;
		gate.CreateInstance(__uuidof(XTAPI::TTGate));

		gate->XTAPITerminate();
		gate = NULL;

		// Release TTInstrObj if it exists
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

		// Decrements the reference count for the TTDropHandler object if it exists.
		if (m_pDropHandler)
		{
			delete m_pDropHandler;
			m_pDropHandler = NULL;
		}

		// Release TTOrderSet if it exists
		if (m_pOrderSet)
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
void CSubmitOrderDialog::CDropHandlerSink_NotifyDrop(const CDropHandlerSink* pSink)
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
		MessageBox(err.Description(), "Exception", MB_ICONERROR | MB_OK);
		return;
	}
}

/* Called when an instrument is located after calling m_pInstrObj->Open()
 * @param pInstr Instrument located
 */
void CSubmitOrderDialog::CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr)
{		
	m_StatusBar = (LPCSTR)"Instrument found.";
	
	// Populate the UI with the instrument information.
	m_ExchangeBox = (LPCSTR)pInstr->Exchange;
	m_ProductBox =(LPCSTR)pInstr->Product;
	m_ContractBox = (LPCSTR)pInstr->Contract;
	m_ProdTypeBox = (LPCSTR)pInstr->ProdType;

	// Create a TTOrderProfile to query for the Customer list
	XTAPI::ITTOrderProfilePtr orderProfile;
	orderProfile.CreateInstance(__uuidof(XTAPI::TTOrderProfile));

	// Populate the Customer ComboBox
	COleSafeArray* lpCustomers = new COleSafeArray(orderProfile->Customers);

	BSTR customer;  
	for (long i = 0L; i < (long)lpCustomers->GetOneDimSize(); i++)
	{
		lpCustomers->GetElement(&i, &customer);	
		m_CustomerCombo.AddString(CString(customer));
	}

	// Release TTOrderProfile
	orderProfile = NULL;

	// delete the array
	delete lpCustomers;
	lpCustomers = NULL;

	// select the first customer in the list.
	m_CustomerCombo.SetCurSel(0);

	// Create the TTOrderSet
	if (SUCCEEDED(m_pOrderSet.CreateInstance(__uuidof(XTAPI::TTOrderSet))))
	{	
		// Set NetLimits to false.
		m_pOrderSet->Set("NetLimits",0L);

		// Open the TTOrderSet to allow orders to be placed.
        m_pOrderSet->Open(true);
	}

	// Enable the controls on the form
	m_PriceControl.EnableWindow(true);
	m_QuantityControl.EnableWindow(true);
	m_StopPriceControl.EnableWindow(true);
	m_CustomerCombo.EnableWindow(true);
	m_OrderTypeCombo.EnableWindow(true);
	m_BuyButton.EnableWindow(true);
	m_SellButton.EnableWindow(true);

	// Call after updating the AFX_DATA fields
	UpdateData(false);
}

/* Called when an instrument update occurs (i.e. LTP changes).
 * @param pInstr Instrument updated
 */
void CSubmitOrderDialog::CInstrNotifySink_Update(XTAPI::ITTInstrObj* pInstr)
{
	long index = 0L;
	_variant_t vData;  

	// extract an array containing all of our required fields.
	COleSafeArray* lpData = new COleSafeArray(pInstr->GetGet((LPCSTR)"Bid,Ask,Last,Change"));

	lpData->GetElement(&index, &vData);	
	m_BidPriceBox = CString(vData); 
	
	lpData->GetElement(&++index, &vData);	
	m_AskPriceBox = CString(vData); 
	
	lpData->GetElement(&++index, &vData);	
	m_LTPBox = CString(vData); 

	lpData->GetElement(&++index, &vData);	
	m_ChangeBox = CString(vData); 

	// Call after updating the AFX_DATA fields
	UpdateData(false);

	// delete the array
	delete lpData;
	lpData = NULL;
}

/* Called when the Buy button is pressed
 */
void CSubmitOrderDialog::OnBuyButton() 
{
	SendOrder("Buy");	
}

/* Called when the Sell button is pressed
 */
void CSubmitOrderDialog::OnSellButton()
{
	SendOrder("Sell");
}

/* Send an order via the API
 */
void CSubmitOrderDialog::SendOrder(CString buySell)
{
	// String placeholder for selected items in the form
	CString selectedItem;

	try
	{
		// Create a TTOrderProfile to query for the Customer list
		XTAPI::ITTOrderProfilePtr orderProfile;
		orderProfile.CreateInstance(__uuidof(XTAPI::TTOrderProfile));

		// Set the TTInstrObj to the TTOrderProfile.
		orderProfile->PutInstrument(m_pInstrObj);	

		// Set the customer default property (e.g. "<Default>").
		m_CustomerCombo.GetLBText(m_CustomerCombo.GetCurSel(), selectedItem);
		orderProfile->PutCustomer((LPCSTR)selectedItem);

		// Set for Buy or Sell.
		orderProfile->Set("BuySell$", (LPCSTR)buySell);

		// Set the quantity.
		m_QuantityControl.GetWindowTextA(selectedItem);
		orderProfile->Set("Qty", (LPCSTR)selectedItem);

		// Set the OrderType and other respective properties
		if (m_OrderTypeCombo.GetCurSel() == 0)
		{
			orderProfile->Set("OrderType", "M");	
		}
		else if (m_OrderTypeCombo.GetCurSel() == 1)
		{
			orderProfile->Set("OrderType", "L");

			m_PriceControl.GetWindowTextA(selectedItem);
			orderProfile->Set("Limit$", (LPCSTR)selectedItem);
		}
		else if (m_OrderTypeCombo.GetCurSel() == 2)
		{
			orderProfile->Set("OrderType","M");
			orderProfile->Set("OrderRestr", "S");

			m_StopPriceControl.GetWindowTextA(selectedItem);
			orderProfile->Set("Stop$", (LPCSTR)selectedItem);
		}
		else if (m_OrderTypeCombo.GetCurSel() == 3)
		{
			orderProfile->Set("OrderType","L");
			orderProfile->Set("OrderRestr", "S");

			m_PriceControl.GetWindowTextA(selectedItem);
			orderProfile->Set("Limit$",(LPCSTR)selectedItem);

			m_StopPriceControl.GetWindowTextA(selectedItem);
			orderProfile->Set("Stop$", (LPCSTR)selectedItem);
		}

		// Send the order by submitting the TTOrderProfile through the TTOrderSet.
		long submittedQty = 0L;
		if (SUCCEEDED(m_pOrderSet->get_SendOrder(orderProfile, &submittedQty)))
		{
			// Cast submitted quantity to a string
			CString strSubmittedQty;
			strSubmittedQty.Format("%ld", submittedQty);

			m_OrderBookBox += CString("Quantity Sent: ") + strSubmittedQty + CString("\r\n");
		}
		else
		{
			MessageBox("Order Failed to be placed.", "Error", MB_ICONERROR | MB_OK);
		}

		// Release TTOrderProfile
		orderProfile = NULL;
	}
	catch (_com_error err)
	{
		MessageBox(err.Description(), "Exception", MB_ICONERROR | MB_OK);
		return;
	}

	// Call after updating the AFX_DATA fields
	UpdateData(false);
}