# Microsoft Developer Studio Project File - Name="XMLFoundationLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=XMLFoundationLib - Win32 DebugStaticCRT
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XMLFoundationLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XMLFoundationLib.mak" CFG="XMLFoundationLib - Win32 DebugStaticCRT"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XMLFoundationLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "XMLFoundationLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "XMLFoundationLib - Win32 DebugStaticCRT" (based on "Win32 (x86) Static Library")
!MESSAGE "XMLFoundationLib - Win32 ReleaseStaticCRT" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XMLFoundationLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\..\Servers\CommonSource\JavaNT" /I "..\NT_Pthreads" /I "..\inc" /I "..\incWin32" /I "..\..\Servers\CommonSource" /I "..\..\Tools\Drivers\SDK" /D "__PROXY__" /D "_NO_SERVER_SUPPORT_" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "__EVAL_COPY__" /FR /Yu"PreCompiledHeader.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\inc" /I "..\inc\Win32" /D "WIN32" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../Release/XMLFoundation.lib"

!ELSEIF  "$(CFG)" == "XMLFoundationLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /Od /I "..\..\Servers\CommonSource\JavaNT" /I "..\NT_Pthreads" /I "..\inc" /I "..\incWin32" /I "..\..\Tools\Drivers\SDK" /I "..\..\Servers\CommonSource" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__PROXY__" /D "__TRACE_STACK__" /D "_NO_DOT_DOT_DOT" /FR /Yu"PrecompiledHeader.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "..\inc" /D "_WIN32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"MyJunk/" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../Debug/XMLFoundation.lib"

!ELSEIF  "$(CFG)" == "XMLFoundationLib - Win32 DebugStaticCRT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "XMLFoundationLib___Win32_DebugStaticCRT"
# PROP BASE Intermediate_Dir "XMLFoundationLib___Win32_DebugStaticCRT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug__StaticCRT"
# PROP Intermediate_Dir "Debug__StaticCRT"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MTd /W3 /GX /Zi /Od /I "..\inc" /I "..\inc\Win32" /D "_WIN32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_NO_DOT_DOT_DOT" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /GX /Zi /Od /I "..\inc" /I "..\inc\Win32" /D "_WIN32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_NO_DOT_DOT_DOT" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../../Debug/XMLFoundationNonDLL.lib"
# ADD LIB32 /nologo /out:"../../Debug/XMLFoundationStaticCRT.lib"

!ELSEIF  "$(CFG)" == "XMLFoundationLib - Win32 ReleaseStaticCRT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XMLFoundationLib___Win32_ReleaseStaticCRT"
# PROP BASE Intermediate_Dir "XMLFoundationLib___Win32_ReleaseStaticCRT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release__StaticCRT"
# PROP Intermediate_Dir "Release__StaticCRT"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\inc" /I "..\inc\Win32" /D "WIN32" /FR /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\inc" /I "..\inc\Win32" /D "WIN32" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../../Release/XMLFoundationNonDLL.lib"
# ADD LIB32 /nologo /out:"../../Release/XMLFoundationStaticCRT.lib"

!ENDIF 

# Begin Target

# Name "XMLFoundationLib - Win32 Release"
# Name "XMLFoundationLib - Win32 Debug"
# Name "XMLFoundationLib - Win32 DebugStaticCRT"
# Name "XMLFoundationLib - Win32 ReleaseStaticCRT"
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\AttributeList.cpp
# End Source File
# Begin Source File

SOURCE=..\src\CacheManager.cpp
# End Source File
# Begin Source File

SOURCE=..\src\DynamicLibrary.cpp
# End Source File
# Begin Source File

SOURCE=..\src\FactoryManager.cpp
# End Source File
# Begin Source File

SOURCE=..\src\FileDataSource.cpp
# End Source File
# Begin Source File

SOURCE=..\src\FrameworkAuditLog.cpp
# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\src\IntegrationBase.cpp
# End Source File
# Begin Source File

SOURCE=..\src\IntegrationLanguages.cpp
# End Source File
# Begin Source File

SOURCE=..\src\MemberDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ObjQueryParameter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ProcedureCall.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Schema.cpp
# End Source File
# Begin Source File

SOURCE=..\src\SocketHTTPDataSource.cpp
# End Source File
# Begin Source File

SOURCE=..\src\StackFrameCheck.cpp
# End Source File
# Begin Source File

SOURCE=..\src\WinINetHTTPDataSource.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xmlAttribute.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xmlDataSource.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xmlElementTree.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xmlElementx.cpp
# End Source File
# Begin Source File

SOURCE=..\src\XMLException.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xmlLex.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xmlObject.cpp
# End Source File
# Begin Source File

SOURCE=..\src\xmlObjectFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\src\XMLProcedureDescriptor.cpp
# End Source File
# End Group
# Begin Group "inc"

# PROP Default_Filter ""
# Begin Group "ATL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\inc\ATL\atlalloc.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlbase.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlbase.inl
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlcache.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlchecked.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlcomcli.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\ATLComMem.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlconv.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlcore.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlcrypt.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlcrypt.inl
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atldebugapi.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atldef.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlexcept.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlextmgmt.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlhtml.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlhttp.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlhttp.inl
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\ATLiFace.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlisapi.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlmem.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlmime.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlpath.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlperf.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlperf.inl
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlplus.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlrc.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlrx.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlsession.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlsharedsvc.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlsiface.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlsimpcoll.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlsimpstr.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlsmtpconnection.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlsmtputil.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlsoap.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlspriv.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlspriv.inl
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlstencil.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atlstr.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\atltrace.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\BaseTsd.h
# End Source File
# Begin Source File

SOURCE=..\inc\CSmtp.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\cstringt.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\cstringt.inl
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\excpt.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\guiddef.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\OAIdl.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\ObjIdl.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\ocidl.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\rpc.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\RpcAsync.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\RpcDce.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\rpcdcep.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\rpcndr.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\RpcNsi.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\RpcNsip.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\RpcNtErr.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\sal.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\salNot.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\ServProv.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\specstrings.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\Unknwn.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\urlmon.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\WTypes.h
# End Source File
# Begin Source File

SOURCE=..\inc\ATL\WTypesbase.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\inc\AbstractionsGeneric.h
# End Source File
# Begin Source File

SOURCE=..\inc\AbstractionsMFC.h
# End Source File
# Begin Source File

SOURCE=..\inc\AbstractionsRougeWave.h
# End Source File
# Begin Source File

SOURCE=..\inc\AbstractionsSTD.h
# End Source File
# Begin Source File

SOURCE=..\inc\AttributeList.h
# End Source File
# Begin Source File

SOURCE=..\inc\Base64.h
# End Source File
# Begin Source File

SOURCE=..\inc\Base64Alternate.h
# End Source File
# Begin Source File

SOURCE=..\inc\BaseInterface.h
# End Source File
# Begin Source File

SOURCE=..\inc\BOLRelation.h
# End Source File
# Begin Source File

SOURCE=..\inc\CacheManager.h
# End Source File
# Begin Source File

SOURCE=..\inc\DynamicLibrary.h
# End Source File
# Begin Source File

SOURCE=..\inc\DynamicXMLObject.h
# End Source File
# Begin Source File

SOURCE=..\inc\ExceptionHandler.h
# End Source File
# Begin Source File

SOURCE=..\inc\FactoryManager.h
# End Source File
# Begin Source File

SOURCE=..\inc\FileDataSource.h
# End Source File
# Begin Source File

SOURCE=..\inc\FrameworkAuditLog.h
# End Source File
# Begin Source File

SOURCE=..\inc\garray.h
# End Source File
# Begin Source File

SOURCE=..\inc\GBTree.h
# End Source File
# Begin Source File

SOURCE=..\inc\GDirectory.h
# End Source File
# Begin Source File

SOURCE=..\inc\GException.h
# End Source File
# Begin Source File

SOURCE=..\inc\GHash.h
# End Source File
# Begin Source File

SOURCE=..\inc\GHTTPMultiPartPOST.h
# End Source File
# Begin Source File

SOURCE=..\inc\GList.h
# End Source File
# Begin Source File

SOURCE=..\inc\GListNodeCache.h
# End Source File
# Begin Source File

SOURCE=..\inc\GlobalInclude.h
# End Source File
# Begin Source File

SOURCE=..\inc\GPerformanceTimer.h
# End Source File
# Begin Source File

SOURCE=..\inc\GProcess.h
# End Source File
# Begin Source File

SOURCE=..\inc\GProfile.h
# End Source File
# Begin Source File

SOURCE=..\inc\GSocketHelpers.h
# End Source File
# Begin Source File

SOURCE=..\inc\GStack.h
# End Source File
# Begin Source File

SOURCE=..\inc\GString.h
# End Source File
# Begin Source File

SOURCE=..\inc\GString0.h
# End Source File
# Begin Source File

SOURCE=..\inc\GString32.h
# End Source File
# Begin Source File

SOURCE=..\inc\GStringCipherZip.h
# End Source File
# Begin Source File

SOURCE=..\inc\GStringList.h
# End Source File
# Begin Source File

SOURCE=..\inc\GStringStack.h
# End Source File
# Begin Source File

SOURCE=..\inc\GThread.h
# End Source File
# Begin Source File

SOURCE=..\inc\GZip.h
# End Source File
# Begin Source File

SOURCE=..\inc\IntegrationBase.h
# End Source File
# Begin Source File

SOURCE=..\inc\IntegrationLanguages.h
# End Source File
# Begin Source File

SOURCE=..\inc\ListAbstraction.h
# End Source File
# Begin Source File

SOURCE=..\inc\MD4xmlf.h
# End Source File
# Begin Source File

SOURCE=..\inc\md5.h
# End Source File
# Begin Source File

SOURCE=..\inc\md5Alternate.h
# End Source File
# Begin Source File

SOURCE=..\inc\MemberDescriptor.h
# End Source File
# Begin Source File

SOURCE=..\inc\MemberHandler.h
# End Source File
# Begin Source File

SOURCE=..\inc\oaidl.h
# End Source File
# Begin Source File

SOURCE=..\inc\ObjCacheQuery.h
# End Source File
# Begin Source File

SOURCE=..\inc\ObjectDataHandler.h
# End Source File
# Begin Source File

SOURCE=..\inc\ObjectPointer.h
# End Source File
# Begin Source File

SOURCE=..\inc\ObjQuery.h
# End Source File
# Begin Source File

SOURCE=..\inc\ObjQueryParameter.h
# End Source File
# Begin Source File

SOURCE=..\inc\ObjResultSet.h
# End Source File
# Begin Source File

SOURCE=..\inc\PluginBuilder.h
# End Source File
# Begin Source File

SOURCE=..\inc\PluginBuilderLowLevelStatic.h
# End Source File
# Begin Source File

SOURCE=..\inc\ProcedureCall.h
# End Source File
# Begin Source File

SOURCE=..\inc\RelationshipWrapper.h
# End Source File
# Begin Source File

SOURCE=..\inc\Schema.h
# End Source File
# Begin Source File

SOURCE=..\inc\sha256.h
# End Source File
# Begin Source File

SOURCE=..\inc\SocketHTTPDataSource.h
# End Source File
# Begin Source File

SOURCE=..\inc\StackFrameCheck.h
# End Source File
# Begin Source File

SOURCE=..\inc\StringAbstraction.h
# End Source File
# Begin Source File

SOURCE=..\inc\SwitchBoard.h
# End Source File
# Begin Source File

SOURCE=..\inc\TwoFish.h
# End Source File
# Begin Source File

SOURCE=..\inc\WinINetHTTPDataSource.h
# End Source File
# Begin Source File

SOURCE=..\inc\WinPhoneThreadEmulation.h
# End Source File
# Begin Source File

SOURCE=..\inc\WTypes.h
# End Source File
# Begin Source File

SOURCE=..\inc\xmlAttribute.h
# End Source File
# Begin Source File

SOURCE=..\inc\xmlDataSource.h
# End Source File
# Begin Source File

SOURCE=..\inc\xmlDefines.h
# End Source File
# Begin Source File

SOURCE=..\inc\xmlElement.h
# End Source File
# Begin Source File

SOURCE=..\inc\xmlElementTree.h
# End Source File
# Begin Source File

SOURCE=..\inc\xmlException.h
# End Source File
# Begin Source File

SOURCE=..\inc\XMLFoundation.h
# End Source File
# Begin Source File

SOURCE=..\inc\xmlLex.h
# End Source File
# Begin Source File

SOURCE=..\inc\xmlObject.h
# End Source File
# Begin Source File

SOURCE=..\inc\xmlObjectFactory.h
# End Source File
# Begin Source File

SOURCE=..\inc\XMLObjectLoader.h
# End Source File
# Begin Source File

SOURCE=..\inc\XMLProcedureDescriptor.h
# End Source File
# End Group
# Begin Group "Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Utils\Base64.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\BZip.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\CSmtp.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GArray.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GBTree.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GDirectory.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GException.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GHash.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GHTTPMultiPartPOST.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GList.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GMemory.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GPerformanceTimer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GProcess.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GProfile.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GSocketHelpers.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GStack.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GString.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GString0.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GString32.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GStringList.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GThread.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\GZip.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\PluginBuilderLowLevelStatic.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\PThread.cpp

!IF  "$(CFG)" == "XMLFoundationLib - Win32 Release"

!ELSEIF  "$(CFG)" == "XMLFoundationLib - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "XMLFoundationLib - Win32 DebugStaticCRT"

!ELSEIF  "$(CFG)" == "XMLFoundationLib - Win32 ReleaseStaticCRT"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Utils\SHA256.cpp
# End Source File
# Begin Source File

SOURCE=..\src\SwitchBoard.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Utils\TwoFish.cpp
# End Source File
# End Group
# End Target
# End Project
