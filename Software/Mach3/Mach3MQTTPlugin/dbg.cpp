#include "stdafx.h"
#include "dbg.h"

// DbgMsg system

BOOL __dbgMode = true;
BOOL __savDbgMode[200];

WORD __dbgLevel = 0;

//----------------------------------------------------------------------------

// Called by the macro ApiDbgMsg to print GetLastError
// info using FormatMessage

void _stdcall ApiDebugMessage(char *msg)

{
	LPTSTR buffer;

	// OutputDebugString(msg);

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				  FORMAT_MESSAGE_FROM_SYSTEM | 
				  FORMAT_MESSAGE_IGNORE_INSERTS,
				  NULL, GetLastError(),
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				  (LPTSTR) &buffer, 0, NULL);

	OutputDebugString(buffer);

	LocalFree(buffer);
}

//----------------------------------------------------------------------------

// Called by the macro DbgMsg
// DbgMsg is compiled out in a release build

void _stdcall DebugMessage(char *msg, ...)

{
	va_list args;
	char buffer[1024];

	if (__dbgMode != false) {

		va_start(args, msg);

		vsprintf(buffer,msg,args);

		OutputDebugString(buffer);
	}
}

//----------------------------------------------------------------------------

// Called by the macro RelMsg
// RelMsg always compiles into the DLL

void _stdcall ReleaseMessage(char *msg, ...)

{
	va_list args;
	char buffer[1024];

	va_start(args, msg);

	vsprintf(buffer,msg,args);

	OutputDebugString(buffer);
}

//----------------------------------------------------------------------------

// Called by the macro ErrMsg
// ErrMsg is compiled out in a release build

void _stdcall ErrorMessage(char *msg, ...)

{
	va_list args;
	char buffer[1024];
	char errmsg[1024];

	strcpy(errmsg,"ERROR: ");

	va_start(args, msg);

	vsprintf(buffer,msg,args);

	strcat(errmsg,buffer);

	OutputDebugString(errmsg);
}

//----------------------------------------------------------------------------

// Sets the current DbgMsg mode from VBA

void _stdcall DbgMode(bool mode)

{
	__dbgMode = mode;
}

//----------------------------------------------------------------------------

// Save the current debug mode

void _stdcall PushDebugMode()

{
	__savDbgMode[__dbgLevel] = __dbgMode;

	__dbgLevel++;
}

//----------------------------------------------------------------------------

// Restore the previous debug mode

void _stdcall PopDebugMode()

{
	__dbgLevel--;

	__dbgMode = __savDbgMode[__dbgLevel];
}

//----------------------------------------------------------------------------

// Dump a COM error message to the RelMsg console

void com_error_msg(_com_error &e)

{
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());

	// Print Com errors.

	("Error\n");

	RelMsg(("\tCode = %08lx", e.Error()));

	RelMsg(("\tCode meaning = %s", e.ErrorMessage()));

	RelMsg(("\tSource = %s", (LPCSTR) bstrSource));

	RelMsg(("\tDescription = %s", (LPCSTR) bstrDescription));

}

//----------------------------------------------------------------------------
