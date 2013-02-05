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
#ifndef PRICEUPDATEMULTIPLEDIALOG_H
#define PRICEUPDATEMULTIPLEDIALOG_H

#include "XTAPI\InstrNotifySink.h"
#include "XTAPI\DropHandlerSink.h"

/* PriceUpdateMultiple dialog window.  Handles the methods inherited from the 
 * drop handler and instrument notify objects.
 */
class CPriceUpdateMultipleDialog : 
	public CDialog,
	public CInstrNotifySink,  // Inherit the Instrument Notification sink class.
	CDropHandlerSink::CNotify // Inherit the TTDropHandler sink class.
{
private:

	// Declare the callback functions.
	virtual void CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr);
	virtual void CInstrNotifySink_Update(XTAPI::ITTInstrObj* pInstr);
	virtual void CDropHandlerSink_NotifyDrop(const CDropHandlerSink*pSink);

	// User defined functions.
	void DisplayInstrumentInformation(XTAPI::ITTInstrObj* pInstr);

public:

	// standard constructor
	CPriceUpdateMultipleDialog(CWnd* pParent = NULL);

	// Dialog Data
	//{{AFX_DATA(CPriceUpdateDragDropDialog)
	enum { IDD = IDD_TTPRICEUPDATE_DIALOG };
	//}}AFX_DATA

protected:

	HICON m_hIcon;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPriceUpdateDragDropDialog)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CPriceUpdateDragDropDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	// Declare the sink class objects.
	CInstrNotifySink* m_pInstrNotify;
	CDropHandlerSink* m_pDropHandler;
	
	// Declare the TTInstrObj object array.
	XTAPI::ITTInstrObjPtr m_pInstrObj[4];

	// Variables mapped to controls
	CString	m_ExchangeBox1;
	CString	m_ProductBox1;
	CString	m_ProdTypeBox1;
	CString	m_ContractBox1;
	CString	m_BidBox1;
	CString	m_AskBox1;
	CString	m_LastBox1;
	CString	m_ExchangeBox2;
	CString	m_ProductBox2;
	CString	m_ProdTypeBox2;
	CString	m_ContractBox2;
	CString	m_BidBox2;
	CString	m_AskBox2;
	CString	m_LastBox2;
	CString	m_ExchangeBox3;
	CString	m_ProductBox3;
	CString	m_ProdTypeBox3;
	CString	m_ContractBox3;
	CString	m_BidBox3;
	CString	m_AskBox3;
	CString	m_LastBox3;
	CString	m_ExchangeBox4;
	CString	m_ProductBox4;
	CString	m_ProdTypeBox4;
	CString	m_ContractBox4;
	CString	m_BidBox4;
	CString	m_AskBox4;
	CString	m_LastBox4;
	CString m_StatusBar;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* PRICEUPDATEMULTIPLEDIALOG_H */