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
#include "MonitorGateways.h"
#include "MonitorGatewaysDialog.h"

/* MonitorGateways dialog constructor.  Initialize the data mappings and
 * instantiate the instrument notifier.
 */
CMonitorGatewaysDialog::CMonitorGatewaysDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorGatewaysDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//{{AFX_DATA_INIT(CMonitorGatewaysDialog)
	m_StatusBar = _T("Select an instrument and click the Connect button.");
	//}}AFX_DATA_INIT
}

/* Called by the framework.
 * @see http://msdn.microsoft.com/en-us/library/x4d0y489%28v=vs.80%29.aspx
 */
void CMonitorGatewaysDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CMonitorGatewaysDialog)
	DDX_Control(pDX, IDC_TXT_PRICECONNECTION, m_txtPriceConnection);
	DDX_Control(pDX, IDC_TXT_ORDERCONNECTION, m_txtOrderConnection);
	DDX_Control(pDX, IDC_TXT_FILLCONNECTION, m_txtFillConnection);
	DDX_Control(pDX, IDC_TXT_XTRADER, m_txtXTRADER);
	DDX_Control(pDX, IDC_TXT_XTRADERPRO, m_txtXTRADERPro);
	DDX_Control(pDX, IDC_TXT_ONSTATUSUPDATE, m_txtOnStatusUpdate);
	DDX_Control(pDX, IDC_TXT_ONEXCHANGESTATEUPDATE, m_txtOnExchangeStateUpdate);
	DDX_Control(pDX, IDC_TXT_ONEXCHANGEMESSAGE, m_txtOnExchangeMessage);
	DDX_Control(pDX, IDC_CBO_AVAILABLEGATEWAYS, m_AvailableGateways);
	DDX_Control(pDX, IDC_BTN_CONNECT, m_btnConnect);
	DDX_Control(pDX, IDC_CHK_OPENPRICE, m_chkOpenPrice);
	DDX_Control(pDX, IDC_CHK_OPENORDER, m_chkOpenOrder);
	DDX_Control(pDX, IDC_CHK_OPENFILL, m_chkOpenFill);
	DDX_Text(pDX, IDC_TXT_STATUSBAR, m_StatusBar);
	//}}AFX_DATA_MAP
}

/* Begins the definition of your message map.
 * @see http://msdn.microsoft.com/en-us/library/9x1t4dy1%28v=vs.80%29.aspx
 */
BEGIN_MESSAGE_MAP(CMonitorGatewaysDialog, CDialog)
	//{{AFX_MSG_MAP(CMonitorGatewaysDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CMonitorGatewaysDialog::OnBnClickedBtnConnect)
	ON_CBN_SELCHANGE(IDC_CBO_AVAILABLEGATEWAYS, &CMonitorGatewaysDialog::OnCbnSelchangeCboAvailablegateways)
END_MESSAGE_MAP()

/* Called when the dialog is loaded, in response to the WM_INITDIALOG message.
 */
BOOL CMonitorGatewaysDialog::OnInitDialog()
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

	// Create a new instance of our TTGate class
	m_pGate = new CGateSink(this);

	// Make sure our check boxes are checked by default
	m_chkOpenPrice.SetCheck(1);
	m_chkOpenOrder.SetCheck(1);
	m_chkOpenFill.SetCheck(1);
	m_AvailableGateways.SetCurSel(0);

	// Set our text box headers
	m_txtOnStatusUpdate.SetWindowTextA("lHintMask,  sText\r\n");	
	m_txtOnExchangeStateUpdate.SetWindowTextA("sExchange,  sText,  bOpenned,  bServerUp\r\n");	
	m_txtOnExchangeMessage.SetWindowTextA("sExchange,  sTimeStamp,  sInfo,  sText\r\n");

	// Delete the font object
	delete pFont;
	pFont = NULL;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/* Called by the framework when the application terminates.  Clean
 * up all XTAPI objects.
 */
void CMonitorGatewaysDialog::OnClose() 
{
	if (m_pGate)
	{
		m_pGate->Obj()->XTAPITerminate();
		delete m_pGate;
		m_pGate = NULL;
	}

	CDialog::OnClose();
}

/* Called when X_TRADER receives a status update
 * @param lHintMask Current X_TRADER status
 * @param sText Description of the change
 */
void CMonitorGatewaysDialog::CGateSink_OnStatusUpdate(long lHintMask, BSTR sText)
{
	// Convert the mask to a string
	CString mask;
	mask.Format(_T("%ld"), lHintMask);

	// Get the text already in the control
	CString statusUpdate;
	m_txtOnStatusUpdate.GetWindowTextA(statusUpdate);

	// Append the new status to the control
	statusUpdate += mask + ", " + CString(sText);
	m_txtOnStatusUpdate.SetWindowTextA(statusUpdate + "\r\n");	

	if (lHintMask == 1)
	{
		m_txtXTRADER.SetWindowTextA("Connected");
	}
	if (lHintMask == 32)
	{
		m_txtXTRADERPro.SetWindowTextA("Available");
	}
}

/* Called when there is an update to an exchange gateway
 * @param sExchange Name of the exchange
 * @param sSrvType Type of the server (Price, order, fill)
 * @param bOpened Whether the application is connected to the gateway
 * @param bServerUp Whether the server is active
 */
void CMonitorGatewaysDialog::CGateSink_OnExchangeStateUpdate(BSTR sExchange, BSTR sSrvType, long bOpened, long bServerUp)
{	
	// Check if the gateway is already in the list
	if (m_AvailableGateways.FindString(0, CString(sExchange)) == CB_ERR)
	{
		// Populate the combobox with all available gateways.
		m_AvailableGateways.AddString((LPCSTR)CString(sExchange));
	}

	// Get the server type (as an integer)
	int index;
	if (CString(sSrvType) == "Price")
	{
		index = PRICE;
	}
	else if ((CString(sSrvType) == "Order") || (CString(sSrvType) == "Order (Downloading)"))
	{
		index = ORDER;
	}
	else if ((CString(sSrvType) == "Fill")  || (CString(sSrvType) == "Fill (Downloading)"))
	{	
		index = FILL;
	}
	else
	{
		return;
	}

	// Create/amend the list of all exchange states.  The exchange data is stored in a
	// struct which is held in a hashmap.  
	LPEXCHANGE lpUpdatetest;
	BOOL found = m_ExchangeHash.Lookup(CString(sExchange), lpUpdatetest);

	if (!found)
	{
		// Create a new struct for the exchnage 
		LPEXCHANGE lpUpdate = new EXCHANGE;
		lpUpdate->pExchange = CString(sExchange);
		lpUpdate->pSrvType[index] = CString(sSrvType);
		lpUpdate->pOpened[index] = bOpened;
		lpUpdate->pServerUp[index] = bServerUp;

		// Store the new struct in the hashmap
		m_ExchangeHash[CString(sExchange)] = lpUpdate;
	}
	else
	{
		LPEXCHANGE lpUpdate = m_ExchangeHash[CString(sExchange)];
		lpUpdate->pSrvType[index] = CString(sSrvType);
		lpUpdate->pOpened[index] = bOpened;
		lpUpdate->pServerUp[index] = bServerUp;

		// Update the hashmap with the new values
		m_ExchangeHash[CString(sExchange)] = lpUpdate;
	}

	// If the gateway passed to this method is the one we're displaying
	// update the "Gateway Status" boxes
	CString selExchange;
	m_AvailableGateways.GetLBText(m_AvailableGateways.GetCurSel(), selExchange);
	if (CString(sExchange) == selExchange)
	{
		GatewayStatusUpdate(CString(sSrvType), bOpened, bServerUp);
	}

	// Convert the boolean values to strings
	CString opened, serverUp;
	opened.Format(_T("%ld"), bOpened);
	serverUp.Format(_T("%ld\r\n"), bServerUp);

	// Get the text already in the control
	CString exchangeUpdate;
	m_txtOnExchangeStateUpdate.GetWindowTextA(exchangeUpdate);

	// Append the new exchange update to the control
	exchangeUpdate += CString(sExchange) + ", " + CString(sSrvType) + ", " + opened + ", " + serverUp;;
	m_txtOnExchangeStateUpdate.SetWindowTextA(exchangeUpdate);	
}

/* Called when the gateway generates an informational message
 * @param sExchange Name of the exchange
 * @param sTimeStamp Time when the message was sent
 * @param sInfo Type of message (ALERT, INFO, ERROR, WARNING)
 * @param sText Text of the message
 */
void CMonitorGatewaysDialog::CGateSink_OnExchangeMessage(BSTR sExchange, BSTR sTimeStamp, BSTR sInfo, BSTR sText)
{
	// Get the text already in the control
	CString exchangeMessage;
	m_txtOnStatusUpdate.GetWindowTextA(exchangeMessage);

	// Append the new exchange update to the control
	exchangeMessage += CString(sExchange) + ", " + CString(sTimeStamp) + ", " + CString(sInfo) + ", " + CString(sText) + "\r\n";
	m_txtOnExchangeMessage.SetWindowTextA(exchangeMessage);	
}

/* Called when a user clicks the "Connect" button to connect to an exchange
 */
void CMonitorGatewaysDialog::OnBnClickedBtnConnect()
{
	CString selectedExchange;
	m_AvailableGateways.GetLBText(m_AvailableGateways.GetCurSel(), selectedExchange);

	// Open exchange prices if checked
	if (m_chkOpenPrice.GetCheck() != 0)
	{
		m_pGate->Obj()->OpenExchangePrices((LPCSTR)selectedExchange);
	}

	// Open exchange orders if checked
	if (m_chkOpenOrder.GetCheck() != 0)
	{
		m_pGate->Obj()->OpenExchangeOrders((LPCSTR)selectedExchange);
	}

	// Open exchange fills if checked
	if (m_chkOpenFill.GetCheck() != 0)
	{
		m_pGate->Obj()->OpenExchangeFills((LPCSTR)selectedExchange);
	}
}

/* Called when a user selects a new exchange.
 */
void CMonitorGatewaysDialog::OnCbnSelchangeCboAvailablegateways()
{
	// Reset all the fields prior to drawing their statuses
	m_txtPriceConnection.SetWindowTextA((LPCSTR)"");
	m_txtOrderConnection.SetWindowTextA((LPCSTR)"");
	m_txtFillConnection.SetWindowTextA((LPCSTR)"");

	// Get the statuses stored in the CMap, and output them to the GUI
	CString selectedExchange;
	m_AvailableGateways.GetLBText(m_AvailableGateways.GetCurSel(), selectedExchange);

	LPEXCHANGE lpUpdate = m_ExchangeHash[selectedExchange];
	for (int i = 0; i < NUM_SERVERS; i++)
	{
		GatewayStatusUpdate(CString(lpUpdate->pSrvType[i]), lpUpdate->pOpened[i], lpUpdate->pServerUp[i]);
	}
}

/* Publish the gateway status to the GUI
 * @param Type of the server (Price, order, fill)
 * @param bOpened Whether the application is connected to the gateway
 * @param bServerUp Whether the server is active
 */
void CMonitorGatewaysDialog::GatewayStatusUpdate(CString sText, long bOpened, long bServerUp)
{
	if (sText == "Price")
	{
		if (bOpened == 0 && bServerUp == 0)	// Server exists.
		{
			m_txtPriceConnection.SetWindowTextA((LPCSTR)"EXISTS");
		}
		if (bOpened == 1 && bServerUp == 0)	// Server down.
		{
			m_txtPriceConnection.SetWindowTextA((LPCSTR)"DOWN");
		}
		if (bOpened == 1 && bServerUp == 1)	// Server up.
		{
			m_txtPriceConnection.SetWindowTextA((LPCSTR)"UP");
		}
	}

	if (sText == "Order")
	{
		if (bOpened == 0 && bServerUp == 0)	// Server exists.
		{
			m_txtOrderConnection.SetWindowTextA((LPCSTR)"EXISTS");
		}
		if (bOpened == 1 && bServerUp == 0)	// Server down.
		{
			m_txtOrderConnection.SetWindowTextA((LPCSTR)"DOWN");
		}
		if (bOpened == 1 && bServerUp == 1)	// Server up.
		{
			m_txtOrderConnection.SetWindowTextA((LPCSTR)"UP");
		}
	}
	else if (sText == "Order (Downloading)")	// Server downloading data.
	{
		m_txtOrderConnection.SetWindowTextA((LPCSTR)"Downloading...");
	}

	if (sText == "Fill")
	{
		if (bOpened == 0 && bServerUp == 0)	// Server exists.
		{
			m_txtFillConnection.SetWindowTextA((LPCSTR)"EXISTS");
		}
		if (bOpened == 1 && bServerUp == 0)	// Server down.
		{
			m_txtFillConnection.SetWindowTextA((LPCSTR)"DOWN");
		}
		if (bOpened == 1 && bServerUp == 1)	// Server up.
		{
			m_txtFillConnection.SetWindowTextA((LPCSTR)"UP");
		}
	}
	else if (sText == "Fill (Downloading)")	// Server downloading data.
	{
		m_txtFillConnection.SetWindowTextA((LPCSTR)"Downloading...");
	}
}