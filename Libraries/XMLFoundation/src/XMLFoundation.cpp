#pragma message("Start Building XMLFoundation.cpp...... ")


//////////////////////////////////////////////////////////////
// these are the platform related preprocessor directives within the XMLFundation codebase
//
//_WIN32	// identifies all Windows platforms 
//_WIN64        // target is 64 bit Windows 
//___LINUX	// IDentifies all Linux builds [32, 64 and various linux types] -  (including Android because Android originates from a fork of Linux and shares much of its structure)
//___LINUX64    // added in addition to ___LINUX when the target is 64 bit
//___IOS	// All Apple platforms are considered 64 bit within XMLFoundation
//___ARM32	// specifically only defined for 32 bit ARMv7 
//___ARM64	// All 64bit ARM builds
//___ANDROID	// Specifies the reduced branch of the Linux build, currently must Accompany either ___ARM32 or ___ARM64
//___KOTLIN_COMPILER
// _CLANG_COMPILER_



#ifdef ___KOTLIN_COMPILER
#include "Utils/KotlinConfigure.cpp"
#endif // ___KOTLIN_COMPILER



#define _LIBRARY_IN_1_FILE

#ifdef ___ANDROID
#define _NO_SYS_BTIME_
#endif

#ifndef _INCLUDED_XMLFOUNDATION_INLINE_STATIC_LIBRARY_
#define _INCLUDED_XMLFOUNDATION_INLINE_STATIC_LIBRARY_

#if defined(_CLANG_COMPILER_) || defined( ___KOTLIN_COMPILER )
#pragma clang diagnostic push

#pragma clang diagnostic ignored "-Wdeprecated-register"	//  C++ keyword 'register' no longer exists and is ignored in C++17
#pragma clang diagnostic ignored "-Wunknown-pragmas"		// the Clang compiler encountered a pragma formatted for other compilers
#pragma clang diagnostic ignored "-Woverloaded-virtual"		// hides overloaded virtual function.  
#pragma clang diagnostic ignored "-Wformat-security"		// format string is not a string literal
#pragma clang diagnostic ignored "-Wpointer-bool-conversion"// address of array 'td->pTSD->szConfigSectionName' will always evaluate to 'true' 
#pragma clang diagnostic ignored "-Wwritable-strings"		// ISO C++11 does not allow conversion from string literal to 'char *'
#pragma clang diagnostic ignored "-Wswitch"					// unused enumeration valuie in switch
#pragma clang diagnostic ignored "-Wunused-variable"		// unused variable, in the case of 'nHTTPContentAcquired' it can be used in a plugin  ServerCore.cpp  line	8703 
#pragma clang diagnostic ignored "-Wmacro-redefined"		// 
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"// cast to 'void *' from smaller integer type 'int' - OK because the value is never used as a litertal memory address - such as src\Utils\GSocketHelpers.cpp	line 190
#pragma clang diagnostic ignored "-Wreorder-ctor"			//  field 'm_root' will be initialized after field 'm_numNodes'  
#pragma clang diagnostic ignored "-Wdelete-incomplete"		// cannot delete expression with pointer-to-'void' type 'void *' -- Its a complex delete statement   GListNodeCache.h	line 216	
#pragma clang diagnostic ignored "-Wint-to-pointer-cast"	// cast to 'GList::Node *' from smaller integer type 'int' - GList.cpp Line 41	(see how its used in GListCpp line 147)  -= this idea has been used in other places within the XMLFoundation
#pragma clang diagnostic ignored "-Wchar-subscripts"		// array subscript is of type 'char' - Base64.cpp	Line 116
#pragma clang diagnostic ignored "-Winfinite-recursion"		// all paths through this function will call itself, \GHash.h	Line 52
#pragma clang diagnostic ignored "-Wimplicitly-unsigned-literal"//integer literal is too large to be represented in a signed integer type, interpreting as unsigned
#pragma clang diagnostic ignored "-Wunused-function"		// unused function 'HandleSE' ExceptionHandler.h	Line 123 --it could be used depending on use and definition of macro XML_CATCH()
#pragma clang diagnostic ignored "-Wc++11-narrowing"  	    // source file [Libraries\XMLFoundation\src\Utils\Rijndael.cpp] requires this

#pragma clang diagnostic ignored "-Wregister"  	            // ISO C++ 17 does not allow the registger storage class specifier, the keyword is safely ignored and only used to optimize older build systems

#endif  // _CLANG_COMPILER_

#include "GlobalInclude.h"

#ifndef XMLF_NO_SERVER_CORE_AUTO_LINK  // ServerCore.cpp needs the XMLFoundation however the XMLFoundation does not need ServerCore.cpp
#pragma message("Building XMLFoundation.cpp - Is automatically linking with [ServerCore.cpp] - OR you can #define XMLF_NO_SERVER_CORE_AUTO_LINK before #include \"XMLFoundation.cpp\"")

// with [\XMLFoundation\Libraries\XMLFoundation\inc] in the build path - this ../../../ works and does not force you to add Servers/Core to your build path
#include "../../../Servers/Core/ServerCore.cpp" 
#endif




#include "Utils/GSocketHelpers.cpp"
#include "Utils/GArray.cpp"
#include "Utils/GBTree.cpp"
#include "Utils/GException.cpp"
#include "Utils/GHash.cpp"
#include "Utils/GList.cpp"
#include "Utils/GProfile.cpp"
#include "Utils/GProcess.cpp"
#include "Utils/GStack.cpp"
#include "Utils/GString.cpp"
#include "Utils/GStringList.cpp"
#include "Utils/GDirectory.cpp"
#include "Utils/GPerformanceTimer.cpp"
#include "Utils/GThread.cpp"
#include "Utils/TwoFish.cpp"
#include "Utils/SHA256xmlf.cpp" // this implementation has been in the XMLFoundation for many years and is used by TwoFish  -- todo:consolidate with SHA256.cpp
#include "Utils/SHA256.cpp"		// this implementation is new to the XMLFoundation and is used by HMAC Authentication       -- todo:consolidate with SHA256xmlf.cpp
#include "Utils/SHA512.cpp"
#include "Utils/Rijndael.cpp"
#include "Utils/GZip.cpp"
#include "Utils/Base64.cpp"
#include "Utils/PluginBuilderLowLevelStatic.cpp"
#include "Utils/GHTTPMultiPartPOST.cpp"
#include "AttributeList.cpp"
#include "CacheManager.cpp"
#include "FactoryManager.cpp"
#include "FrameworkAuditLog.cpp"
#include "MemberDescriptor.cpp"
#include "FileDataSource.cpp"
#include "ObjQueryParameter.cpp"
#include "ProcedureCall.cpp"
#include "Schema.cpp"
#include "SocketHTTPDataSource.cpp"
#include "StackFrameCheck.cpp"
#include "xmlAttribute.cpp"
#include "xmlDataSource.cpp"
#include "xmlElement.cpp"
#include "xmlElementTree.cpp"
#include "XMLException.cpp"
#include "xmlLex.cpp"
#include "xmlObject.cpp"
#include "xmlObjectFactory.cpp"
#include "XMLProcedureDescriptor.cpp"
#include "SwitchBoard.cpp"
#include "DynamicLibrary.cpp"
#include "IntegrationBase.cpp"
#include "IntegrationLanguages.cpp"

//#include "Utils/CSmtp.cpp"
//#include "Utils/GString0.cpp"			
//#include "Utils/GString32.cpp"
//#include "Utils/BZip.cpp"  // this has always been unused and had been included as an alternative to GZip if the situation ever arose.  It is portable - with many compile warnings.  The source was removed from the build in 2024 however it remains in the source distro 



#ifdef ___ANDROID
#ifndef _JAVA_SHELL_EXEC_DEFIEND
#define _JAVA_SHELL_EXEC_DEFIEND

#ifdef _NO_JAVA_DEPENDENCIES
void JavaShellExec(GString & strCommand, GString & strResult)
{
	//stub out for dynamic lib builds
}
#else

#include "../src/JavaFoundation.cpp"

extern void JavaShellExec(GString& strCommand, GString& strResult); // in JavaFoundation.cpp
#endif
#endif
#endif


#ifdef _CLANG_COMPILER_
#pragma clang diagnostic pop
#endif



#endif //_INCLUDED_XMLFOUNDATION_INLINE_STATIC_LIBRARY_

#pragma message("Done Building XMLFoundation.cpp - !!!")