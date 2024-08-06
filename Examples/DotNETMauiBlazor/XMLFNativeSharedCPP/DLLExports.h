#pragma once
#pragma message("Start Building DLLExports.h")
#include "XMLFoundationApp.h"
using namespace XMLFoundationNative;


#ifdef _WINDOWS
#define DllExport _declspec(dllexport)
#else
#define DllExport
#endif




extern "C"
{
        // This block of exports would be constant to all applications
        DllExport XMLFoundationApp* CreateAppHandle();
        DllExport void DeleteAppHandle(XMLFoundationApp* pApp);
        DllExport int SetAppValue(XMLFoundationApp* pApp, const char* pzSection, const char* pzEntry, const char* pzValue);
        DllExport int GetAppValue(XMLFoundationApp* pApp, const char* pzSection, const char* pzEntry, char* pInOutBuffer, __int64* nBufSize);
        DllExport XMLResultset* GetAllAppValues(XMLFoundationApp* pApp, char* pResultsBuf, __int64* pBufLength, bool bAsXML = 0, const char* pzSectionOnly = 0);
        DllExport int SetAllAppValues(XMLFoundationApp* pApp, char* pResultsBuf, __int64* pDataLen, bool bAsXML = 0);
        DllExport int FromXML(XMLFoundationApp* pApp, const char* pzXML);
        DllExport XMLResultset* ToXML(XMLFoundationApp* pApp, char* pResultsBuf, __int64* pBufLength, int nTabs = 0, const char* TagOverride = 0, unsigned int nSerializeFlags = FULL_SERIALIZE, const char* pzSubsetOfObjectByTagName = 0, bool bAppend = 0);
        
        DllExport void DeleteResultsetHandle(XMLResultset* handleRS);


        // this block of export functions would also be constant to all applications at a global level using CacheManager which is why 
        // none of the functions in this block require the [XMLFoundationApp* pApp] argument
        DllExport int GetMemberValue(char* pInOutBuffer, __int64* nBufSize, const char* pzOIDClassName, const char *pzTagName, int nSource);
        DllExport int SetMemberValue(const char* pzOIDClassName, const char* pzTagName, const char *pzValue);
        
        DllExport bool IsXMLObject(const char* pzOIDClassName);
        DllExport XMLObject* GetXMLObjectRef(const char* pzOIDClassName);
        DllExport void FreeXMLObjectRef(XMLObject *pObj);



        // this block of exports is specific to the example, these would be removed or changed per application
        DllExport int InsertOrUpdateCustomer(XMLFoundationApp* pApp, unsigned __int64 nID, const char* pzName, const char* pzColor, const char* strListItems);
        DllExport int InsertOrUpdateAddress(XMLFoundationApp* pApp, unsigned __int64 nCustID, const char* strLine1, const char* strLine2, const char* strCity, const char* strState, const char* strZip, int nAddrType);
        DllExport int GetLastAppError(XMLFoundationApp* pApp, char* pInOutBuffer, __int64* nBufSize);
        DllExport int GetAppResultString(XMLFoundationApp* pApp, char* pzInOutString, __int64* nBufSize, int nType);



        // ServerCore.cpp is a C++ Application which is built upon the highly portable XMLFoundation collective library.
        // ServerCore needs the XMLFoundation, however the XMLFoundation does not need ServerCore and does not need to include it into all applications by default.  
        // It has very little to do with XML specifically unless it is transporting XML by one of the services as raw data.
        // ServerCore manages various services and protocols, such as HTTP service, or HTTP proxy service, or "SwitchBoard Service"
        // this is architectually a clear line of delineation into a single C++ source file, ServerCore.cpp.  Here begins the .NET Interop to ServerCore.cpp.
#ifndef _NO_XMLF_SERVICES
        DllExport void SetCoreValue(const char* pzSection, const char* pzEntry, const char* pzValue);
        DllExport int GetCoreValue(char* pInOutBuffer, __int64* nBufSize, const char* pzSection, const char* pzEntry);
        DllExport XMLResultset* GetAllCoreValues(char* pResultsBuf, __int64* pBufLength, bool bAsXML = 0, const char* pzSectionOnly = 0);
        DllExport int GetLogFileData(char* pResultsBuf, __int64* pBufLength);
        DllExport int SetAllCoreValues(const char* pzConfig, bool bAsXML = 0);


        DllExport int CoreCommand(char* pInOutBuffer, __int64* nBufSize, const char* pzAuthKey, const char* pzCmd, const char* Arg1="", const char* Arg2 = "", const char* Arg3 = "");

        // supply the buffer for the error description and the size of that buffer
        // returns the length of the error description in [pInOutBuffer] - if return value > nBufSize the error message was truncated
        DllExport int GetLastServerError(char* pInOutBuffer, __int64* nBufSize);

#endif

}

#pragma message("Done Building DLLExports.h !!!")
