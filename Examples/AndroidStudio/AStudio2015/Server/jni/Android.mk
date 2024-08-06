LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

##LOCAL_STATIC_LIBRARY := stdc++ 
##LOCAL_LDLIBS     := -llog

# -D__NO_OPENSSL

LOCAL_LDFLAGS += -lc:\XMLFoundation\Libraries\openssl\bin-androidARM\libssl.a
LOCAL_LDFLAGS += -lc:\XMLFoundation\Libraries\openssl\bin-androidARM\libcrypto.a


LOCAL_C_INCLUDES := c:\XMLFoundation\Libraries\XMLFoundation\inc
LOCAL_C_INCLUDES += c:\XMLFoundation\Libraries\XMLFoundation\src\JNI
LOCAL_CPPFLAGS   := -frtti -fexceptions -c -O3 -w -D_LINUX -D_ANDROID -fpermissive 
LOCAL_PATH       := c:\XMLFoundation\Libraries\XMLFoundation\src
LOCAL_MODULE    := Server
LOCAL_SRC_FILES :=	 ..\..\..\Examples\Android\Server\jni\Server.cpp\
					 ..\..\..\Servers\Core\ServerCore.cpp\
					 Utils\GSocketHelpers.cpp\
					 Utils\GArray.cpp\
					 Utils\CSmtp.cpp\
					 Utils\GBTree.cpp\
					 Utils\GException.cpp\
					 Utils\GHash.cpp\
					 Utils\GList.cpp\
					 Utils\GProfile.cpp\
					 Utils\GStack.cpp\
					 Utils\GString.cpp\
					 Utils\GString0.cpp\
					 Utils\GString32.cpp\
					 Utils\GStringList.cpp\
					 Utils\GDirectory.cpp\
					 Utils\GPerformanceTimer.cpp\
					 Utils\TwoFish.cpp\
					 Utils\SHA256.cpp\
					 Utils\BZip.cpp\
					 Utils\GZip.cpp\
					 Utils\Base64.cpp\
					 Utils\PluginBuilderLowLevelStatic.cpp\
					 Utils\GHTTPMultiPartPOST.cpp\
					 AttributeList.cpp\
					 CacheManager.cpp\
					 FactoryManager.cpp\
					 FrameworkAuditLog.cpp\
					 MemberDescriptor.cpp\
					 FileDataSource.cpp\
					 ObjQueryParameter.cpp\
					 ProcedureCall.cpp\
					 Schema.cpp\
					 SocketHTTPDataSource.cpp\
					 StackFrameCheck.cpp\
					 xmlAttribute.cpp\
					 xmlDataSource.cpp\
					 xmlElement.cpp\
					 xmlElementTree.cpp\
					 XMLException.cpp\
					 xmlLex.cpp\
					 xmlObject.cpp\
					 xmlObjectFactory.cpp\
					 XMLProcedureDescriptor.cpp\
					 SwitchBoard.cpp\
					 DynamicLibrary.cpp\
					 IntegrationBase.cpp\
					 IntegrationLanguages.cpp


include $(BUILD_SHARED_LIBRARY)
