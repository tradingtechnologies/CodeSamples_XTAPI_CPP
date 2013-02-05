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

#include "stdafx.h"
#include "ServerMode.h"
#include "ServerModeDialog.h"

/* Marks the beginning of the object map that provides support for the 
 * registration, initialization, and creation of instances of ATL COM classes.
 */
BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

/* Empty constructor
 */
CServerModeApp::CServerModeApp() { }

/* Marks the beginning of the object map that provides support for the 
 * registration, initialization, and creation of instances of ATL COM classes.
 */
CServerModeApp theApp;

/* Initialize the XTAPI COM module
 */
BOOL CServerModeApp::InitInstance()
{
	// Initializes the COM library on the current thread and identifies the 
	// concurrency model as single-thread apartment (STA).
	if (FAILED(CoInitialize(NULL)))
	{
		return FALSE;
	}

	// Initialize ATL
	if (FAILED(_Module.Init(ObjectMap, AfxGetInstanceHandle())))
	{
		return FALSE;
	}

	// create and display our dialog window
	CServerModeDialog dlg;
	CWinThread::m_pMainWnd = &dlg;

	INT_PTR nRet = dlg.DoModal();
	// nRet can be handled here if need be

	// The dialog has been closed - return false so the application exits
	return FALSE;
}

/* Destroy the XTAPI COM module
 */
int CServerModeApp::ExitInstance()
{
	_Module.RevokeClassObjects();
	_Module.Term();
	CoUninitialize();

	return CWinApp::ExitInstance();
}