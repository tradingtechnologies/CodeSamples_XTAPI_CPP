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
#include "MarketExplorer.h"
#include "MarketExplorerDialog.h"

/* MEAPI dialog constructor.  Initialize the data mappings and
 * instantiate the instrument notifier.
 */
CMarketExplorerDialog::CMarketExplorerDialog(CWnd* pParent /*=NULL*/) : CDialog(CMarketExplorerDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

/* Called by the framework.
 * @see http://msdn.microsoft.com/en-us/library/x4d0y489%28v=vs.80%29.aspx
 */
void CMarketExplorerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CMarketExplorerDialog)
	DDX_Control(pDX, IDC_TXT_STATUSBAR, m_StatusBar);
	DDX_Control(pDX, IDC_LB_GATEWAYS, m_lbGateways);
	DDX_Control(pDX, IDC_LB_PRODUCTTYPES, m_lbProductTypes);
	DDX_Control(pDX, IDC_LB_INFO, m_lbInfo);
	DDX_Control(pDX, IDC_TREE_PRODUCTS, m_tvProducts);
	//}}AFX_DATA_MAP
}

/* Begins the definition of your message map.
 * @see http://msdn.microsoft.com/en-us/library/9x1t4dy1%28v=vs.80%29.aspx
 */
BEGIN_MESSAGE_MAP(CMarketExplorerDialog, CDialog)
	//{{AFX_MSG_MAP(CMarketExplorerDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_MEAPI, &CMarketExplorerDialog::OnBnClickedRequestGateways)
	ON_LBN_SELCHANGE(IDC_LB_GATEWAYS, &CMarketExplorerDialog::OnLbnSelchangeLbGateways)
	ON_LBN_SELCHANGE(IDC_LB_PRODUCTTYPES, &CMarketExplorerDialog::OnLbnSelchangeLbProducttypes)
END_MESSAGE_MAP()

/* Called when the dialog is loaded, in response to the WM_INITDIALOG message.
 */
BOOL CMarketExplorerDialog::OnInitDialog()
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
	
	// Create a new instance of our MarketExplorer class
	m_pMeapi = new CMeapiSink();

	m_ProductCollection = NULL;
	m_SelectedGateway = NULL;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/* Called by the framework when the application terminates.
 */
void CMarketExplorerDialog::OnClose() 
{
	if (m_pMeapi)
	{
		m_pMeapi->Obj()->MEAPITerminate();
		delete m_pMeapi;
		m_pMeapi = NULL;
	}

	CDialog::OnClose();
}

/* TT Gateways are found in response to a RequestGateways method call or when
 * TT Gateways become available.
 * @param gateway The TT Gateway object found by the MEAPI
 */
void CMarketExplorerDialog::CMeapiSink_OnGateway(MEAPI::ITTGateway* gateway)
{
	// Add the gateway to the collection so we can create our event handlers
	if (!m_GatewayCollection.Lookup(CString((BSTR)gateway->Name), gateway))
	{
		m_GatewayCollection[CString((BSTR)gateway->Name)] = gateway;
	}
	
	// publish the gateway to the UI
	PublishGatewayName(CString((BSTR)gateway->Name));
}

/* MEAPI recieves the gateway attribute list (GAL settings) for the TT Gateway.
 * @param pGateway TT Gateway whose GAL settings changed
 * @param attributeList Comma-separated list of GAL name-value pairs
 * @param service Service that started or restarted
 */
void CMarketExplorerDialog::CMeapiSink_OnGatewayAttributes(MEAPI::ITTGateway* pGateway, BSTR attributeList, MEAPI::enumServiceType service)
{
	// Get the selected text in the ListBox
	CString selectedText;
	int index = m_lbGateways.GetCurSel();
	m_lbGateways.GetText(index, selectedText);

	// request the products and product types for our selected gateway
	if (CString((BSTR)pGateway->Name) == selectedText)
	{
		pGateway->RequestProductTypes();
		pGateway->RequestProducts(0);
	}
}

/* TT Gateways return a list of products hosted on the TT Gateway
 * @param pGateway TT Gateway whose products are requested
 * @param attributeList Prodcut collection from the gateway
 */
void CMarketExplorerDialog::CMeapiSink_OnProducts(MEAPI::ITTGateway* pGateway, MEAPI::ITTProductCollection* products)
{
	// set our global values containing our products
	m_ProductCollection = products;

	m_lbInfo.AddString("Products delivered for: " + CString((BSTR)pGateway->Name));
}

/* TT Gateways timeout on a product request
 * @param pGateway TT Gateway which timed out
 */
void CMarketExplorerDialog::CMeapiSink_OnRequestProductsTimeout(MEAPI::ITTGateway* pGateway)
{
	m_lbInfo.AddString("Product Request Timeout for: " + CString((BSTR)pGateway->Name));
}

/* Status of a TT Gateway server change
 * @param pGateway Gateway affected by the server status change
 * @param service Service whose status changed
 * @param isAvailable Current status of the service
 * @param isLoggedIn Current logged in status
 */
void CMarketExplorerDialog::CMeapiSink_OnServerStatus(MEAPI::ITTGateway* pGateway, MEAPI::enumServiceType service, long isAvailable, long isLoggedIn)
{
	// By default, set the gateway to unavailable
	m_IsGatewayAvailable = false;

	// Publish the status to the UI
	CString gatewayName = CString((BSTR)pGateway->Name);
	CString available, loggedin;
	available.Format("%i", isAvailable);
	loggedin.Format("%i", isLoggedIn);

	m_lbInfo.AddString("Server Status :: Gateway: " + gatewayName + " | " + "Available: " + available + " | Logged In: " + loggedin);
	
	CString outstring = CString((BSTR)pGateway->Name) + ": Connecting to Price Server...";
	m_StatusBar.SetWindowTextA((LPCSTR)outstring);

	if (service == MEAPI::PRICE_SERVICE)
	{
		if ((isAvailable != 0) && (isLoggedIn != 0))
			m_IsGatewayAvailable = true;
	}
}

/* TT Gateways return a list of product types hosted on the TT Gateway
 * @param pGateway TT Gateway containing the product types
 * @param productTypes Array of product type IDs
 * @param productTypesAsString Array of product type names
 */
void CMarketExplorerDialog::CMeapiSink_OnProductTypes(MEAPI::ITTGateway* pGateway, VARIANT productTypes, VARIANT productTypesAsString)
{
	// Retrieve the list of the product types and publish to the UI
	long lBound = 0, uBound = 0;

	SAFEARRAY* pSafeArray  = productTypesAsString.parray;
	SafeArrayGetLBound(pSafeArray, 1, &lBound);
	SafeArrayGetUBound(pSafeArray, 1, &uBound);

	BSTR prodTypeText;
	for (long i = lBound; i <= uBound; i++)
	{
		SafeArrayGetElement(pSafeArray, &i, &prodTypeText);
		m_lbProductTypes.AddString(CString(prodTypeText));
	}

	CString numProductTypes;
	numProductTypes.Format("%i", (uBound - lBound + 1));

	// Update the Status Label
	CString outstring = "Product Types Found for " + CString((BSTR)pGateway->Name) + ": " + numProductTypes;
	m_StatusBar.SetWindowTextA((LPCSTR)outstring);
}

/* TT Gateway recieves new products from an exchange.
 * @param pGateway TT Gateway on which the product was added
 * @param pProduct New Product
 */
void CMarketExplorerDialog::CMeapiSink_OnNewProduct(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct)
{
	m_lbInfo.AddString("New Product Found: " + CString((BSTR)pProduct->Name));
}

/* TT Gateways add new contracts for a product
 * @param pGateway TT Gateway on which the contract was added
 * @param pProduct Product associated with the new contract
 * @param pContractInfo New Contract
 */
void CMarketExplorerDialog::CMeapiSink_OnNewContract(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct, MEAPI::ITTContractInfo* pContractInfo)
{
	m_lbInfo.AddString("New Contract Found: " + CString((BSTR)pProduct->Name) + " " + CString((BSTR)pContractInfo->Name));
}

/* TT Gateways deliver a list of contracts associated with the product
 * @param pGateway TT Gateway on which the product contracts trade
 * @param pProduct Product associated with the contracts
 * @param contracts Collection of the contracts available for the product
 */
void CMarketExplorerDialog::CMeapiSink_OnContracts(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct, MEAPI::ITTContractCollection* contracts)
{
	// Place the product as the root node
	HTREEITEM prodRoot = m_tvProducts.InsertItem((LPCSTR)pProduct->Name, TVI_ROOT, TVI_LAST);

	// Iterate through the contract collection and place them in the tree
	for (long i = 0; i < contracts->Count; i++)
	{
		_variant_t vContract;
		if (SUCCEEDED(contracts->get_Item(_variant_t(i), &vContract)))
		{
			MEAPI::ITTContractInfoPtr contract = MEAPI::ITTContractInfoPtr(vContract);
			m_tvProducts.InsertItem((LPCSTR)contract->Name, prodRoot, TVI_LAST);

			contract = NULL;
		}
	}
}

/* Contract download times out
 * @param pGateway TT Gateway on which the contracts product trades
 * @param pProduct Product associated with the contract
 */
void CMarketExplorerDialog::CMeapiSink_OnRequestContractsTimeout(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct)
{
	m_lbInfo.AddString("New Contract Found: " + CString((BSTR)pGateway->Name) + " " + CString((BSTR)pProduct->Name));
}

/* Triggered when the "Request Gateways" button is clicked
 */
void CMarketExplorerDialog::OnBnClickedRequestGateways()
{
	MEAPI::enumRegisterReturnCode code;

	// Register an event handler to the ITTMarketExplorerEvents
	CMarketExplorerEventsSink* eventHandler = dynamic_cast<CMarketExplorerEventsSink*>(this);
	if (SUCCEEDED(m_pMeapi->Obj()->raw_RegisterEventHandler((IUnknown*)eventHandler, &code)))
	{
		if (code == MEAPI::RR_SUCCESS)
		{
			m_StatusBar.SetWindowTextA("Requesting Gateways...");
			m_ExchangeCounter = 0;

			// Request our list of gateways
			m_pMeapi->Obj()->RequestGateways();
		}
		else
		{
			m_StatusBar.SetWindowTextA((LPCSTR)"Error requesting gateways.");
		}
	}
	else
	{
		m_StatusBar.SetWindowTextA((LPCSTR)"Error requesting gateways.");
	}
}

/* Triggered when a gateway is selected
 */
void CMarketExplorerDialog::OnLbnSelchangeLbGateways()
{
	// Release product event handlers
	ReleaseProductEventHandlers();

	// Clear out the UI
	m_lbProductTypes.ResetContent();
	m_tvProducts.DeleteAllItems();
	
	// Clear out any previous product collections
	m_ProductCollection = NULL;

	CString selectedText;
	int index = m_lbGateways.GetCurSel();
	m_lbGateways.GetText(index, selectedText);

	CGatewayEventsSink* eventHandler = dynamic_cast<CGatewayEventsSink*>(this);

	// Unregister the existing event handler (if exists), and register the new one
	if (m_SelectedGateway != NULL)
	{
		m_SelectedGateway->UnregisterEventHandler((IUnknown*)eventHandler);
	}

	m_SelectedGateway = (MEAPI::ITTGateway*)m_GatewayCollection[selectedText];

	// Register new event handler
	MEAPI::enumRegisterReturnCode code;
	if (SUCCEEDED(m_SelectedGateway->raw_RegisterEventHandler((IUnknown*)eventHandler, &code)))
	{
		if (code != MEAPI::RR_SUCCESS)
		{
			m_StatusBar.SetWindowTextA("Error registering gateway event handler.");
		}
	}
	else
	{
		m_StatusBar.SetWindowTextA("Error registering gateway event handler.");
	}
}

/* Triggered when a product type is selected
 */
void CMarketExplorerDialog::OnLbnSelchangeLbProducttypes()
{
	if (m_ProductCollection == NULL)
	{
		MessageBox("Products have not yet been delivered to the application.  Please wait, and try again.", "Error", MB_OK);
		return;
	}

	m_StatusBar.SetWindowTextA("Requesting Products...");

	// Release product event handlers
	ReleaseProductEventHandlers();

	// Clear out the UI
	m_tvProducts.DeleteAllItems();

	// Get the selected text in the ListBox
	CString selectedText;
	int index = m_lbProductTypes.GetCurSel();
	m_lbProductTypes.GetText(index, selectedText);

	// Return codes
	MEAPI::enumRegisterReturnCode regCode;
	MEAPI::enumQueryReturnCode queryCode;

	// Event handler
	CProductEventsSink* eventHandler = dynamic_cast<CProductEventsSink*>(this);

	int productCount = 0;

	// Iterate through the collection, and request the contracts for the requested product type
	for (long i = 0; i < m_ProductCollection->Count; i++)
	{
		// MEAPI product variable
		MEAPI::ITTProductPtr product;

		_variant_t vProduct;
		if (SUCCEEDED(m_ProductCollection->get_Item(_variant_t(i), &vProduct)))
		{
			product = MEAPI::ITTProductPtr(vProduct);
		}
		else
		{
			continue;
		}

		// Make sure the product type selected matches what we're registering
		if (CString((BSTR)product->TypeAsString) == selectedText)
		{
			product->raw_RegisterEventHandler((IUnknown*)eventHandler, &regCode);

			if (regCode == MEAPI::RR_SUCCESS)
			{
				product->raw_RequestContracts(&queryCode);
				productCount++;
			}
			else
			{
				m_lbInfo.AddString("ERROR: Cannot register Event Handler for " + CString((BSTR)product->Name));
			}
		}

		product = NULL;
	}

	CString count;
	count.Format("%i", productCount);
	m_StatusBar.SetWindowTextA("Products Found: " + productCount);
}

/* Add gateway to the GUI
 */
void CMarketExplorerDialog::PublishGatewayName(CString gatewayName)
{
	// Add the gateways to the listbox
	m_lbGateways.AddString(gatewayName);

	// Increment the number of exchanges and push it to the UI
	m_ExchangeCounter++;

	CString exchangeCount;
	exchangeCount.Format("%i", m_ExchangeCounter);
	m_StatusBar.SetWindowTextA("Exchanges Found: " + exchangeCount);
}

/* Releases the event handlers for a product
 */
void CMarketExplorerDialog::ReleaseProductEventHandlers()
{
	m_StatusBar.SetWindowTextA("Releasing Product Event Handlers...");

	if (m_ProductCollection == NULL)
		return;

	// Event handler
	CProductEventsSink* eventHandler = dynamic_cast<CProductEventsSink*>(this);

	// Iterate through the collection, and request the contracts for the requested product type
	for (long i = 0; i < m_ProductCollection->Count; i++)
	{
		_variant_t vProduct;
		if (SUCCEEDED(m_ProductCollection->get_Item(_variant_t(i), &vProduct)))
		{
			MEAPI::ITTProductPtr product = MEAPI::ITTProductPtr(vProduct);
			product->UnregisterEventHandler((IUnknown*)eventHandler);

			product = NULL;
		}
	}
}