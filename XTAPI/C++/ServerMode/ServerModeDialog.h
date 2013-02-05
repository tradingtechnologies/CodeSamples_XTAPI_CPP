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

#pragma once
#ifndef SERVERMODEDIALOG_H
#define SERVERMODEDIALOG_H

#include "XTAPI\OrderSetNotifySink.h"

/* ServerMode dialog window.  Handles the methods inherited from the 
 * drop handler and instrument notify objects.
 */
class CServerModeDialog : 
	public CDialog,
	public COrderSetNotifySink::CNotify  // Inherit the TTOrderSet sink class.
{
private:

	// Declare the callback functions.
	virtual void COrderSetNotifySink_OnOrderSubmitted(XTAPI::ITTOrderObj* pNewOrderObj,
													  XTAPI::ITTOrderObj* pOldOrderObj,
													  BSTR sSiteOrderKey,
													  XTAPI::enumOrderAction eOrderAction,
													  long wrkQty,
													  BSTR sOrderType,
													  BSTR sOrderTraits);
	virtual void COrderSetNotifySink_OnOrderUpdated(XTAPI::ITTOrderObj* pNewOrderObj,
													XTAPI::ITTOrderObj* pOldOrderObj,
													BSTR sSiteOrderKey,
													XTAPI::enumOrderNotifyState eOrderState,
													XTAPI::enumOrderAction eOrderAction,
													long updQty, 
													BSTR sOrderType, 
													BSTR sOrderTraits);
	virtual void COrderSetNotifySink_OnOrderDeleted(XTAPI::ITTOrderObj* pNewOrderObj,
													XTAPI::ITTOrderObj* pOldOrderObj,
													BSTR sSiteOrderKey,
													XTAPI::enumOrderNotifyState eOrderState,
													XTAPI::enumOrderAction eOrderAction,
													long delQty);
	virtual void COrderSetNotifySink_OnOrderFilled(XTAPI::ITTOrderObj* pNewOrderObj,
												   XTAPI::ITTOrderObj* pOldOrderObj,
												   BSTR sSiteOrderKey,
												   XTAPI::enumOrderNotifyState eOrderState,
												   long fillQty);

	// Declare the private methods
	void PublishEventOrderData(CString callingMethod, XTAPI::ITTOrderObj* pNewOrderObj, XTAPI::ITTOrderObj* pOldOrderObj);

public:

	// standard constructor
	CServerModeDialog(CWnd* pParent = NULL);	

	// Dialog Data
	//{{AFX_DATA(CMonitorGatewaysDialog)
	enum { IDD = IDD_SERVERMODE_DIALOG };
	//}}AFX_DATA

protected:

	HICON m_hIcon;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerModeDialog)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMonitorGatewaysDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnconnect();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	// Declare the sink class objects.
	COrderSetNotifySink* m_pOrderSet;

	// Declare the XTAPI smart pointers.
	XTAPI::ITTGatePtr m_pGate;

	// Variables mapped to controls
	CEdit m_UsernameBox;
	CEdit m_PasswordBox;
	CEdit m_OrderUpdateBox;
	CString m_StatusBar;
	CButton m_btnConnect;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* SERVERMODEDIALOG_H */