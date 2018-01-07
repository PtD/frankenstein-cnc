#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
#include <_TrajectoryControl.h>
#include <Engine.h>
#include <_Mach4View.h>
#include <rs274ngc.h>



typedef void (CALLBACK *NoParms) ();
typedef void (_cdecl *OneShort) ( short );
typedef double (_cdecl *DoubleShort) ( short );
typedef void (_cdecl *DoubleShortDouble) ( short , double );
typedef bool (_cdecl *BoolShort) ( short );
typedef void (_cdecl *CSTRret) ( CString );
typedef void (_cdecl *VoidLPCSTR) (LPCTSTR );

// ===========================================================================================
//
// Here are the entry points to the implementation to be called by Mach3

// initial access to Mach profile
// when enumerating available plugins

CString	profileInit(CString name, CXMLProfile *DevProf);

// called during Mach initialization
// you can influence subsequent init by actions here
// **** Not used in typical device plugin

void initControl();

// called when mach is fully set up

void postInitControl();

// Called to configure the device

void readConfig(CXMLProfile *DevProf);

// 10Hz update loop

void update();

// called at 40Hz

void highSpeedUpdate();

// Used to cleanup heap variables before destruction of PlugIn

void cleanUp();

 // homing and probing calls from Mach3.

void onHome(short axis);

void onProbe();

// ==================================================================================================
//
// Here are the main data areas with which the implementor will do his/her work
//
// ==================================================================================================

// These pointers are set up in the code during init of DLL

// ************** Beware *****************
// You can alter almost anything and so hasten the end of the world as we know it
//
extern _TrajectoryControl *trajectoryControl;	// used for most planner funcitons and program control
extern _CMach4View *view;			// used for most framework and configuration calls.
extern TrajBuffer *engine;				// Ring0 memory for printer port control and other device syncronisation

// =====================================================================================================
//
// Here are the pointers to the functions we can call in Mach 3
//
// =====================================================================================================

extern OneShort           DoButton;     // void DoButton(code)
extern DoubleShort        GetDRO;       // Double GetDRO(code)
extern DoubleShortDouble  SetDRO;       // void SetDRO(short code, double value);
extern BoolShort          GetLED;       // bool GetLED(short code);
// extern VoidShortBool	  SetLED;
extern void(_cdecl *SetLED) (short, bool);
extern CSTRret            GetProName;   // CString GetProName()
extern VoidLPCSTR         Code;         // void Code("G0X10Y10");

// =====================================================================================================
