# setup the Android SDK then setup the Android NDK then build the NDK samples like "hello-jni" as a prererquisite.
# create your own project -  Place this Android.mk file in a folder named jni or cpp



LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := c:\XMLFoundation\Libraries\XMLFoundation\inc
LOCAL_CPPFLAGS   := -frtti -fexceptions -c -O3 -w -D___LINUX -D___ANDROID -D___ARM64
LOCAL_PATH       := c:\XMLFoundation\Libraries\XMLFoundation\src

LOCAL_MODULE    := Example
LOCAL_SRC_FILES :=	 JNI\Example.c\
					 ..\..\..\Servers\Core\ServerCore.cpp\
					 Utils\GSocketHelpers.cpp\
					 Utils\GArray.cpp\
					 Utils\GBTree.cpp\
					 Utils\GException.cpp\
					 Utils\GHash.cpp\
					 Utils\GList.cpp\
					 Utils\GProfile.cpp\
					 Utils\GStack.cpp\
					 Utils\GString.cpp\
					 Utils\GStringList.cpp\
					 Utils\GDirectory.cpp\
					 Utils\GPerformanceTimer.cpp\
					 Utils\TwoFish.cpp\
					 Utils\SHA256.cpp\
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