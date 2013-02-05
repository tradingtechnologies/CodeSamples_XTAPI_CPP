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
#include "ServerMode.h"
#include "ServerModeDialog.h"

/* ServerMode dialog constructor.  Initialize the data mappings and
 * instantiate the instrument notifier.
 */
CServerModeDialog::CServerModeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CServerModeDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//{{AFX_DATA_INIT(CServerModeDialog)
	m_StatusBar = _T("Enter the Server Mode login information and click the Connect button.");
	//}}AFX_DATA_INIT
}

/* Called by the framework.
 * @see http://msdn.microsoft.com/en-us/library/x4d0y489%28v=vs.80%29.aspx
 */
void CServerModeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CServerModeDialog)
	DDX_Control(pDX, IDC_USERNAMEBOX, m_UsernameBox);
	DDX_Control(pDX, IDC_PASSWORDBOX, m_PasswordBox);
	DDX_Control(pDX, IDC_ORDERUPDATEBOX, m_OrderUpdateBox);
	DDX_Text(pDX, IDC_STATUSBAR, m_StatusBar);
	//}}AFX_DATA_MAP
}

/* Begins the definition of your message map.
 * @see http://msdn.microsoft.com/en-us/library/9x1t4dy1%28v=vs.80%29.aspx
 */
BEGIN_MESSAGE_MAP(CServerModeDialog, CDialog)
	//{{AFX_MSG_MAP(CServerModeDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTNCONNECT, &CServerModeDialog::OnBnClickedBtnconnect)
END_MESSAGE_MAP()

/* Called when the dialog is loaded, in response to the WM_INITDIALOG message.
 */
BOOL CServerModeDialog::OnInitDialog()
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

	// Create the TTGate object
	m_pGate.CreateInstance(__uuidof(XTAPI::TTGate));
	// Enable Server Mode
	m_pGate->EnableServerMode();

	// Initialize the TTOrderSet event sink.
	m_pOrderSet = new COrderSetNotifySink();	
	// Set this class to receive the XTAPI TTOrderSet events.
	m_pOrderSet->SetCallback(this);	
	// Set normal level of detail in our order status events
	m_pOrderSet->Obj()->OrderStatusNotifyMode = XTAPI::ORD_NOTIFY_NORMAL;

	m_OrderUpdateBox.SetWindowTextA("Order State:   Acct, OrdStatus, OrdAction, Contract$, BuySell, OrderQty, Price, SiteOrderKey, OrderNo\r\n\r\n");

	// Delete the font object
	delete pFont;
	pFont = NULL;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/* Called by the framework when the application terminates.  Clean
 * up all XTAPI objects.
 */
void CServerModeDialog::OnClose() 
{
	try
	{
		// Terminate the XTAPI
		if (m_pGate)
		{
			m_pGate->XTAPITerminate();
			m_pGate = NULL;
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

/* Called by the user clicks the connect button.  Use the credentials
 * in the form and attempt to authenticate
 */
void CServerModeDialog::OnBnClickedBtnconnect()
{
	try
	{
		CString username, password;
		m_UsernameBox.GetWindowTextA(username);
		m_PasswordBox.GetWindowTextA(password);

		// Returns an array of all exchanges you've logged in to (not implemented in this sample)
		_variant_t vExchangeList = m_pGate->SetUniversalLogin(_bstr_t(username), _bstr_t(password));

		// Open the TTOrderSet
		m_pOrderSet->Obj()->Open(false);

		m_StatusBar = "Authenticated";

		// Call after updating the AFX_DATA fields
		UpdateData(false);
	}
	catch (_com_error err)
	{
		MessageBox(err.Description(), "Exception", MB_ICONERROR | MB_OK);
		return;
	}
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
void CServerModeDialog::COrderSetNotifySink_OnOrderSubmitted(XTAPI::ITTOrderObj* pNewOrderObj,
															 XTAPI::ITTOrderObj* pOldOrderObj,
															 BSTR sSiteOrderKey,
															 XTAPI::enumOrderAction eOrderAction,
														     long wrkQty,
															 BSTR sOrderType,
															 BSTR sOrderTraits)
{
	PublishEventOrderData("OnOrderSubmitted", pNewOrderObj, pOldOrderObj);
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
void CServerModeDialog::COrderSetNotifySink_OnOrderUpdated(XTAPI::ITTOrderObj* pNewOrderObj,
														   XTAPI::ITTOrderObj* pOldOrderObj,
														   BSTR sSiteOrderKey,
														   XTAPI::enumOrderNotifyState eOrderState,
														   XTAPI::enumOrderAction eOrderAction,
														   long updQty, 
														   BSTR sOrderType, 
														   BSTR sOrderTraits)
{
	PublishEventOrderData("OnOrderUpdated", pNewOrderObj, pOldOrderObj);
}

/* Triggered when orders are taken out of the market.
 * @param pNewOrderObj Order obj containing the updated order info
 * @param pOldOrderObj Order obj containing the previous order
 * @param sSiteOrderKey UID for the order
 * @param eOrderState State of the updated order
 * @param eOrderAction Action associated with the submitted order
 * @param delQty Quantity of the delete.
 */
void CServerModeDialog::COrderSetNotifySink_OnOrderDeleted(XTAPI::ITTOrderObj* pNewOrderObj,
														   XTAPI::ITTOrderObj* pOldOrderObj,
														   BSTR sSiteOrderKey,
														   XTAPI::enumOrderNotifyState eOrderState,
														   XTAPI::enumOrderAction eOrderAction,
														   long delQty)
{
	PublishEventOrderData("OnOrderDeleted", pNewOrderObj, pOldOrderObj);
}

/* Triggered when orders are taken out of the market.
 * @param pNewOrderObj Order obj containing the updated order info
 * @param pOldOrderObj Order obj containing the previous order
 * @param sSiteOrderKey UID for the order
 * @param eOrderState State of the updated order
 * @param fillQty Quantity of the fill.
 */
void CServerModeDialog::COrderSetNotifySink_OnOrderFilled(XTAPI::ITTOrderObj* pNewOrderObj,
														  XTAPI::ITTOrderObj* pOldOrderObj,
														  BSTR sSiteOrderKey,
														  XTAPI::enumOrderNotifyState eOrderState,
														  long fillQty)
{
	PublishEventOrderData("OnOrderFilled", pNewOrderObj, pOldOrderObj);
}

/* Publish specific information about each order to the GUI
 * @param callingMethod Method which called the update
 * @param pNewOrderObj Order obj containing the updated order info
 * @param pOldOrderObj Order obj containing the previous order
 */
void CServerModeDialog::PublishEventOrderData(CString callingMethod, XTAPI::ITTOrderObj* pNewOrderObj, XTAPI::ITTOrderObj* pOldOrderObj)
{
	CString orderEventText;

	// Get the information from the OldOrderObj
	if (pOldOrderObj->IsNull != 0)
	{
		orderEventText += callingMethod + " - Old Order: NULL\r\n";
	}
	else
	{
		try
		{
			orderEventText += callingMethod + " - Old Order: ";
			COleSafeArray* lpData = new COleSafeArray(pOldOrderObj->GetGet((LPCSTR)"Acct,OrdStatus,OrdAction,Contract$,BuySell,OrderQty,Price,SiteOrderKey,OrderNo"));

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
			orderEventText += CString(callingMethod + " - Old Order: Error Message - ") + CString((char*)err.Description()) + CString("\r\n");
		}
	}

	// Get the information from the NewOrderObj
	if (pNewOrderObj->IsNull != 0)
	{
		orderEventText += callingMethod + " - New Order: NULL\r\n";
	}
	else
	{
		try
		{
			orderEventText += callingMethod + " - New Order: ";
			COleSafeArray* lpData = new COleSafeArray(pNewOrderObj->GetGet((LPCSTR)"Acct,OrdStatus,OrdAction,Contract$,BuySell,OrderQty,Price,SiteOrderKey,OrderNo"));

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
			orderEventText += CString(callingMethod + " - New Order: Error Message - ") + CString((char*)err.Description()) + CString("\r\n");
		}
	}

	orderEventText += "\r\n";

	// Publish text to our UI.
	CString orderStatusText;
	m_OrderUpdateBox.GetWindowTextA(orderStatusText);
	m_OrderUpdateBox.SetWindowTextA(orderStatusText + orderEventText);
	
	// Call after updating the AFX_DATA fields
	UpdateData(false);
}