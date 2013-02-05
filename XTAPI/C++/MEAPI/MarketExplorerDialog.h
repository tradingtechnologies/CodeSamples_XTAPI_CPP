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
#ifndef MARKETEXPLORERDIALOG_H
#define MARKETEXPLORERDIALOG_H

#include "MEAPISink.h"

/* MarketExplorer dialog window.  Handles the methods inherited from the 
 * drop handler and instrument notify objects.
 */
class CMarketExplorerDialog : 
	public CDialog, 
	public CMeapiSink	// Inherit the MEAPI Sink class
{
private:

	// Declare the callback functions.
	virtual void CMeapiSink_OnGateway(MEAPI::ITTGateway* pGateway);
	virtual void CMeapiSink_OnGatewayAttributes(MEAPI::ITTGateway* pGateway, BSTR attributeList, MEAPI::enumServiceType service);
	virtual void CMeapiSink_OnProducts(MEAPI::ITTGateway* pGateway, MEAPI::ITTProductCollection* products);
	virtual void CMeapiSink_OnRequestProductsTimeout(MEAPI::ITTGateway* pGateway);
	virtual void CMeapiSink_OnServerStatus(MEAPI::ITTGateway* pGateway, MEAPI::enumServiceType service, long isAvailable, long isLoggedIn);
	virtual void CMeapiSink_OnProductTypes(MEAPI::ITTGateway* pGateway, VARIANT productTypes, VARIANT productTypesAsString);
	virtual void CMeapiSink_OnNewProduct(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct);
	virtual void CMeapiSink_OnNewContract(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct, MEAPI::ITTContractInfo* pContractInfo);
	virtual void CMeapiSink_OnContracts(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct, MEAPI::ITTContractCollection* contracts);
	virtual void CMeapiSink_OnNewContractA(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct, MEAPI::ITTContractInfo* pContractInfo) {};
	virtual void CMeapiSink_OnRequestContractsTimeout(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct);

	// Declare the private methods
	void PublishGatewayName(CString gatewayName);
	void ReleaseProductEventHandlers();

public:

	// standard constructor
	CMarketExplorerDialog(CWnd* pParent = NULL);

	// Dialog Data
	//{{AFX_DATA(CMarketExplorerDialog)
	enum { IDD = IDD_MARKETEXPLORER_DIALOG };
	//}}AFX_DATA

protected:

	HICON m_hIcon;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarketExplorerDialog)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMonitorGatewaysDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedRequestGateways();
	afx_msg void OnLbnSelchangeLbGateways();
	afx_msg void OnLbnSelchangeLbProducttypes();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	// Declare the sink class object.
	CMeapiSink* m_pMeapi;

	// Object to store our downloaded product collection
	MEAPI::ITTProductCollection* m_ProductCollection;
	MEAPI::ITTGateway* m_SelectedGateway;

	// Variables mapped to controls
	CEdit m_StatusBar;
	CListBox m_lbGateways;
	CListBox m_lbProductTypes;
	CListBox m_lbInfo;
	CTreeCtrl m_tvProducts;

	// private member variables
	CMap<CString, LPCSTR, MEAPI::ITTGateway*, MEAPI::ITTGateway*> m_GatewayCollection;
	int m_ExchangeCounter;
	int m_ProductCounter;
	BOOL m_IsGatewayAvailable;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* MARKETEXPLORERDIALOG_H */