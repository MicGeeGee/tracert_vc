
// tracert_vc.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Ctracert_vcApp:
// See tracert_vc.cpp for the implementation of this class
//

class Ctracert_vcApp : public CWinApp
{
public:
	Ctracert_vcApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Ctracert_vcApp theApp;