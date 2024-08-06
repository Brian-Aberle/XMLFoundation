// --------------------------------------------------------------------------
//						United Business Technologies
//			  Copyright (c) 2000 - 2024  All Rights Reserved.
//
// Source in this file is released to the public under the following license:
// --------------------------------------------------------------------------
// This toolkit may be used free of charge for any purpose including corporate
// and academic use.  For profit, and Non-Profit uses are permitted.
//
// This source code and any work derived from this source code must retain 
// this copyright at the top of each source file.
// --------------------------------------------------------------------------

#ifndef _EXCEPT_HANDLER_H_AND_IMPL
#define _EXCEPT_HANDLER_H_AND_IMPL

#ifdef WINCE
	#define _NO_EXCEPT
#endif
#ifdef BORLANDC
	#define _NO_EXCEPT
#endif


#include "GException.h"

static int _DispSignal= 0;

#ifdef _WIN32
	// for standard Win32 C exception handling
#if defined (_WIN32) && !defined(__WINPHONE) && !defined(_WIN64) && !defined(WINCE)  //32 bit Windows but not WINCE or __WINPHONE
//	#ifndef _NO_EXCEPT
		#include <eh.h>
//	#endif
	#include <windows.h>
#endif
#if defined (_WIN64)  // 64 bit windows - Recording the callstack is not implemented in 64 bit windows, only Exception trap.

	#include <signal.h>
	void HandleSE(int nSignal); // in GException.cpp

#endif

	#define ___TRY	if (1)							

#if defined (_WIN32) && !defined(__WINPHONE) && !defined(_WIN64) && !defined(WINCE)  //32 bit Windows 
	static void HandleSE(int) {}

	// Overloaded Win32 C runtime exception handler
	static void _xml_se_translator(unsigned int e, _EXCEPTION_POINTERS* p)
	{

		switch (e)
		{
			case EXCEPTION_ACCESS_VIOLATION :
				throw GException(0,"Access violation");
			case EXCEPTION_DATATYPE_MISALIGNMENT :
				throw GException(7001,"Data type misalignment");
			case EXCEPTION_BREAKPOINT :
				throw GException(7002,"Breakpoint");
			case EXCEPTION_SINGLE_STEP :
				throw GException(7003,"Single step");
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED :
				throw GException(7004,"Array bounds exceeded");
			case EXCEPTION_FLT_DENORMAL_OPERAND :
				throw GException(7005,"Float denormal orerand");
			case EXCEPTION_FLT_DIVIDE_BY_ZERO :
				throw GException(7006,"Float divide by zero");
			case EXCEPTION_FLT_INEXACT_RESULT :
				throw GException(7007,"Float inexact result");
			case EXCEPTION_FLT_INVALID_OPERATION :
				throw GException(7008,"Float invalid operation");
			case EXCEPTION_FLT_OVERFLOW :
				throw GException(7009,"Float overflow.");
			case EXCEPTION_FLT_STACK_CHECK :
				throw GException(7010,"Float stack check.");
			case EXCEPTION_FLT_UNDERFLOW :
				throw GException(7011,"Float underflow");
			case EXCEPTION_INT_DIVIDE_BY_ZERO :
				throw GException(7012,"Integer divide by zero");
			case EXCEPTION_INT_OVERFLOW :
				throw GException(7013,"Integer overflow");
			case EXCEPTION_PRIV_INSTRUCTION :
				throw GException(7014,"Priv instruction");
			case EXCEPTION_IN_PAGE_ERROR :
				throw GException(7015,"Page error");
			case EXCEPTION_ILLEGAL_INSTRUCTION :
				throw GException(7016,"Illegal instruction");
			case EXCEPTION_NONCONTINUABLE_EXCEPTION :
				throw GException(7017,"Non-continuable exception");
			case EXCEPTION_STACK_OVERFLOW :
				throw GException(7018,"Stack overflow");
			case EXCEPTION_INVALID_DISPOSITION :
				throw GException(7019,"Invalid disposition");
			case EXCEPTION_GUARD_PAGE :
				throw GException(7020,"Page guard fault");
			case EXCEPTION_INVALID_HANDLE :
				throw GException(7021,"Invalid handle");
			default :
				GString strExceptionText;
				strExceptionText.Format("OS detected exception[%d]",(int)e);
				throw GException(7022,(const char *)strExceptionText);
		}
	}
#endif
#else  //(where _WIN32 is not defined)  
	extern "C" 
	{

	#include <signal.h>
	#include <setjmp.h>

	static jmp_buf _x__env;

	#define ILLEGAL_INSTRUCTION				0x10
	#define FLOATING_POINT_EXCEPTION		0x11
	#define EXCEPTION_DATATYPE_MISALIGNMENT	0x12
	#define EXCEPTION_ACCESS_VIOLATION		0x13
	#define INTERRUPT_BREAK					0x14
	#define SIG_HUP							0x15
	#define SIG_PIPE						0x16

	// This may generate an 'Unused Function' warning
	static void HandleSE(int e)
	{
		switch (e)
		{
		case ILLEGAL_INSTRUCTION :
			throw GException(7016,"Signal: Illegal instruction");
		case FLOATING_POINT_EXCEPTION :
			throw GException(7008, "Signal: Floating Point Exception");
		case EXCEPTION_DATATYPE_MISALIGNMENT :
			throw GException(7001, "Signal: Datatype Misalignment");
		case EXCEPTION_ACCESS_VIOLATION :
			throw GException(7000, "Signal: Access Violation");
		case INTERRUPT_BREAK:
			throw GException(7023, "Signal: Interrupt Break");
		case SIG_HUP:
			throw GException(7024, "Signal: HUP");
		case SIG_PIPE:
			throw GException(7025, "Signal: Broken Pipe");
		default :
			throw GException(7022, "Signal: Unknown");
		}
	}
		static void DispSIGHUP(int i)
		{
			longjmp(_x__env, SIG_HUP);
		}
		static void DispSIGPIPE(int i)
		{
			longjmp(_x__env, SIG_PIPE);
		}
		static void DispSIGINT(int i)
		{
			longjmp(_x__env, ILLEGAL_INSTRUCTION);
		}
		static void DispSIGILL(int i)
		{
			longjmp(_x__env, ILLEGAL_INSTRUCTION);
		}
		static void DispSIGFPE(int i)
		{
			longjmp(_x__env, FLOATING_POINT_EXCEPTION);
		}
		static void DispSIGBUS(int i)
		{
			longjmp(_x__env, EXCEPTION_DATATYPE_MISALIGNMENT);
		}
		static void DispSIGSEGV(int i)
		{
			longjmp(_x__env, EXCEPTION_ACCESS_VIOLATION);
		}
	}
	#define ___TRY 	if (_DispSignal == 0)		
#endif


	

class ExceptionHandlerScope
{
#if defined (_WIN64)  // 64 bit windows
	typedef void (*_se_signal_handler64)(int);
	_se_signal_handler64 fnPrev;
#endif

#if defined (_WIN32) && !defined(__WINPHONE) && !defined(_WIN64)  // 32 bit Windows - Begin
//#ifndef _NO_EXCEPT
	_se_translator_function	fnPrev;
//#endif
#endif  // 32 bit Windows - End

#ifndef _WIN32 // all non windows
	typedef void (*fnPtr)(int);

	fnPtr nILL;
	fnPtr nFPE;
	fnPtr nBUS;
	fnPtr nSEGV;
	fnPtr nINT;
	fnPtr nHUP;
	fnPtr nPIPE;
#endif
public:
	ExceptionHandlerScope()
	{
#if defined (_WIN32) && !defined(__WINPHONE) && !defined(_WIN64)  // 32 bit Windows
// NOTE: In Microsoft Visual Studio compiler, right click the project select Properties, then C/C++, All Settings
//       Change "Enable C++ Exceptions" from "yes(/EHsc)" to "Yes with SEH Exceptions (/EHa)"
		fnPrev = _set_se_translator(_xml_se_translator);
#endif
#if defined (_WIN64)   // 64 bit windows
		fnPrev = signal(SIGSEGV, HandleSE);
#endif

#ifndef _WIN32
		nILL = signal(SIGILL,  DispSIGILL);	
		nFPE = signal(SIGFPE,  DispSIGFPE); 
		nBUS = signal(SIGBUS,  DispSIGBUS); 
		nSEGV =signal(SIGSEGV, DispSIGSEGV);	
		nINT = signal(SIGINT,  DispSIGINT); 
		nHUP = signal(SIGHUP,  DispSIGHUP); 
		nPIPE =signal(SIGPIPE,  DispSIGPIPE); 

		_DispSignal = setjmp(_x__env);
#endif
	}
	~ExceptionHandlerScope()
	{
#if defined (_WIN32) && !defined(__WINPHONE) && !defined(_WIN64)  // 32 bit Windows
		_set_se_translator(fnPrev);
#endif
#if defined (_WIN64)   // 64 bit windows
		signal(SIGSEGV, fnPrev);
#endif
#ifndef _WIN32
		signal(SIGILL,  nILL);	
		signal(SIGFPE,  nFPE); 
		signal(SIGBUS,  nBUS); 
		signal(SIGSEGV, nSEGV);	

		signal(SIGINT, nINT);
		signal(SIGHUP, nHUP);
		signal(SIGPIPE, nPIPE);

#endif
	}
};


// _CATCH_ALL is defined in yvals.h by Microsoft for Apache-2.0 WITH LLVM-exception
// therefore XMLFoundation's _CATCH_ALL was remaned to ___CATCH_ALL in 2024
// and to be conformant, _TRY_ALL was renamed to ___TRY_ALL


#ifdef _NO_EXCEPT
 #define ___TRY_ALL try 
 #define ___CATCH_ALL(ex) catch (GException &ex)
#else
 #define ___TRY_ALL try { ___TRY
 #define ___CATCH_ALL(ex)	else { HandleSE(_DispSignal); } }  catch (GException &ex)
#endif

#endif




