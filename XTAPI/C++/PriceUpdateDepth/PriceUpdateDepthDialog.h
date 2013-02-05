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
#ifndef PRICEUPDATEDEPTHDIALOG_H
#define PRICEUPDATEDEPTHDIALOG_H

#include "XTAPI\InstrNotifySink.h"
#include "XTAPI\DropHandlerSink.h"

/* PriceUpdateDepth dialog window.  Handles the methods inherited from the 
 * drop handler and instrument notify objects.
 */
class CPriceUpdateDepthDialog : 
	public CDialog,
	public CInstrNotifySink,  // Inherit the Instrument Notification sink class.
	CDropHandlerSink::CNotify // Inherit the TTDropHandler sink class.
{
private:

	// Declare the callback functions.
	virtual void CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr);
	virtual void CInstrNotifySink_Depth(XTAPI::ITTInstrObj* pInstr);
	virtual void CDropHandlerSink_NotifyDrop(const CDropHandlerSink*pSink);

public:

	// standard constructor
	CPriceUpdateDepthDialog(CWnd* pParent = NULL);	

	// Dialog Data
	//{{AFX_DATA(CPriceUpdateDepthDialog)
	enum { IDD = IDD_TTPRICEUPDATE_DIALOG };
	//}}AFX_DATA

protected:

	HICON m_hIcon;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPriceUpdateDepthDialog)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CPriceUpdateDepthDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedChkenabledepth();
	afx_msg void OnCbnSelchangeCombobiddepth();
	afx_msg void OnCbnSelchangeComboaskdepth();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	// Declare the sink class object.
	CInstrNotifySink* m_pInstrNotify;
	CDropHandlerSink* m_pDropHandler;
	
	// Declare the TTInstrObj object pointer.
	XTAPI::ITTInstrObjPtr m_pInstrObj;

	// Variables mapped to controls
	CString	m_ExchangeBox;
	CString	m_ProductBox;
	CString	m_ProdTypeBox;
	CString	m_ContractBox;
	CString m_StatusBar;
	CComboBox m_BidDepthCombo;
	CComboBox m_AskDepthCombo;
	CListBox m_BidDepthList;
	CListBox m_AskDepthList;
	CButton m_EnableDepth;

	// Private member variables
	CString m_bidDepthValue;
	CString m_askDepthValue;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* PRICEUPDATEDEPTHDIALOG_H */
