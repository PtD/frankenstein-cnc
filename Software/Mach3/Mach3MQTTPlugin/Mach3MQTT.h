#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <_TrajectoryControl.h>
#include <Engine.h>
#include <_Mach4View.h>
#include <rs274ngc.h>


// Here are the types of the pointers to functions that can be called

typedef void (CALLBACK *NoParms) ();
typedef void (_cdecl *OneShort) ( short );
typedef double (_cdecl *DoubleShort) ( short );
typedef void (_cdecl *VoidShortDouble) ( short , double );
typedef bool (_cdecl *BoolShort) ( short );
typedef void (_cdecl *CSTRret) ( CString );
typedef void (_cdecl *VoidLPCSTR) (LPCTSTR );
typedef void (_cdecl *VoidShortBool) ( short, bool );
typedef void (_cdecl *DoubleShortDouble) ( short , double );

extern OneShort           DoButton;     // void DoButton(code)
extern DoubleShort        GetDRO;       // Double GetDRO(code)
extern DoubleShortDouble  SetDRO;       // void SetDRO(short code, double value);
extern BoolShort          GetLED;       // bool GetLED(short code);
extern VoidShortBool      SetLED;
extern CSTRret            GetProName;   // CString GetProName()
extern VoidLPCSTR         Code;         // void Code("G0X10Y10");

extern _TrajectoryControl *trajectoryControl;	// used for most planner funcitons and program control
extern _CMach4View *view;			// used for most framework and configuration calls.
extern TrajBuffer *engine;				//Ring0 memory for printer port control and other device syncronisation
extern setup *vsetup;					//Trajectory planners setup block. Always in effect

void ShowMachMessage(CString);

//----------------------------------------------------------------------------

// CProtoApp
// See Proto.cpp for the implementation of this class

class CMach3MQTTApp : public CWinApp
{
public:
	CMach3MQTTApp();
	~CMach3MQTTApp();

// Overrides

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

//----------------------------------------------------------------------------
