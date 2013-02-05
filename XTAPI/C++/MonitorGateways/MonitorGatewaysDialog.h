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
#ifndef MONITORGATEWAYSDIALOG_H
#define MONITORGATEWAYSDIALOG_H

#include "XTAPI\GateSink.h"

// Server types
#define NUM_SERVERS (3)
#define PRICE 0
#define ORDER 1
#define FILL 2

/* MonitorGateways dialog window.  Handles the methods inherited from the 
 * drop handler and instrument notify objects.
 */
class CMonitorGatewaysDialog : 
	public CDialog,
	public CGateSink // Inherit the TTGate notification sink class
{
private:

	// Declare the callback functions.
	virtual void CGateSink_OnStatusUpdate(long lHintMask, BSTR sText);
	virtual void CGateSink_OnExchangeStateUpdate(BSTR sExchange, BSTR sText, long bOpened, long bServerUp);
	virtual void CGateSink_OnExchangeMessage(BSTR sExchange, BSTR sTimeStamp, BSTR sInfo, BSTR sText);

	// User defined functions.
	void GatewayStatusUpdate(CString sText, long bOpened, long bServerUp);

public:

	// standard constructor
	CMonitorGatewaysDialog(CWnd* pParent = NULL);

	// Dialog Data
	//{{AFX_DATA(CMonitorGatewaysDialog)
	enum { IDD = IDD_MONITORGATEWAYS_DIALOG };
	//}}AFX_DATA

protected:

	HICON m_hIcon;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonitorGatewaysDialog)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMonitorGatewaysDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnCbnSelchangeCboAvailablegateways();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	// Declare the sink class object.
	CGateSink* m_pGate;

	// Variables mapped to controls
	CEdit m_txtPriceConnection;
	CEdit m_txtOrderConnection;
	CEdit m_txtFillConnection;
	CEdit m_txtXTRADER;
	CEdit m_txtXTRADERPro;
	CEdit m_txtOnStatusUpdate;
	CEdit m_txtOnExchangeStateUpdate;
	CEdit m_txtOnExchangeMessage;
	CComboBox m_AvailableGateways;
	CButton m_btnConnect;
	CButton m_chkOpenPrice;
	CButton m_chkOpenOrder;
	CButton m_chkOpenFill;
	CString m_StatusBar;

	// Struct to store exchange statuses
	typedef struct _EXCHANGE 
	{
		CString pExchange; 
		CString pSrvType[NUM_SERVERS];
		long pOpened[NUM_SERVERS];
		long pServerUp[NUM_SERVERS];
	} EXCHANGE, *LPEXCHANGE;

	// Hashmap of all exchange status structs
	CMap<CString, LPCSTR, LPEXCHANGE, LPEXCHANGE> m_ExchangeHash;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* MONITORGATEWAYSDIALOG_H */