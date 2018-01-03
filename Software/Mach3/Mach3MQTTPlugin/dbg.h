// DbgMsg system
#pragma once 

#include "stdio.h"
#include "stdarg.h"
#include "comdef.h"

void _stdcall ApiDebugMessage(char *msg);
void _stdcall DebugMessage(char *msg, ...);
void _stdcall ReleaseMessage(char *msg, ...);
void _stdcall ErrorMessage(char *msg, ...);
void _stdcall DbgMode(bool mode);
void _stdcall PushDebugMode(void);
void _stdcall PopDebugMode(void);

void com_error_msg(_com_error &e);

// The printf_exp MUST be enclosed in quotes as:

// DbgMsg(("the debug message"));

extern BOOL __dbgMode;
extern BOOL __savDbgMode[200];

extern WORD __dbgLevel;

#ifdef _DEBUG
#define ErrMsg(printf_exp) (void)(ErrorMessage printf_exp)
#define RelMsg(printf_exp) (void)(ReleaseMessage printf_exp)
#define DbgMsg(printf_exp) (void)(DebugMessage printf_exp)
#define ApiDbgMsg(printf_exp) (void)(ApiDebugMessage printf_exp)
#define PushDbgMode() PushDebugMode()
#define PopDbgMode() PopDebugMode()
#define DbgOn __dbgMode = true;
#define DbgOff __dbgMode = false;

#else

#define ErrMsg(printf_exp) ((void)0)
#define RelMsg(printf_exp) (void)(ReleaseMessage printf_exp)
#define DbgMsg(printf_exp) ((void)0)
#define ApiDbgMsg(printf_exp) ((void)0)
#define PushDbgMode()
#define PopDbgMode()
#define DbgOn
#define DbgOff

#endif