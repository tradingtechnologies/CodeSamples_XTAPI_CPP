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
#ifndef FILLUPDATEDIALOG_H
#define FILLUPDATEDIALOG_H

#include "XTAPI\OrderSetNotifySink.h"

/* OrderUpdate dialog window.  Handles the methods inherited from the 
 * orderset object.
 */
class CFillUpdateDialog : 
	public CDialog,
	public COrderSetNotifySink::CNotify  // Inherit the OrderSet Notification sink class.
{
private:

	// Declare the callback functions.
	virtual void COrderSetNotifySink_FillData(XTAPI::ITTFillObj* pFillObj);
	virtual void COrderSetNotifySink_FillBlockStart();
	virtual void COrderSetNotifySink_FillBlockEnd();

public:

	// standard constructor
	CFillUpdateDialog(CWnd* pParent = NULL);

	// Dialog Data
	//{{AFX_DATA(COrderUpdateDialog)
	enum { IDD = IDD_FILLUPDATE_DIALOG };
	//}}AFX_DATA

protected:

	HICON m_hIcon;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderUpdateDialog)
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CFillUpdateDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	// Declare the sink class object.
	COrderSetNotifySink* m_pOrderSet;

	// Variables mapped to controls
	CString m_StatusBar;
	CString m_FillOutput;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* FILLUPDATEDIALOG_H */
