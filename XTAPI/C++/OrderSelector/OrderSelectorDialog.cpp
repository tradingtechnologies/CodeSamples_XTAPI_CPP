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
#include "OrderSelector.h"
#include "OrderSelectorDialog.h"

/* OrderSelector dialog constructor.  Initialize the data mappings and
 * instantiate the instrument notifier.
 */
COrderSelectorDialog::COrderSelectorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COrderSelectorDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//{{AFX_DATA_INIT(COrderSelectorDialog)
	m_ExchangeBox = _T("");
	m_ProductBox = _T("");
	m_ProdTypeBox = _T("");
	m_ContractBox = _T("");
	m_StatusBar = _T("Please drag a contract from the Market Grid");
	//}}AFX_DATA_INIT

	// Instantiate the InstrNotifySink Class
	m_pInstrNotify = new CInstrNotifySink(this);
}

/* Called by the framework.
 * @see http://msdn.microsoft.com/en-us/library/x4d0y489%28v=vs.80%29.aspx
 */
void COrderSelectorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(COrderSelectorDialog)
	DDX_Text(pDX, IDC_EXCHANGEBOX, m_ExchangeBox);
	DDX_Text(pDX, IDC_PRODUCTBOX, m_ProductBox);
	DDX_Text(pDX, IDC_PRODTYPEBOX, m_ProdTypeBox);
	DDX_Text(pDX, IDC_CONTRACTBOX, m_ContractBox);
	DDX_Text(pDX, IDC_STATUSBAR, m_StatusBar);
	DDX_Control(pDX, IDC_CUSTOMERCOMBO, m_CustomerCombo);		
	DDX_Control(pDX, IDC_PRICEBOX, m_PriceEdit);
	DDX_Control(pDX, IDC_QUANTITYBOX, m_QuantityEdit);
	DDX_Control(pDX, IDC_BUYBUTTON, m_BuyButton);
	DDX_Control(pDX, IDC_SELLBUTTON, m_SellButton);
	DDX_Control(pDX, IDC_CHK_XTRADER, m_XTRADERCheck);
	DDX_Control(pDX, IDC_CHK_XTAPI, m_XTAPICheck);
	DDX_Control(pDX, IDC_CHK_SELL, m_SellCheck);
	DDX_Control(pDX, IDC_CHK_BUY, m_BuyCheck);
	DDX_Control(pDX, IDC_CHK_CONTRACT, m_ContractCheck);
	DDX_Control(pDX, IDC_CHK_EXCHANGE, m_ExchangeCheck);
	DDX_Control(pDX, IDC_CONTRACTBOX2, m_ContractEdit);
	DDX_Control(pDX, IDC_EXCHANGEBOX2, m_ExchangeEdit);
	DDX_Control(pDX, IDC_RAD_ALLREQUIRED, m_AllMatchesRadio);
	DDX_Control(pDX, IDC_RAD_ALLOWANY, m_AnyMatchesRadio);
	DDX_Control(pDX, IDC_SELECTORBUTTON, m_SelectorButton);
	DDX_Control(pDX, IDC_ORDERBOOKBOX, m_OrderBookEdit);
	//}}AFX_DATA_MAP
}

/* Begins the definition of your message map.
 * @see http://msdn.microsoft.com/en-us/library/9x1t4dy1%28v=vs.80%29.aspx
 */
BEGIN_MESSAGE_MAP(COrderSelectorDialog, CDialog)
	//{{AFX_MSG_MAP(COrderSelectorDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUYBUTTON, OnBuyButton)
	ON_BN_CLICKED(IDC_SELLBUTTON, OnSellButton)
	ON_BN_CLICKED(IDC_SELECTORBUTTON, &COrderSelectorDialog::OnBnClickedSelectorbutton)
END_MESSAGE_MAP()

/* Called when the dialog is loaded, in response to the WM_INITDIALOG message.
 */
BOOL COrderSelectorDialog::OnInitDialog()
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

	// Initialize the TTOrderSet event sink.
	m_pOrderSetReceive = new COrderSetNotifySink();	
	// Set this class to receive the XTAPI TTOrderSet events.
	m_pOrderSetReceive->SetCallback(this);	
	// Open the TTOrderSet directly.
	m_pOrderSetReceive->Obj()->Open(true);
	// Enable the order updates and order tracker directly.
	m_pOrderSetReceive->Obj()->put_EnableOrderSetUpdates(1L);
	m_pOrderSetReceive->Obj()->put_EnableOrderUpdateData(1L);

	// Create the OrderSelector object.
	m_pOrderSelector.CreateInstance(__uuidof(XTAPI::TTOrderSelector));

	// set our radio button as checked.
	m_AllMatchesRadio.SetCheck(1);

	// Delete the font object
	delete pFont;
	pFont = NULL;

	return TRUE;  // return TRUE unless you set the focus to a control
}

/* Called by the framework when the application terminates.  Clean
 * up all XTAPI objects.
 */
void COrderSelectorDialog::OnClose() 
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

		// Destroy the sending TTOrderSet object if it exists.
		if (m_pOrderSetSend)
		{
			m_pOrderSetSend = NULL;
		}

		// Destroy the receiving TTOrderSet object if it exists.
		if (m_pOrderSetReceive)
		{
			delete m_pOrderSetReceive;
			m_pOrderSetReceive = NULL;
		}

		// Destroy the OrderSelector object if it exists.
		if (m_pOrderSelector)
		{
			m_pOrderSelector = NULL;
		}
	}
	catch (_com_error err) { }

	CDialog::OnClose();
}

/* Called when an instrument is dragged and dropped from the 
 * Market Grid in X_TRADER.
 */
void COrderSelectorDialog::CDropHandlerSink_NotifyDrop(const CDropHandlerSink* pSink)
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
void COrderSelectorDialog::CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr)
{		
	m_StatusBar = (LPCSTR)"Instrument found.";
	
	// Populate the UI with the instrument information.
	m_ExchangeBox = (LPCSTR)pInstr->Exchange;
	m_ProductBox = (LPCSTR)pInstr->Product;
	m_ContractBox = (LPCSTR)pInstr->Contract;
	m_ProdTypeBox = (LPCSTR)pInstr->ProdType;

	m_ExchangeEdit.SetWindowTextA((LPCSTR)pInstr->Exchange);
	m_ContractEdit.SetWindowTextA((LPCSTR)pInstr->Contract);

	// Create a TTOrderProfile to query for the Customer list
	XTAPI::ITTOrderProfilePtr orderProfile;
	orderProfile.CreateInstance(__uuidof(XTAPI::TTOrderProfile));

	// Populate the Customer ComboBox
	COleSafeArray* lpCustomers;
	lpCustomers = new COleSafeArray(orderProfile->Customers);

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

	// Create the sending OrderSet
	if (SUCCEEDED(m_pOrderSetSend.CreateInstance(__uuidof(XTAPI::TTOrderSet))))
	{	
		// Set NetLimits to false.
		m_pOrderSetSend->Set("NetLimits",0L);
		// Open the TTOrderSet to allow orders to be placed.
        m_pOrderSetSend->Open(true);
	}

	m_CustomerCombo.EnableWindow(true);
	m_BuyButton.EnableWindow(true);
	m_SellButton.EnableWindow(true);

	// Call after updating the AFX_DATA fields
	UpdateData(false);
}

/* Called when the Buy button is pressed
 */
void COrderSelectorDialog::OnBuyButton() 
{
	SendOrder("Buy");	

}

/* Called when the Sell button is pressed
 */
void COrderSelectorDialog::OnSellButton()
{
	SendOrder("Sell");
}

/* Send an order via the API
 */
void COrderSelectorDialog::SendOrder(CString buySell)
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
		m_QuantityEdit.GetWindowTextA(selectedItem);
		orderProfile->Set("Qty", (LPCSTR)selectedItem);

		// Set the order type to "L" for a limit order.
		orderProfile->Set("OrderType", "L");

		// Set the limit order price.
		m_PriceEdit.GetWindowTextA(selectedItem);
		orderProfile->Set("Limit$", (LPCSTR)selectedItem);

		// Send the order by submitting the TTOrderProfile through the TTOrderSet.
		long submittedQty = 0L;
		if (SUCCEEDED(m_pOrderSetSend->get_SendOrder(orderProfile, &submittedQty)))
		{
			// Cast submitted quantity to a string (submittedQty not currently used in this sample)
			CString strSubmittedQty;
			strSubmittedQty.Format("%ld", submittedQty);
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

/* Called when an instrument update occurs (i.e. LTP changes).
 *
 * Note:   The TTOrderTrackerObj has been deprecated in favor of the order status 
 *		   events, and remains in this sample for legacy code review.  
 *		   TT recommends monitoring orders through the events outlined in the
 *		   OrderUpdate example.
 * @param pOrderSet OrderSet updated
 */
void COrderSelectorDialog::COrderSetNotifySink_Update(XTAPI::ITTOrderSet* pOrderSet)
{
	// Obtain the TTOrderTrackerObj object from the TTOrderSet object passed into this function.
	XTAPI::ITTOrderTrackerObjPtr pOrderTracker = pOrderSet->GetNextOrderTracker();
	
	// Iterate through the list of TTOrderTrackerObj objects.
	while (pOrderTracker != NULL)
	{
		// Has Old order (past order state).
		if (pOrderTracker->GetHasOldOrder())  
		{
			// Obtain the TTOrderObj object from the TTOrderTrackerObj object.
			XTAPI::ITTOrderObjPtr pOrderObj = pOrderTracker->GetOldOrder();

			// Call the TTOrderObj Get properties.
			COleSafeArray* lpData = new COleSafeArray(pOrderObj->GetGet((LPCSTR)"Instr.Exchange,Contract$,BuySell,OrderSource$"));
		
			// Print the Get properties to the UI.
			long uBound = 0L;
			CString orderInfo;
			_variant_t vData;

			lpData->GetUBound(1, &uBound);
			for (long i = 0; i <= uBound; i++)
			{
				lpData->GetElement(&i, &vData);
				orderInfo += CString(((_bstr_t)vData) + CString(", "));
			}

			// get the text from the orderBookTextBox
			CString orderBookText;
			m_OrderBookEdit.GetWindowTextA(orderBookText);
			m_OrderBookEdit.SetWindowTextA(orderBookText + CString("OldOrder: ") + orderInfo + CString("\r\n"));

			delete lpData;
			lpData = NULL;

			pOrderObj = NULL;
		}
		else
		{
			// get the text from the orderBookTextBox
			CString orderBookText;
			m_OrderBookEdit.GetWindowTextA(orderBookText);
			m_OrderBookEdit.SetWindowTextA(orderBookText + CString("OldOrder: NULL\r\n"));
		}

		// Has New order (current order state).
		if(pOrderTracker->GetHasNewOrder())  
		{
			// Obtain the TTOrderObj object from the TTOrderTrackerObj object.
			XTAPI::ITTOrderObjPtr pOrderObj = pOrderTracker->GetNewOrder();	

			// Call the TTOrderObj Get properties.
			COleSafeArray* lpData = new COleSafeArray(pOrderObj->GetGet((LPCSTR)"Instr.Exchange,Contract$,BuySell,OrderSource$"));

			// Print the Get properties to the UI.
			long uBound = 0L;
			CString orderInfo;
			_variant_t vData;

			lpData->GetUBound(1, &uBound);
			for (long i = 0; i <= uBound; i++)
			{
				lpData->GetElement(&i, &vData);
				orderInfo += CString(((_bstr_t)vData) + CString(", "));
			}

			// get the text from the orderBookTextBox
			CString orderBookText;
			m_OrderBookEdit.GetWindowTextA(orderBookText);
			m_OrderBookEdit.SetWindowTextA(orderBookText + CString("NewOrder: ") + orderInfo + CString("\r\n"));

			delete lpData;
			lpData = NULL;

			pOrderObj = NULL;
		}
		else
		{
			// get the text from the orderBookTextBox
			CString orderBookText;
			m_OrderBookEdit.GetWindowTextA(orderBookText);
			m_OrderBookEdit.SetWindowTextA(orderBookText + CString("NewOrder: NULL\r\n"));
		}

		// get the text from the orderBookTextBox
		CString orderBookText;
		m_OrderBookEdit.GetWindowTextA(orderBookText);
		m_OrderBookEdit.SetWindowTextA(orderBookText + CString("\r\n"));

		// Get the next TTOrderTrackerObj object and continue iterating through the list.
		pOrderTracker = pOrderSet->GetNextOrderTracker();
	}

	UpdateData(false);
}

/* OrderSelector parameters have changed.
 */
void COrderSelectorDialog::OnBnClickedSelectorbutton()
{
	// Reset the order book text
	m_OrderBookEdit.SetWindowTextA("Order State:  Exchange,  Contract$,  BuySell,  OrderSource\r\n\r\n");

	// Reset the OrderSelector.
	m_pOrderSelector->Reset();

    // Filter orders placed through X_TRADER.
	if( m_XTRADERCheck.GetCheck() != 0 )	
		m_pOrderSelector->AddTest("OrderSource", "0");

    // Filter orders placed through XTAPI.
	if( m_XTAPICheck.GetCheck() != 0 )	
		m_pOrderSelector->AddTest("OrderSource", "3");

    // Filter sell orders.
	if( m_SellCheck.GetCheck() != 0 )	
		m_pOrderSelector->AddTest("IsSell", "True");

    // Filter buy orders.
	if( m_BuyCheck.GetCheck() != 0 )	
		m_pOrderSelector->AddTest("IsBuy", "True");

    // Filter by contract.
	if( m_ContractCheck.GetCheck() != 0 )	
	{
		CString strEditData;
		m_ContractEdit.GetWindowTextA(strEditData);
		m_pOrderSelector->AddTest("Contract$", (LPCSTR)strEditData);
	}

    // Filter by exchange.
	if( m_ExchangeCheck.GetCheck() != 0 )	
	{
		CString strEditData;
		m_ExchangeEdit.GetWindowTextA(strEditData);
		m_pOrderSelector->AddTest("Instr.Exchange", (LPCSTR)strEditData);
	}

    // "AND" all filters.
	if( m_AllMatchesRadio.GetCheck() != 0 )	
	{
		m_pOrderSelector->AllMatchesRequired = 1;
		m_pOrderSelector->AllowAnyMatches = 0;
	}
    // "OR" all filters.
	else	
	{
		m_pOrderSelector->AllMatchesRequired = 0;
		m_pOrderSelector->AllowAnyMatches = 1;
	}

	// Set the OrderSelector to the receiving TTOrderSet.
	m_pOrderSetReceive->Obj()->OrderSelector = m_pOrderSelector;
	
	// Open the TTOrderSet with send orders disabled.
	m_pOrderSetReceive->Obj()->Open(0);
}