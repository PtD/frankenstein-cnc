#include "stdafx.h"
#include "resource.h"
#include "Mach3MQTT.h"
#include "XMLProfile.h"
#include "dbg.h"

#pragma warning(disable:4005) // kills redef errors from resources.

//----------------------------------------------------------------------------

// This is a Generalized Device file
// The actual device is implemented in the file with these entries.
// Conventionally this is ProtoImpl.cpp

// Here are the routines in the implementors file

// initial access to Mach profile
// when enumerating available plugins

extern CString profileInit(CString, CXMLProfile*);

// called during Mach initialisation
// you can influence subsequent init by actions here
// **** Not used in typical device plugin

extern void	initControl();

// called when mach fully set up

extern void	postInitControl();

// Called to configure the device

extern void	readConfig(CXMLProfile*);

// 10Hz update loop

extern void	update();

// called at 40Hz

extern void	highSpeedUpdate();

//Destruction routine for cleanup

extern void cleanUp();

//----------------------------------------------------------------------------

// System Variables

CXMLProfile		*DevProf;
OneShort		DoButton;     // void DoButton(code);
DoubleShort		GetDRO;       // Double GetDRO(code);
VoidShortDouble	SetDRO;       // void   SetDRO(short code, double value);
BoolShort		GetLED;       // bool   GetLED(short code);
VoidShortBool	SetLED;       // void	SetLED();
CSTRret			GetProName;   // CString GetProName();
VoidLPCSTR		Code;         // void Code("G0X10Y10");
CXMLProfile *AppProf;
CString ProfileName;

_TrajectoryControl *trajectoryControl;	// used for most planner funcitons and program control
_CMach4View *view;			// used for most framework and configuration calls.
TrajBuffer *engine;				//Ring0 memory for printer port control and other device syncronisation
setup *vsetup;					//Trajectory planners setup block. Always in effect

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Mach3 defines of External Variables. These var's are directly usable in this dll as they are all
// instantiated prior to this dll being opened. Most work in Mach can be done by attaching to these variables
// You cannot, however, call functions in these classes, they are not bound to the plugin. You may only call
// the functions instantiated in thsi dll. All variables and structures , however, may be used.
// Note the way this example uses various objects.


// Following routines are for this DLL to function properly
// They are used to create and lock the instinces of this dll. The Dll can be loaded mumiple times, so locks are maintained.
// Most users can ignore these functions.. Look further down for the start of the Mach callback functions.

BEGIN_MESSAGE_MAP(CMach3MQTTApp, CWinApp)
END_MESSAGE_MAP()

static CString pluginName = "MQTT DRO Comm";
static int counter = 0;

//----------------------------------------------------------------------------

CMach3MQTTApp::CMach3MQTTApp() {
	// Place all significant initialization in InitInstance
}


CMach3MQTTApp::~CMach3MQTTApp() {

}


CMach3MQTTApp theApp;

const GUID CDECL BASED_CODE _tlid =
    { 0x2c07387a, 0xe19c, 0x4b2b,{ 0x88, 0x35, 0x40, 0xae, 0x7e, 0xde, 0x1f, 0x73 } };

const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

//----------------------------------------------------------------------------

// initialization
BOOL CMach3MQTTApp::InitInstance() {
	CWinApp::InitInstance();

	if (!AfxSocketInit()) {

		AfxMessageBox("Sockets Init Failed");

		return(FALSE);
	}

	// Register all OLE server (factories) as running.  This enables the
	// OLE libraries to create objects from other applications.

	COleObjectFactory::RegisterAll();

	return(TRUE);
}

// DllGetClassObject - Returns class factory
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return(AfxDllGetClassObject(rclsid, riid, ppv));
}

// DllCanUnloadNow - Allows COM to unload DLL
STDAPI DllCanUnloadNow(void) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return(AfxDllCanUnloadNow());
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll())
		return SELFREG_E_CLASS;

	return(S_OK);
}

// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll(FALSE))
		return SELFREG_E_CLASS;

	return(S_OK);
}

// Callback Functions are instantiated here for general use.
// Get the Callbacks to the Mach3 System

// void DoButton(short code);
extern "C" __declspec(dllexport) void SetDoButton(OneShort pFunc) {
	DoButton = pFunc;
}

// void SetDRO(short code, double value);
extern "C" __declspec(dllexport) void SetSetDRO(VoidShortDouble pFunc) {
	SetDRO = pFunc;
}

// double GetDRO(short code);
extern "C" __declspec(dllexport) void SetGetDRO(DoubleShort pFunc) {
	GetDRO = pFunc;
}

// bool GetLED(short code);
extern "C" __declspec(dllexport) void SetGetLED(BoolShort pFunc)  {
	GetLED = pFunc;
}

// bool GetLED( short code );
extern "C" __declspec(dllexport) void SetSetLED(VoidShortBool pFunc) {
	SetLED = pFunc;
}

//----------------------------------------------------------------------------

// bool GetLED(short code);

extern "C" __declspec(dllexport) void SetCode(VoidLPCSTR pFunc) {
	Code = pFunc;
}

//----------------------------------------------------------------------------
extern "C" __declspec(dllexport) char* SetProName(CString name) {
    ProfileName = name;
    DevProf = new CXMLProfile(); // start up the Profile class for XML usage. Same as Mach3's.
    profileInit(name, DevProf);
    delete DevProf;

    return ((char*) (LPCTSTR) (pluginName));
}

//----------------------------------------------------------------------------

// Calls into the DLL
//*****************************************************
//
// Mach3 Calls to the Plugin Follow
//
// Tells Mach3 whether to start the printer driver in Ring 0 or not. Return false to run a different device.
// Also gives access to parameter blocks and variables from the 4 main classes of Mach3.

//This is a timer loop set for 25ms to keep latency low. We do need to disable it though
//if the callback loop is not running.

//----------------------------------------------------------------------------
extern "C" __declspec(dllexport) void StopPlug(void) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	cleanUp();
}

//----------------------------------------------------------------------------
extern "C" __declspec(dllexport) void DoDwell(double time) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
}


//----------------------------------------------------------------------------
VOID CALLBACK OnTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent,DWORD dwTime) {
	highSpeedUpdate();
}

//----------------------------------------------------------------------------
extern "C" __declspec(dllexport) void PostInitControl() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	//this routine is called after Mach3 has initialised. Use it as Init, BUT no usage of the XML files at all here. Only in  Init.
	//this routine is handy for changing variables that Mach3 has loaded at startup. Usually, Mach3 will permanently save any var's you change here..

	postInitControl();

    SetTimer(NULL, 1, 25, OnTimer);
}

//----------------------------------------------------------------------------

//This routine is for setting various pointers, and for shutting off the Movement Engines for an external device to use Mach3, is necessary. It
//can interrupt things from occuring..

extern "C" __declspec(dllexport) bool InitControl(void *oEngine , void *oSetup , void *oMainPlanner, void *oView) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

	engine = (TrajBuffer*)(oEngine);
	// setup = (setup*)(oSetup);
	view = (_CMach4View*)(oView);
	trajectoryControl = (_TrajectoryControl*)(oMainPlanner);

	initControl();

	return(TRUE);
}

//----------------------------------------------------------------------------

extern "C" __declspec(dllexport) void Config() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}

//----------------------------------------------------------------------------

// Called when Reset is pressed.

extern "C" __declspec(dllexport) void Reset() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//Called when reset is pressed, at end of actual reset commend in Mach3.
	//Check the Engine.Estop variable to see if we have reset or not..
}

//----------------------------------------------------------------------------

// Called when Jog is commanded. 0 for speed is Jog% jog, otherwise it is a new jog%
extern "C" __declspec(dllexport) void JogOn(short axis, short dir, double speed){
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
}

//----------------------------------------------------------------------------

//Called when jog should stop on a particular axis
extern "C" __declspec(dllexport) void JogOff(short axis) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}

//----------------------------------------------------------------------------
extern "C" __declspec(dllexport) void Purge(short flags) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}

//----------------------------------------------------------------------------

extern "C" __declspec(dllexport) void Probe() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}

//----------------------------------------------------------------------------

extern "C" __declspec(dllexport) void Home(short axis) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}

//----------------------------------------------------------------------------

// This is your main update loop. Approx 10hz or so..
// Since the Timer refresh is too low for ModIO at only 10hz, and we want smooth control at 40hz or so,
// we will use this loop only to shut down the main timing loop if the user disables this plugin.
// If the plugin gets enabled, the timer procedure is kicked into life at 25ms update, or about 40hz.

// UPDATE LOOP 10 Times a Second.
extern "C" __declspec(dllexport) void Update() {
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
}

CString infoMessage;
// ---------------------------------------------------------------------------
extern "C" __declspec(dllexport) void Notify(int ID)          //void DoButton( short code );
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
}
//----------------------------------------------------------------------------

