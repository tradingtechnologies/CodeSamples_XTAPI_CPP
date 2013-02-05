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
#ifndef MODIFYORDERDIALOG_H
#define MODIFYORDERDIALOG_H

#include "XTAPI\InstrNotifySink.h"
#include "XTAPI\DropHandlerSink.h"

/* ModifyOrder dialog window.  Handles the methods inherited from the 
 * drop handler and instrument notify objects.
 */
class CModifyOrderDialog : 
	public CDialog,
	public CInstrNotifySink,			 // Inherit the Instrument Notification sink class.
	public CDropHandlerSink::CNotify	 // Inherit the TTDropHandler sink class.
{
private:

	// Declare the callback functions.
	virtual void CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr);
	virtual void CDropHandlerSink_NotifyDrop(const CDropHandlerSink*pSink);

	// Declare the private methods
	void SendOrder(CString buySell);

public:
	
	// standard constructor
	CModifyOrderDialog(CWnd* pParent = NULL);	

	// Dialog Data
	//{{AFX_DATA(CModifyOrderDialog)
	enum { IDD = IDD_TTModifyOrder_DIALOG };
	//}}AFX_DATA

protected:

	HICON m_hIcon;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModifyOrderDialog)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CModifyOrderDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBuybutton();
	afx_msg void OnBnClickedSellbutton();
	afx_msg void OnBnClickedModifybutton();
	afx_msg void OnBnClickedDeletebutton();
	afx_msg void OnClose();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	// Declare the sink class objects.
	CInstrNotifySink* m_pInstrNotify;
	CDropHandlerSink* m_pDropHandler;

	// Declare the XTAPI smart pointers.
	XTAPI::ITTInstrObjPtr m_pInstrObj;
	XTAPI::ITTOrderSetPtr m_pOrderSet;

	// Variables mapped to controls
	CString	m_ExchangeBox;
	CString	m_ProductBox;
	CString	m_ProdTypeBox;
	CString	m_ContractBox;
	CString m_SOKBox;
	CString m_StatusBar;
	CEdit m_HighPriceBox;
	CEdit m_LowPriceBox;
	CEdit m_PriceBox;
	CEdit m_QtyBox;
	CEdit m_NewPriceBox;
	CEdit m_NewQtyBox;
	CComboBox m_CustomerCombo;
	CComboBox m_ModifyTypeCombo;
	CComboBox m_BuySellCombo;
	CComboBox m_DeleteOrderCombo;
	CButton m_ModifyButton;
	CButton m_DeleteButton;
	CButton m_IncRadioButton;
	CButton m_ExcRadioButton;
	
	// Store the last SOK for modification
	CString m_LastOrderSiteOrderKey;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* MODIFYORDERDIALOG_H */
