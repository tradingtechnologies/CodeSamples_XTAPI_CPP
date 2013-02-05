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
#include "FillUpdate.h"
#include "FillUpdateDialog.h"

/* FillUpdate dialog constructor.  Initialize the data mappings and
 * instantiate the instrument notifier.
 */
CFillUpdateDialog::CFillUpdateDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFillUpdateDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//{{AFX_DATA_INIT(CFillUpdateDialog)
	m_StatusBar = _T("Downloading Fill Information...  (This may take a moment)");
	m_FillOutput = _T("Contract,  Price,  Qty,  FillType,  OrderNo,  SiteOrderKey\r\n\r\n");
	//}}AFX_DATA_INIT
	
	// Instantiate the OrderSetNotifySink Class
	m_pOrderSet = new COrderSetNotifySink();
}

/* Called by the framework.
 * @see http://msdn.microsoft.com/en-us/library/x4d0y489%28v=vs.80%29.aspx
 */
void CFillUpdateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CFillUpdateDialog)
	DDX_Text(pDX, IDC_STATUSBAR, m_StatusBar);
	DDX_Text(pDX, IDC_FILLOUTPUT, m_FillOutput);
	//}}AFX_DATA_MAP
}

/* Begins the definition of your message map.
 * @see http://msdn.microsoft.com/en-us/library/9x1t4dy1%28v=vs.80%29.aspx
 */
BEGIN_MESSAGE_MAP(CFillUpdateDialog, CDialog)
	//{{AFX_MSG_MAP(CFillUpdateDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/* Called when the dialog is loaded, in response to the WM_INITDIALOG message.
 */
BOOL CFillUpdateDialog::OnInitDialog()
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

	// Enable fills
	m_pOrderSet->Obj()->EnableOrderFillData = 1L;
	// Set this class to receive the XTAPI TTOrderSet events.
	m_pOrderSet->SetCallback(this);	
	// Open the TTOrderSet directly.
	m_pOrderSet->Obj()->Open(false);

	// Delete the font object
	delete pFont;
	pFont = NULL;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/* Called by the framework when the application terminates.  Clean
 * up all XTAPI objects.
 */
void CFillUpdateDialog::OnClose() 
{
	// Terminate the XTAPI
	XTAPI::ITTGatePtr gate;
	gate.CreateInstance(__uuidof(XTAPI::TTGate));

	gate->XTAPITerminate();
	gate = NULL;

	// Release TTOrderSet if it exists
	if (m_pOrderSet)
	{
		delete m_pOrderSet;
		m_pOrderSet = NULL;
	}

	CDialog::OnClose();
}

/* Called when a fill has been recieved
 * @param pFillObj TTFillObj representing the fill
 */
void CFillUpdateDialog::COrderSetNotifySink_FillData(XTAPI::ITTFillObj *pFillObj)
{
	m_StatusBar = _T("Fill Recieved.");

	// extract an array containing all of our required fields.
	COleSafeArray* lpData = new COleSafeArray(pFillObj->GetGet((LPCSTR)"Contract,Price,Qty,FillType,OrderNo,SiteOrderKey"));
	
	// Print the Get properties to the UI.
	CString fillEventText;
	long uBound = 0L;
	_variant_t vData;

	lpData->GetUBound(1, &uBound);
	for (long i = 0; i <= uBound; i++)
	{
		lpData->GetElement(&i, &vData);
		fillEventText += CString(vData);

		if (i != uBound)
			fillEventText += CString(",  ");
	}

	m_FillOutput += fillEventText + CString("\r\n");

	// Call after updating the AFX_DATA fields
	UpdateData(false);
}

/* Called when a block of fills is about to begin
 */
void CFillUpdateDialog::COrderSetNotifySink_FillBlockStart()
{
	m_FillOutput += CString("FillBlockStart\r\n");

	// Call after updating the AFX_DATA fields
	UpdateData(false);
}

/* Called when the block of fills is finished
 */
void CFillUpdateDialog::COrderSetNotifySink_FillBlockEnd()
{
	m_FillOutput += CString("FillBlockEnd\r\n");

	// Call after updating the AFX_DATA fields
	UpdateData(false);
}