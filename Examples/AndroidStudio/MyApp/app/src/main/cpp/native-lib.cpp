
// Generally you do not change source code in XMLFoundation.cpp because the
// XMLFoundation class library contains nothing application specific
#define ___KOTLIN_COMPILER 1   // define this prior to including XMLFoundation.cpp
#include "../src/XMLFoundation.cpp"

// this is code specific to your application - this is where you develop your own C++ code
#include "XMLFoundationApp.cpp"
#include "ExampleObjectModeling.cpp"

/*
 * All the source code above this comment was copied verbatim from the Windows .NET-MAIU-Blazor Example App
 * Within this comment block is the interface defined by the .NET Interop DLL used by C#
 * It is included here because the comments are relevant and this is what will be re-implemented
 * designed for Java rather than designed for C# and .NET

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
*/

// You could run the .NET-MAUI-Blazor App and see all the functionality implemented within
// that example application to understand what is already implemented in this example application.
// The only thing remaining for this application is to expose the interface designed for Java
// then to improve the user interface in the Java layer of this app to use the interface
// below that is currently still under development on August 1, 2024


#include <jni.h>
#include <string>



extern "C" JNIEXPORT jlong JNICALL
Java_com_example_myapp_MainActivity_CreateAppHandle(JNIEnv* env, jclass)
{
    XMLFoundationNative::XMLFoundationApp* pApp = new XMLFoundationNative::XMLFoundationApp();
    return reinterpret_cast<jlong>(pApp); 
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapp_MainActivity_DeleteAppHandle(JNIEnv* env, jclass, jlong handle)
{
    XMLFoundationNative::XMLFoundationApp* pApp = reinterpret_cast<XMLFoundationNative::XMLFoundationApp*>(handle);
    if (pApp)
	   delete pApp;
}


extern "C" JNIEXPORT int JNICALL
Java_com_example_myapp_MainActivity_SetAppValue(JNIEnv* env, jclass, jlong handle, jstring sSection, jstring sEntry, jstring sValue)
{
    XMLFoundationNative::XMLFoundationApp*pApp = reinterpret_cast<XMLFoundationNative::XMLFoundationApp*>(handle);
    if (pApp)
    {
	jboolean isCopy;
	const char*pzSection= env->GetStringUTFChars(sSection, &isCopy);
	const char*pzEntry= env->GetStringUTFChars(sEntry, &isCopy);
	const char*pzValue= env->GetStringUTFChars(sValue, &isCopy);

        return pApp->SetAppValue(pzSection, pzEntry, pzValue);
    }	
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myapp_MainActivity_GetAppValue(JNIEnv* env, jclass, jlong handle, jstring sSection, jstring sEntry)
{
    XMLFoundationNative::XMLFoundationApp* pApp = reinterpret_cast<XMLFoundationNative::XMLFoundationApp*>(handle);
    if (pApp)
    {
	jboolean isCopy;
	const char*pzSection= env->GetStringUTFChars(sSection, &isCopy);
	const char*pzEntry= env->GetStringUTFChars(sEntry, &isCopy);

	const char *pResult =  pApp->GetAppValue(pzSection, pzEntry);
	if (pResult)
        {
	    return env->NewStringUTF(pResult);
        }
	else
        {
	    return env->NewStringUTF("");
        }
    }	
}



extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myapp_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {


// using a custom define flag you can set options in your C++ code from the CMake files where cppFlags += "-D _USER_DEFINED_CUSTOM_BUILD_FLAG"
#ifdef _USER_DEFINED_CUSTOM_BUILD_FLAG
    std::string hello = "C++ says Hello";
#else
    std::string hello = "Hello from C++";
#endif

    return env->NewStringUTF(hello.c_str());
}

