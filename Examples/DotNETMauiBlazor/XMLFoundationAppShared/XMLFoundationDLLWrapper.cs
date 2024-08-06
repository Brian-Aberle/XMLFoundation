using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Reflection.Metadata;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
/*
      // Import user32.dll (containing the function we need) and define
        // the method corresponding to the native function.
        [LibraryImport("user32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
        private static partial int MessageBox(IntPtr hWnd, string lpText, string lpCaption, uint uType);
 
 
 
        // Define a delegate that corresponds to the unmanaged function.
        private delegate bool EnumWC(IntPtr hwnd, IntPtr lParam);
//        BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

        [LibraryImport("user32.dll")]
        private static partial int EnumWindows(EnumWC lpEnumFunc, IntPtr lParam);

 */



namespace XMLFoundation
{
    internal static class XMLFAppWrapper
    {
  
#if __ANDROID__
        const string DllName = "XMLFNativeAndroid.so";
#elif __IOS__
        const string DllName = "__Internal";
#else
        const string DllName = "XMLFNativeWindows.dll";
#endif
        internal static string GetDllName() => DllName;


        // The following 2 functions [CreateAppHandle() and DeleteAppHandle()] should be in nearly all implementations
        [DllImport(DllName, EntryPoint = "CreateAppHandle")]
        internal static extern XMLFAppSafeHandle CreateAppHandle();

        [DllImport(DllName, EntryPoint = "DeleteAppHandle")]
        internal static extern void DeleteAppHandle(XMLFAppSafeHandle handle);


        // the next 4 XMLApp methods [SetAppValue(), GetAppValue(), GetAllAppValues() and SetAllAppValues()] manages an application configuration structure making it easdy for C# to configure the C++ layer.
        // This is fully abstract from any specific implementation being a common general support for most implementations.
        [DllImport(DllName, EntryPoint = "SetAppValue")]
        internal static extern int SetAppValue(XMLFAppSafeHandle handle, string sSection, string sEntry, string sValue);

        [DllImport(DllName, EntryPoint = "GetAppValue")]
        internal static extern int GetAppValue(XMLFAppSafeHandle handle, string sSection, string sEntry, [Out] byte[] pInOutBuffer, ref long nBufSize);

        [DllImport(DllName, EntryPoint = "GetAllAppValues")]
        internal static extern ResultsetSafeHandle GetAllAppValues(XMLFAppSafeHandle handle, [Out] byte[] pInOutBuffer, ref long nBufSize, bool bAsXML = false, string pzSectionOnly = "");

        [DllImport(DllName, EntryPoint = "SetAllAppValues")]
        internal static extern int SetAllAppValues(XMLFAppSafeHandle handle, [Out] byte[] pInOutBuffer, ref long nBufSize, bool bAsXML = false);





        // the following 3 App methods,[FromXML() ToXML() and DeleteResultsetHandle() ] are the most foundational aspect of interaction between the C++ object model instance and XML from anywhere in the distributed application layer.
        [DllImport(DllName, EntryPoint = "FromXML")]
        internal static extern int FromXML(XMLFAppSafeHandle handle, string sXML);

        [DllImport(DllName, EntryPoint = "ToXML")]
        internal static extern ResultsetSafeHandle ToXML(XMLFAppSafeHandle handle, [Out] byte[] pInOutBuffer, ref long nBufSize, int nTabs = 0, string TagOverride = "", int nSerializeFlags = 128, string pzSubsetOfObjectByTagName = "", bool bAppend = false);

        [DllImport(DllName, EntryPoint = "DeleteResultsetHandle")]
        internal static extern void DeleteResultsetHandle(ResultsetSafeHandle handleRS);


        // these functions operate on the global object cache 
        [DllImport(DllName, EntryPoint = "GetMemberValue")]
        internal static extern int GetMemberValue([Out] byte[] pInOutBuffer, ref long nBufSize, string sOIDClassName, string sTagName, int nSource);

        [DllImport(DllName, EntryPoint = "SetMemberValue")]
        internal static extern int SetMemberValue(string sOIDClassName, string sTagName, string sValue);







        // The following 2 functions[InsertOrUpdateCustomer() and InsertOrUpdateAddress() ] will be removed or changed in any implementation.
        // These functions serve only as a conceptual example of "the other way" besides XML markup passed to FromXML() that Objects are manipulated in memory.
        // Both appraoches manipulate the same object model implementation in memory.  Calling ToXML() will show the effects from calling either of these two.
        // The OID, or ObjectID, defined by example object type [Customer] is the ID.  When supplied an existing OID, the action is an update, and an unknown OID
        // causes the XML based transaction to be an insert of a new object instance.  The [Address] object uses a 2 part OID made of the related CustID plus the
        // [Address] "type" therefore only 1 of each address type can exist causing an update with the surrounding data when the xml data matches both ID+type
        // the OID key causes the exact same Insert vs Update behavior when the information comes as XML into FromXML()
        [DllImport(DllName, EntryPoint = "InsertOrUpdateCustomer")]
        internal static extern int InsertOrUpdateCustomer(XMLFAppSafeHandle handle, ulong nID, string sName, string sColor, string sListItems, int nType);

        [DllImport(DllName, EntryPoint = "InsertOrUpdateAddress")]
        internal static extern int InsertOrUpdateAddress(XMLFAppSafeHandle handle, ulong nCustID, string sLine1, string sLine2, string sCity, string sState, string sZip, int nAddrType);





        // The XMLFoundation class library is what ServerCore.cpp is built on.  
        // ServerCore needs the XMLFoundation, however the XMLFoundation does not need ServerCore and does not need to include it into all applications by default.
        // [CoreCommand] and [GetLastServerError] manage various services and protocols, such as HTTP service, or HTTP proxy service, or "SwitchBoard Service"
        // this is architectually a clear line of delineation into a single C++ source file, ServerCore.cpp.  This is the .NET Interop to ServerCore.cpp.
        // ServerCore.cpp is a C++ Application which is built upon the highly portable XMLFoundation collective library.  These 2 DLL entry points can be compiled out 
        // of the XMLFNativeWindows.dll by setting the preprocessor directive, _NO_XMLF_SERVICES within that project->propertiues->>c++->prepropcessor settings.
        [DllImport(DllName, EntryPoint = "CoreCommand")]
        internal static extern int CoreCommand([Out] byte[] pInOutBuffer, ref long nBufSize, string AuthKey, string sCmd, string pzArg1="", string pzArg2 = "", string pzArg3 = "");

        [DllImport(DllName, EntryPoint = "GetLastServerError")]
        internal static extern int GetLastServerError([Out] byte[] pInOutBuffer, ref long nBufSize);

        [DllImport(DllName, EntryPoint = "SetCoreValue")]
        internal static extern void SetCoreValue(string sSection, string sEntry, string sValue);

        [DllImport(DllName, EntryPoint = "GetCoreValue")]
        internal static extern int GetCoreValue([Out] byte[] pInOutBuffer, ref long nBufSize, string sSection, string sEntry);

        [DllImport(DllName, EntryPoint = "GetAllCoreValues")]
        internal static extern ResultsetSafeHandle GetAllCoreValues([Out] byte[] pInOutBuffer, ref long nBufSize, bool bAsXML = false, string sSectionOnly = "");

        [DllImport(DllName, EntryPoint = "SetAllCoreValues")]
        internal static extern int SetAllCoreValues(string sConfig, bool bAsXML = false);

        [DllImport(DllName, EntryPoint = "GetLogFileData")]
        internal static extern int GetLogFileData([Out] byte[] pInOutBuffer, ref long nBufSize);




        [DllImport(DllName, EntryPoint = "GetLastAppError")]
        internal static extern int GetLastAppError(XMLFAppSafeHandle handle, [Out] byte[] pInOutBuffer, ref long nBufSize);


        [DllImport(DllName, EntryPoint = "GetAppResultString")]
        internal static extern int GetAppResultString(XMLFAppSafeHandle handle, [Out] byte[] pInOutBuffer, ref long nBufSize, int nType);




    }
}


