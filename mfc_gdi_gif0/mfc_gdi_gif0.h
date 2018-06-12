
// mfc_gdi_gif0.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cmfc_gdi_gif0App:
// See mfc_gdi_gif0.cpp for the implementation of this class
//

class Cmfc_gdi_gif0App : public CWinApp
{
public:
	Cmfc_gdi_gif0App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cmfc_gdi_gif0App theApp;