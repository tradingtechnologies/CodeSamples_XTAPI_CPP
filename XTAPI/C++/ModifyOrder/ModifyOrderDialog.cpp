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
#include "ModifyOrder.h"
#include "ModifyOrderDialog.h"

/* ModifyOrder dialog constructor.  Initialize the data mappings and
 * instantiate the instrument notifier.
 */
CModifyOrderDialog::CModifyOrderDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CModifyOrderDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//{{AFX_DATA_INIT(CModifyOrderDialog)
	m_ExchangeBox = _T("");
	m_ProductBox = _T("");
	m_ProdTypeBox = _T("");
	m_ContractBox = _T("");
	m_SOKBox = _T("");
	m_StatusBar = _T("Please drag a contract from the Market Grid.");
	//}}AFX_DATA_INIT
	
	// Instantiate the InstrNotifySink Class
	m_pInstrNotify = new CInstrNotifySink(this);
}

/* Called by the framework.
 * @see http://msdn.microsoft.com/en-us/library/x4d0y489%28v=vs.80%29.aspx
 */
void CModifyOrderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CModifyOrderDialog)
	DDX_Text(pDX, IDC_EXCHANGEBOX, m_ExchangeBox);
	DDX_Text(pDX, IDC_PRODUCTBOX, m_ProductBox);
	DDX_Text(pDX, IDC_PRODTYPEBOX, m_ProdTypeBox);
	DDX_Text(pDX, IDC_CONTRACTBOX, m_ContractBox);
	DDX_Text(pDX, IDC_SOKBOX, m_SOKBox);
	DDX_Text(pDX, IDC_STATUSBAR, m_StatusBar);
	DDX_Control(pDX, IDC_HIGHPRICEBOX, m_HighPriceBox);
	DDX_Control(pDX, IDC_LOWPRICEBOX, m_LowPriceBox);
	DDX_Control(pDX, IDC_NEWPRICEBOX, m_NewPriceBox);
	DDX_Control(pDX, IDC_NEWQTYBOX, m_NewQtyBox);
	DDX_Control(pDX, IDC_PRICEBOX, m_PriceBox);
	DDX_Control(pDX, IDC_QTYBOX, m_QtyBox);
	DDX_Control(pDX, IDC_CUSTOMERCOMBO, m_CustomerCombo);
	DDX_Control(pDX, IDC_MODIFYTYPECOMBO, m_ModifyTypeCombo);
	DDX_Control(pDX, IDC_BUYSELLCOMBO, m_BuySellCombo);
	DDX_Control(pDX, IDC_DELETEORDERCOMBO, m_DeleteOrderCombo);
	DDX_Control(pDX, IDC_MODIFYBUTTON, m_ModifyButton);
	DDX_Control(pDX, IDC_DELETEBUTTON, m_DeleteButton);
	DDX_Control(pDX, IDC_INCLUDERADIO, m_IncRadioButton);
	DDX_Control(pDX, IDC_EXCLUDERADIO, m_ExcRadioButton);
	//}}AFX_DATA_MAP
}

/* Begins the definition of your message map.
 * @see http://msdn.microsoft.com/en-us/library/9x1t4dy1%28v=vs.80%29.aspx
 */
BEGIN_MESSAGE_MAP(CModifyOrderDialog, CDialog)
	//{{AFX_MSG_MAP(CModifyOrderDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUYBUTTON, &CModifyOrderDialog::OnBnClickedBuybutton)
	ON_BN_CLICKED(IDC_SELLBUTTON, &CModifyOrderDialog::OnBnClickedSellbutton)
	ON_BN_CLICKED(IDC_MODIFYBUTTON, &CModifyOrderDialog::OnBnClickedModifybutton)
	ON_BN_CLICKED(IDC_DELETEBUTTON, &CModifyOrderDialog::OnBnClickedDeletebutton)
END_MESSAGE_MAP()

/* Called when the dialog is loaded, in response to the WM_INITDIALOG message.
 */
BOOL CModifyOrderDialog::OnInitDialog()
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

	// Make sure the first item in each combobox is selected.
	m_ModifyTypeCombo.SetCurSel(0);
	m_BuySellCombo.SetCurSel(0);
	m_DeleteOrderCombo.SetCurSel(0);

	// check an arbitrary radio button
	m_IncRadioButton.SetCheck(1);

	// Delete the font object
	delete pFont;
	pFont = NULL;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/* Called by the framework when the application terminates.  Clean
 * up all XTAPI objects.
 */
void CModifyOrderDialog::OnClose() 
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
void CModifyOrderDialog::CDropHandlerSink_NotifyDrop(const CDropHandlerSink* pSink)
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
void CModifyOrderDialog::CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr)
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

	m_ModifyButton.EnableWindow(true);
	m_DeleteButton.EnableWindow(true);

	// Call after updating the AFX_DATA fields
	UpdateData(false);
}

/* Called when the Buy button is pressed
 */
void CModifyOrderDialog::OnBnClickedBuybutton() 
{
	SendOrder("Buy");
}

/* Called when the Sell button is pressed
 */
void CModifyOrderDialog::OnBnClickedSellbutton()
{
	SendOrder("Sell");
}

/* Send an order via the API
 */
void CModifyOrderDialog::SendOrder(CString buySell)
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
		m_QtyBox.GetWindowTextA(selectedItem);
		orderProfile->Set("Qty", (LPCSTR)selectedItem);

		// Set the order type to "L" for a limit order.
		orderProfile->Set("OrderType", "L");

		// Set the limit order price.
		m_PriceBox.GetWindowTextA(selectedItem);
		orderProfile->Set("Limit$", (LPCSTR)selectedItem);

		// Send the order by submitting the TTOrderProfile through the TTOrderSet.
		long submittedQty = 0L;
		if (SUCCEEDED(m_pOrderSet->get_SendOrder(orderProfile, &submittedQty)))
		{
			// Cast submitted quantity to a string (submittedQty not currently used in this sample)
			CString strSubmittedQty;
			strSubmittedQty.Format("%ld", submittedQty);

			// Obtain the SiteOrderKey for the last order submitted.
			_variant_t vRequest("SiteOrderKey");
			_variant_t vSOK;

			orderProfile->get_GetLast(vRequest, &vSOK);
			m_SOKBox = m_LastOrderSiteOrderKey = CString(vSOK);
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

/* Called when the modify Invoke button is pressed
 */
void CModifyOrderDialog::OnBnClickedModifybutton()
{
	// Make sure we have orders to modify
	if (m_pOrderSet->Count <= 0L)
	{
		MessageBox("There are no orders in the TTOrderSet to modify!", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	// make sure a modify type was selected
	if (m_ModifyTypeCombo.GetCurSel() == 0)
	{
		MessageBox("Please specify a modify type.", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	try
	{
		// Obtain the TTOrderObj of the last order using the saved SiteOrderKey.
		XTAPI::ITTOrderObjPtr order(m_pOrderSet->GetSiteKeyLookup(_bstr_t(m_LastOrderSiteOrderKey)));

		// Obtain the TTOrderProfile from the last order.
		XTAPI::ITTOrderProfilePtr orderProfile = order->CreateOrderProfile;

		// Set the quantity and price properties.
		CString value;
		m_NewQtyBox.GetWindowTextA(value);
		orderProfile->Set("Qty", (LPCSTR)value);

		m_NewPriceBox.GetWindowTextA(value);
		orderProfile->Set("Price", (LPCSTR)value);

		// Update Order as change or cancel/replace (0 - change, 1 - cancel/replace).
		m_pOrderSet->UpdateOrder(orderProfile, (m_ModifyTypeCombo.GetCurSel() - 1));

		// Release TTOrderObj and TTOrderProfile
		order = NULL;
		orderProfile = NULL;
	}
	catch (_com_error err)
	{
		MessageBox(err.Description(), "Exception", MB_ICONERROR | MB_OK);
		return;
	}
}

/* Called when the delete Invoke button is pressed
 */
void CModifyOrderDialog::OnBnClickedDeletebutton()
{
	// make sure we have an order to delete
	if (m_pOrderSet->Count <= 0L)
	{
		MessageBox("There are no orders in the TTOrderSet to delete!", "Error", 0);
		return;
	}

	// make sure we have a delete type.
	if (m_DeleteOrderCombo.GetCurSel() == 0)
	{
		MessageBox("Please specify a delete type.", "Error", 0);
		return;
	}

	long quantityDeleted = 0L;
	_variant_t vBasePrice(0.0f);

	try
	{
		///////////////////////////
        // Delete the last order //
        ///////////////////////////
		if (m_DeleteOrderCombo.GetCurSel() == 1)
		{
			XTAPI::ITTOrderObjPtr order(m_pOrderSet->GetSiteKeyLookup(_bstr_t(m_LastOrderSiteOrderKey)));
			quantityDeleted = order->GetDelete();

			// Release TTOrderObj 
			order = NULL;
		}
		///////////////////////
        // Delete all orders //
        ///////////////////////
		else if (m_DeleteOrderCombo.GetCurSel() == 2)
		{
			_variant_t vNull;
			vNull.vt = VT_EMPTY;

			// Delete all orders.
			quantityDeleted = m_pOrderSet->GetDeleteOrders(vtMissing, vNull, vNull, 0L, NULL);
		}
		//////////////////////////////
        // Delete a range of orders //
        //////////////////////////////
		else if (m_DeleteOrderCombo.GetCurSel() == 3)
		{
			int buySell, inclusiveRange;
			_variant_t vData; 

			// Obtain the tick price of the instrument (first parameter is a base price of zero).
			m_pInstrObj->get_TickPrice(_variant_t(0.0f), 1, _bstr_t("#"), &vData);
			double tickPrice = vData.dblVal;

			// Obtain the tick increment of the instrument.
			m_pInstrObj->get_Get(_variant_t("TickIncrement"), &vData);
			int tickIncrement = vData.intVal;

			// Convert the high price range value to decimal format
			CString inputValue;
			m_LowPriceBox.GetWindowTextA(inputValue);
			m_pInstrObj->get_TickPrice(_variant_t(inputValue), 0, _bstr_t("#"), &vData);
			double lowVal = vData.dblVal;
		
			// Convert the low price range value to decimal format
			m_HighPriceBox.GetWindowTextA(inputValue);
			m_pInstrObj->get_TickPrice(_variant_t(inputValue), 0, _bstr_t("#"), &vData);
			double highVal = vData.dblVal;

			// Calculate the lowTick and highTick values
			double lowTick = (lowVal / tickPrice * (double)tickIncrement);
			double highTick = (highVal / tickPrice * (double)tickIncrement);

			if (m_BuySellCombo.GetCurSel() == 0)
			{
				buySell = 1;
			}
			else
			{
				buySell = 0;
			}

			if (m_IncRadioButton.GetCheck() == 1)
			{
				inclusiveRange = 1L;
			}
			else
			{
				inclusiveRange = 0L;
			}

			quantityDeleted = m_pOrderSet->GetDeleteOrders(buySell, lowTick, highTick, inclusiveRange, NULL);
		}

		MessageBox("Quanitity Deleted: " + (_bstr_t)quantityDeleted, "Qty", MB_ICONINFORMATION | MB_OK);
	}
	catch (_com_error err)
	{
		MessageBox(err.Description(), "Exception", MB_ICONERROR | MB_OK);
		return;
	}
}