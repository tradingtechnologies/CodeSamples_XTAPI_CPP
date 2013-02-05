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
#ifndef PROFITLOSSDIALOG_H
#define PROFITLOSSDIALOG_H

#include "XTAPI\InstrNotifySink.h"
#include "XTAPI\DropHandlerSink.h"

/* PriceUpdateMultiple dialog window.  Handles the methods inherited from the 
 * drop handler and instrument notify objects.
 */
class CProfitLossDialog : 
	public CDialog,
	public CInstrNotifySink,  // Inherit the Instrument Notification sink class.
	CDropHandlerSink::CNotify // Inherit the TTDropHandler sink class.
{
private:

	// Declare the callback functions.
	virtual void CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr);
	virtual void CInstrNotifySink_Update(XTAPI::ITTInstrObj* pInstr);
	virtual void CInstrNotifySink_OrderSet(XTAPI::ITTInstrObj* pInstr, XTAPI::ITTOrderSet* pOrderSet);
	virtual void CDropHandlerSink_NotifyDrop(const CDropHandlerSink*pSink);

	// User defined functions.
	void DisplayPLInformation(XTAPI::ITTInstrObj* pInstr);

public:
	
	// standard constructor
	CProfitLossDialog(CWnd* pParent = NULL);	

	// Dialog Data
	//{{AFX_DATA(CProfitLossDialog)
	enum { IDD = IDD_PROFITLOSS_DIALOG };
	//}}AFX_DATA

protected:

	HICON m_hIcon;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfitLossDialog)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CProfitLossDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	// Declare the sink class objects.
	CInstrNotifySink* m_pInstrNotify;
	CDropHandlerSink* m_pDropHandler;
	
	// Declare the XTAPI smart pointers
	XTAPI::ITTInstrObjPtr m_pInstrObj[4];
	XTAPI::ITTOrderSetPtr m_pOrderSet;

	// Variables mapped to controls
	CString	m_ExchangeBox1;
	CString	m_ProductBox1;
	CString	m_ProdTypeBox1;
	CString	m_ContractBox1;
	CString	m_PLBox1;
	CString	m_NetPosBox1;
	CString	m_ExchangeBox2;
	CString	m_ProductBox2;
	CString	m_ProdTypeBox2;
	CString	m_ContractBox2;
	CString	m_PLBox2;
	CString	m_NetPosBox2;
	CString	m_ExchangeBox3;
	CString	m_ProductBox3;
	CString	m_ProdTypeBox3;
	CString	m_ContractBox3;
	CString	m_PLBox3;
	CString	m_NetPosBox3;
	CString	m_ExchangeBox4;
	CString	m_ProductBox4;
	CString	m_ProdTypeBox4;
	CString	m_ContractBox4;
	CString	m_PLBox4;
	CString	m_NetPosBox4;
	CString m_StatusBar;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* PROFITLOSSDIALOG_H */