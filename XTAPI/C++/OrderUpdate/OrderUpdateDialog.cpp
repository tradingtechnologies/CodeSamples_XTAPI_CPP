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
#include "OrderUpdate.h"
#include "OrderUpdateDialog.h"

/* OrderUpdate dialog constructor.  Initialize the data mappings and
 * instantiate the instrument notifier.
 */
COrderUpdateDialog::COrderUpdateDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COrderUpdateDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//{{AFX_DATA_INIT(COrderUpdateDialog)
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
void COrderUpdateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(COrderUpdateDialog)
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
	DDX_Control(pDX, IDC_ORDERSTATUSBOX, m_OrderStatusEdit);
	//}}AFX_DATA_MAP
}

/* Begins the definition of your message map.
 * @see http://msdn.microsoft.com/en-us/library/9x1t4dy1%28v=vs.80%29.aspx
 */
BEGIN_MESSAGE_MAP(COrderUpdateDialog, CDialog)
	//{{AFX_MSG_MAP(COrderUpdateDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUYBUTTON, OnBuyButton)
	ON_BN_CLICKED(IDC_SELLBUTTON, OnSellButton)
END_MESSAGE_MAP()

/* Called when the dialog is loaded, in response to the WM_INITDIALOG message.
 */
BOOL COrderUpdateDialog::OnInitDialog()
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

	// Initialize the TTOrderSet event sink.
	m_pOrderSet = new COrderSetNotifySink();	
	// Set this class to receive the XTAPI TTOrderSet events.
	m_pOrderSet->SetCallback(this);	
	// Open the TTOrderSet directly.
	m_pOrderSet->Obj()->Open(true);
	// Enable reject events
	m_pOrderSet->Obj()->put_EnableOrderRejectData(1L);
	// Set normal level of detail in our order status events
	m_pOrderSet->Obj()->OrderStatusNotifyMode = XTAPI::ORD_NOTIFY_NORMAL;

	// Set the NetLimits to false and open the OrderSet for order placement
	m_pOrderSet->Obj()->Set("NetLimits",0L);

	// Delete the font object
	delete pFont;
	pFont = NULL;
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

/* Called by the framework when the application terminates.  Clean
 * up all XTAPI objects.
 */
void COrderUpdateDialog::OnClose() 
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
			delete m_pOrderSet;
			m_pOrderSet = NULL;
		}
	}
	catch (_com_error err) { }

	CDialog::OnClose();
}

/* Called when an instrument is dragged and dropped from the 
 * Market Grid in X_TRADER.
 */
void COrderUpdateDialog::CDropHandlerSink_NotifyDrop(const CDropHandlerSink* pSink)
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
void COrderUpdateDialog::CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr)
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

	m_CustomerCombo.EnableWindow(true);
	m_BuyButton.EnableWindow(true);
	m_SellButton.EnableWindow(true);

	// Call after updating the AFX_DATA fields
	UpdateData(false);
}

/* Triggered when the XTAPI receives an order rejected from the exchange
 * @param pRejectedOrderObj Order object for the rejected order
 */
void COrderUpdateDialog::COrderSetNotifySink_OnOrderRejected(XTAPI::ITTOrderObj* pRejectedOrderObj)
{
	CString orderStatusText;
	m_OrderStatusEdit.GetWindowTextA(orderStatusText);

	CString sok = CString(pRejectedOrderObj->GetGet((LPCSTR)"SiteOrderKey"));

	m_OrderStatusEdit.SetWindowTextA(orderStatusText + CString("Order Rejected by Exchange: ") + sok + CString("\r\n\r\n"));
}

/* Triggered when a new order is submitted to the exchange or a held
 * @param pNewOrderObj Order obj containing the updated order info
 * @param pOldOrderObj Order obj containing the previous order
 * @param sSiteOrderKey UID for the order
 * @param eOrderAction Action associated with the submitted order
 * @param wrkQty Working qty for the order
 * @param sOrderType Order Type associated with the order
 * @param sOrderTraits Traits associated with the order
 */
void COrderUpdateDialog::COrderSetNotifySink_OnOrderSubmitted(XTAPI::ITTOrderObj* pNewOrderObj,
															 XTAPI::ITTOrderObj* pOldOrderObj,
															 BSTR sSiteOrderKey,
															 XTAPI::enumOrderAction eOrderAction,
														     long wrkQty,
															 BSTR sOrderType,
															 BSTR sOrderTraits)
{
	CString orderStatusText;
	m_OrderStatusEdit.GetWindowTextA(orderStatusText);

	m_OrderStatusEdit.SetWindowTextA(orderStatusText + CString("Order Submitted: ") + CString(sSiteOrderKey) + CString("\r\n\r\n"));

	PublishEventOrderData(pNewOrderObj, pOldOrderObj);
}

/* Triggered when a TT Gateway receives a hold order
 * @param pNewOrderObj Order obj containing the updated order info
 * @param pOldOrderObj Order obj containing the previous order
 * @param sSiteOrderKey UID for the order
 * @param eOrderAction Action associated with the submitted order
 * @param ordQty Quantity of the held order
 */
void COrderUpdateDialog::COrderSetNotifySink_OnOrderHeld(XTAPI::ITTOrderObj* pNewOrderObj,
														XTAPI::ITTOrderObj* pOldOrderObj,
														BSTR sSiteOrderKey,
														XTAPI::enumOrderAction eOrderAction,
														long ordQty)
{
	CString orderStatusText;
	m_OrderStatusEdit.GetWindowTextA(orderStatusText);

	m_OrderStatusEdit.SetWindowTextA(orderStatusText + CString("Order Held: ") + CString(sSiteOrderKey) + CString("\r\n\r\n"));

	PublishEventOrderData(pNewOrderObj, pOldOrderObj);
}

/* Triggered when there is a change in the existing orders state.
 * @param pNewOrderObj Order obj containing the updated order info
 * @param pOldOrderObj Order obj containing the previous order
 * @param sSiteOrderKey UID for the order
 * @param eOrderState State of the updated order
 * @param eOrderAction Action associated with the submitted order
 * @param updQty Working qty if the updated order was in a working state
 * @param sOrderType Order Type associated with the order
 * @param sOrderTraits Traits associated with the order
 */
void COrderUpdateDialog::COrderSetNotifySink_OnOrderUpdated(XTAPI::ITTOrderObj* pNewOrderObj,
														   XTAPI::ITTOrderObj* pOldOrderObj,
														   BSTR sSiteOrderKey,
														   XTAPI::enumOrderNotifyState eOrderState,
														   XTAPI::enumOrderAction eOrderAction,
														   long updQty, 
														   BSTR sOrderType, 
														   BSTR sOrderTraits)
{
	CString orderStatusText;
	m_OrderStatusEdit.GetWindowTextA(orderStatusText);

	m_OrderStatusEdit.SetWindowTextA(orderStatusText + CString("Order Updated: ") + CString(sSiteOrderKey) + CString("\r\n\r\n"));

	PublishEventOrderData(pNewOrderObj, pOldOrderObj);
}

/* Triggered when orders are taken out of the market.
 * @param pNewOrderObj Order obj containing the updated order info
 * @param pOldOrderObj Order obj containing the previous order
 * @param sSiteOrderKey UID for the order
 * @param eOrderState State of the updated order
 * @param eOrderAction Action associated with the submitted order
 * @param delQty Quantity of the delete.
 */
void COrderUpdateDialog::COrderSetNotifySink_OnOrderDeleted(XTAPI::ITTOrderObj* pNewOrderObj,
														   XTAPI::ITTOrderObj* pOldOrderObj,
														   BSTR sSiteOrderKey,
														   XTAPI::enumOrderNotifyState eOrderState,
														   XTAPI::enumOrderAction eOrderAction,
														   long delQty)
{
	CString orderStatusText;
	m_OrderStatusEdit.GetWindowTextA(orderStatusText);

	m_OrderStatusEdit.SetWindowTextA(orderStatusText + CString("Order Deleted: ") + CString(sSiteOrderKey) + CString("\r\n\r\n"));

	PublishEventOrderData(pNewOrderObj, pOldOrderObj);
}

/* Triggered when orders are taken out of the market.
 * @param pNewOrderObj Order obj containing the updated order info
 * @param pOldOrderObj Order obj containing the previous order
 * @param sSiteOrderKey UID for the order
 * @param eOrderState State of the updated order
 * @param fillQty Quantity of the fill.
 */
void COrderUpdateDialog::COrderSetNotifySink_OnOrderFilled(XTAPI::ITTOrderObj* pNewOrderObj,
														  XTAPI::ITTOrderObj* pOldOrderObj,
														  BSTR sSiteOrderKey,
														  XTAPI::enumOrderNotifyState eOrderState,
														  long fillQty)
{
	CString orderStatusText;
	m_OrderStatusEdit.GetWindowTextA(orderStatusText);

	m_OrderStatusEdit.SetWindowTextA(orderStatusText + CString("Order Filled: ") + CString(sSiteOrderKey) + CString("\r\n\r\n"));

	PublishEventOrderData(pNewOrderObj, pOldOrderObj);
}

/* Called when the Buy button is pressed
 */
void COrderUpdateDialog::OnBuyButton() 
{
	SendOrder("Buy");	
}

/* Called when the Sell button is pressed
 */
void COrderUpdateDialog::OnSellButton()
{
	SendOrder("Sell");
}

/* Send an order via the API
 */
void COrderUpdateDialog::SendOrder(CString buySell)
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
		if (SUCCEEDED(m_pOrderSet->Obj()->get_SendOrder(orderProfile, &submittedQty)))
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

/* Publish specific information about each order to the GUI
 * @param pNewOrderObj Order obj containing the updated order info
 * @param pOldOrderObj Order obj containing the previous order
 */
void COrderUpdateDialog::PublishEventOrderData(XTAPI::ITTOrderObj* pNewOrderObj, XTAPI::ITTOrderObj* pOldOrderObj)
{
	CString orderEventText;

	// Get the information from the OldOrderObj
	if (pOldOrderObj->IsNull != 0)
	{
		orderEventText += "Old Order: NULL\r\n";
	}
	else
	{
		try
		{
			COleSafeArray* lpData = new COleSafeArray(pOldOrderObj->GetGet((LPCSTR)"Contract$,BuySell,OrderQty,Price,OrderNo,ExecutionType"));

			// Print the Get properties to the UI.
			long uBound = 0L;
			_variant_t vData;

			lpData->GetUBound(1, &uBound);
			for (long i = 0; i <= uBound; i++)
			{
				lpData->GetElement(&i, &vData);
				orderEventText += CString(((_bstr_t)vData) + CString(", "));
			}

			// trim the last comma and add a new line
			orderEventText = orderEventText.TrimRight(", ");
			orderEventText += "\r\n";

			// delete the array
			delete lpData;
			lpData = NULL;
		}
		catch (_com_error err)
		{
			orderEventText += CString("Old Order: Error Message - ") + CString((char*)err.Description()) + CString("\r\n");
		}
	}

	// Get the information from the NewOrderObj
	if (pNewOrderObj->IsNull != 0)
	{
		orderEventText += "New Order: NULL\r\n";
	}
	else
	{
		try
		{
			COleSafeArray* lpData = new COleSafeArray(pNewOrderObj->GetGet((LPCSTR)"Contract$,BuySell,OrderQty,Price,OrderNo,ExecutionType"));

			// Print the Get properties to the UI.
			long uBound = 0L;
			_variant_t vData;

			lpData->GetUBound(1, &uBound);
			for (long i = 0; i <= uBound; i++)
			{
				lpData->GetElement(&i, &vData);
				orderEventText += CString(((_bstr_t)vData) + CString(", "));
			}

			// trim the last comma and add a new line
			orderEventText = orderEventText.TrimRight(", ");
			orderEventText += "\r\n";

			// delete the array
			delete lpData;
			lpData = NULL;
		}
		catch (_com_error err)
		{
			orderEventText += CString("New Order: Error Message - ") + CString((char*)err.Description()) + CString("\r\n");
		}
	}

	orderEventText += "\r\n";

	// Publish text to our UI.
	CString orderStatusText;
	m_OrderStatusEdit.GetWindowTextA(orderStatusText);
	m_OrderStatusEdit.SetWindowTextA(orderStatusText + orderEventText);

	// Call after updating the AFX_DATA fields
	UpdateData(false);
}