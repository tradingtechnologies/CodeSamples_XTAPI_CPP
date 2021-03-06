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
#ifndef ORDERSELECTORDIALOG_H
#define ORDERSELECTORDIALOG_H

#include "XTAPI\InstrNotifySink.h"
#include "XTAPI\DropHandlerSink.h"
#include "XTAPI\OrderSetNotifySink.h"

/* SubmitOrder dialog window.  Handles the methods inherited from the 
 * drop handler and instrument notify objects.
 */
class COrderSelectorDialog : 
	public CDialog,
	public CInstrNotifySink,			 // Inherit the Instrument Notification sink class.
	public CDropHandlerSink::CNotify,	 // Inherit the TTDropHandler sink class.
	public COrderSetNotifySink::CNotify  // Inherit the TTOrderSet sink class.
{
private:

	// Declare the callback functions.
	virtual void CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr);
	virtual void CDropHandlerSink_NotifyDrop(const CDropHandlerSink*pSink);
	virtual void COrderSetNotifySink_Update(XTAPI::ITTOrderSet * pOrderSet);

	// Declare the private methods
	void SendOrder(CString buySell);
	
public:

	// standard constructor
	COrderSelectorDialog(CWnd* pParent = NULL);	

	// Dialog Data
	//{{AFX_DATA(COrderSelectorDialog)
	enum { IDD = IDD_ORDERSELECTOR_DIALOG };
	//}}AFX_DATA

protected:

	HICON m_hIcon;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderSelectorDialog)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(COrderSelectorDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSellButton();
	afx_msg void OnBuyButton();
	afx_msg void OnBnClickedSelectorbutton();
	afx_msg void OnClose();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	// Declare the sink class objects.
	CInstrNotifySink* m_pInstrNotify;
	CDropHandlerSink* m_pDropHandler;
	COrderSetNotifySink* m_pOrderSetReceive;

	// Declare XTAPI objects.
	XTAPI::ITTInstrObjPtr m_pInstrObj;
	XTAPI::ITTOrderSetPtr m_pOrderSetSend;
	XTAPI::ITTOrderSelectorPtr m_pOrderSelector;

	// Variables mapped to controls
	CString	m_ExchangeBox;
	CString	m_ProductBox;
	CString	m_ProdTypeBox;
	CString	m_ContractBox;	
	CString m_StatusBar;
	CComboBox m_CustomerCombo;
	CEdit m_PriceEdit;
	CEdit m_QuantityEdit;
	CButton m_BuyButton;
	CButton m_SellButton;
	CButton m_XTRADERCheck;
	CButton m_XTAPICheck;
	CButton m_BuyCheck;
	CButton m_SellCheck;
	CButton m_ContractCheck;
	CEdit m_ContractEdit;
	CButton m_ExchangeCheck;
	CEdit m_ExchangeEdit;
	CButton m_AllMatchesRadio;
	CButton m_AnyMatchesRadio;
	CButton m_SelectorButton;
	CEdit m_OrderBookEdit;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* ORDERSELECTORDIALOG_H */