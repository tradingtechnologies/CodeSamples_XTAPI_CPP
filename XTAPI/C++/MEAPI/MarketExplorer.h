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
#ifndef MARKETEXPLORER_H
#define MARKETEXPLORER_H

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

/* Represents the MEAPI COM module
 */
class CMarketExplorerApp : public CWinApp
{
public:
	CMarketExplorerApp();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarketExplorerApp)
	public:
	virtual BOOL InitInstance(); //MEAPI - Initialize the COM module.
	virtual int ExitInstance();  //MEAPI - Destroy the COM module.
	//}}AFX_VIRTUAL

private:
	// MEAPI ATL COM module
	MEAPI_Module _Module;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* MARKETEXPLORER_H */