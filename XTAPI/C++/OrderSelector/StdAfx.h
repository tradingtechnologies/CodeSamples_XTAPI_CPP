// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#if !defined(AFX_STDAFX_H__3E6B6E98_C0E2_4543_81E7_05C11B64A8D3__INCLUDED_)
#define AFX_STDAFX_H__3E6B6E98_C0E2_4543_81E7_05C11B64A8D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define _ATL_APARTMENT_THREADED
#include <atlbase.h>

// declare a CComModule class for the atlcom.h header file.
class XTAPI_Module : public CComModule {};
extern XTAPI_Module _Module;

#include <atlcom.h>

// Load the XTAPI and rename the namespace
#import "C:\tt\x_trader\xtapi\bin\xtapi_7.dll" rename_namespace("XTAPI") named_guids

#endif /* AFX_STDAFX_H__3E6B6E98_C0E2_4543_81E7_05C11B64A8D3__INCLUDED_ */
