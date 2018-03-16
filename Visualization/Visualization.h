
// Visualization.h : main header file for the Visualization application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CVisualizationApp:
// See Visualization.cpp for the implementation of this class
//

class CVisualizationApp : public CWinApp
{
public:
	CVisualizationApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CVisualizationApp theApp;
