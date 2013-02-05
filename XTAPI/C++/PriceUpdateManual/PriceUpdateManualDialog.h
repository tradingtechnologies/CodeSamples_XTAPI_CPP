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
#ifndef PRICEUPDATEMANUALDIALOG_H
#define PRICEUPDATEMANUALDIALOG_H

#include "XTAPI\InstrNotifySink.h"

/* PriceUpdateManual dialog window.  Handles the methods inherited from the 
 * instrument notify objects.
 */
class CPriceUpdateManualDialog : 
	public CDialog,
	public CInstrNotifySink  
{
private:
	// Declare the callback functions.
	virtual void CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr);
	virtual void CInstrNotifySink_Update(XTAPI::ITTInstrObj* pInstr);
	virtual void CInstrNotifySink_NotFound(XTAPI::ITTInstrObj* pInstr);

	// User defined methods
	void ProcessPriceData(XTAPI::ITTInstrObj* pInstr);

public:

	// standard constructor
	CPriceUpdateManualDialog(CWnd* pParent = NULL);	

	// Dialog Data
	//{{AFX_DATA(CPriceUpdateManualDialog)
	enum { IDD = IDD_PRICEUPDATEMANUAL_DIALOG };
	//}}AFX_DATA

protected:

	HICON m_hIcon;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPriceUpdateManualDialog)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CPriceUpdateManualDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnconnect();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	// Declare the sink class object.
	CInstrNotifySink* m_pInstrNotify;
	
	// Declare the TTInstrObj object pointer.
	XTAPI::ITTInstrObjPtr m_pInstrObj;

	// Variables mapped to controls
	CEdit m_ExchangeBox;
	CEdit m_ProductBox;
	CEdit m_ProdTypeBox;
	CEdit m_ContractBox;
	CString	m_BidBox;
	CString	m_BidQtyBox;
	CString	m_AskBox;
	CString	m_AskQtyBox;
	CString	m_LastBox;
	CString m_LastQty;
	CString m_LastQtyChange;
	CString m_StatusBar;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* PRICEUPDATEMANUALDIALOG_H */