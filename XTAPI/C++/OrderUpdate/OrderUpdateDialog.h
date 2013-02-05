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
#ifndef ORDERUPDATEDIALOG_H
#define ORDERUPDATEDIALOG_H

#include "XTAPI\InstrNotifySink.h"
#include "XTAPI\DropHandlerSink.h"
#include "XTAPI\OrderSetNotifySink.h"

/* OrderUpdate dialog window.  Handles the methods inherited from the 
 * drop handler, orderset and instrument notify objects.
 */
class COrderUpdateDialog : 
	public CDialog,
	public CInstrNotifySink,			 // Inherit the Instrument Notification sink class.
	public CDropHandlerSink::CNotify,	 // Inherit the TTDropHandler sink class.
	public COrderSetNotifySink::CNotify  // Inherit the TTOrderSet sink class.
{
private:

	// Declare the callback functions.
	virtual void CInstrNotifySink_Found(XTAPI::ITTInstrObj* pInstr);
	virtual void CDropHandlerSink_NotifyDrop(const CDropHandlerSink* pSink);
	virtual void COrderSetNotifySink_OnOrderRejected(XTAPI::ITTOrderObj* pRejectedOrderObj);
	virtual void COrderSetNotifySink_OnOrderSubmitted(XTAPI::ITTOrderObj* pNewOrderObj,
													  XTAPI::ITTOrderObj* pOldOrderObj,
													  BSTR sSiteOrderKey,
													  XTAPI::enumOrderAction eOrderAction,
													  long wrkQty,
													  BSTR sOrderType,
													  BSTR sOrderTraits);
	virtual void COrderSetNotifySink_OnOrderHeld(XTAPI::ITTOrderObj* pNewOrderObj,
												 XTAPI::ITTOrderObj* pOldOrderObj,
												 BSTR sSiteOrderKey,
												 XTAPI::enumOrderAction eOrderAction,
												 long ordQty);
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
	void SendOrder(CString buySell);
	void PublishEventOrderData(XTAPI::ITTOrderObj* pNewOrderObj, XTAPI::ITTOrderObj* pOldOrderObj);

public:

	// standard constructor
	COrderUpdateDialog(CWnd* pParent = NULL);	

	// Dialog Data
	//{{AFX_DATA(COrderUpdateDialog)
	enum { IDD = IDD_ORDERUPDATE_DIALOG };
	//}}AFX_DATA

protected:

	HICON m_hIcon;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderUpdateDialog)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(COrderUpdateDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSellButton();
	afx_msg void OnBuyButton();
	afx_msg void OnClose();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	// Declare the sink class objects.
	CInstrNotifySink* m_pInstrNotify;
	CDropHandlerSink* m_pDropHandler;
	COrderSetNotifySink* m_pOrderSet;

	// Declare the XTAPI smart pointers.
	XTAPI::ITTInstrObjPtr m_pInstrObj;

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
	CEdit m_OrderStatusEdit;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* ORDERUPDATEDIALOG_H */
