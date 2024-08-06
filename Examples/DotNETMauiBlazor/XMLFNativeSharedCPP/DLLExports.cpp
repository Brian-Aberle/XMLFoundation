#pragma message("Start Building DLLExports.cpp......A")
#define _CRT_RAND_S     // needed for rand_s() on Windows.  This must be defined prior to including  <stdlib.h>

// This makes a big difference in build order, only building dependencies as they are needed, and even less dependencies in some applications.
#define WIN32_LEAN_AND_MEAN


#include <stdlib.h>  

#include "DLLExports.h"
#include <direct.h>  // for _getcwd

#pragma message("      Building DLLExports.cpp......B")
#include "XMLFoundation.h"

#pragma message("      Building DLLExports.cpp......C")



/////////////////////////     INCLUDES ALL SOURCE .CPP files from XMLFoundation  ///////////////////////////////////////////////////
// 
// This project searches the #include path from the XMLFoundation souce distro according to the Projects 
// Properties->C++->General->"Additional Include Directories" setting = [C:\XMLFoundation\Libraries\XMLFoundation\inc]
// 
//									  relative to : [/XMLFoundation/Libraries/XMLFoundation/inc]
//#include "../src/XMLFoundation.cpp"	// is found at: [/XMLFoundation/Libraries/XMLFoundation/src]
/////////////////////////     INCLUDES ALL SOURCE .CPP files from XMLFoundation  ///////////////////////////////////////////////////


GProfile* g_AppProfile = 0;
int isRunning = 0;
GString g_strLastError;

#include "XMLFoundation.h"
#include "XMLFoundationApp.h"

	XMLFoundationApp* CreateAppHandle()
	{
		g_strLastError = "ok";
		return new XMLFoundationApp();
	}

	int GetLastAppError(XMLFoundationApp* pApp, char* pInOutBuffer, __int64* nBufSize)
	{
		if (pApp != nullptr)
		{
			strncpy(pInOutBuffer, pApp->m_strLastAppErr, *nBufSize);
			*nBufSize = pApp->m_strLastAppErr.Length();
		}
		return 0;
	}

	int GetAppResultString(XMLFoundationApp* pApp, char* pzInOutString, __int64* nBufSize, int nType)
	{
		if (pApp != nullptr)
		{
			if (pApp->Example(nType) == 0)
			{
				strncpy(pzInOutString, pApp->m_strExampleOutput, *nBufSize);
				*nBufSize = pApp->m_strExampleOutput.Length();
			}
			else
			{
				strncpy(pzInOutString, pApp->m_strLastAppErr, *nBufSize);
				*nBufSize = pApp->m_strLastAppErr.Length();
			}
			return 0;
		}
		return -999;
	}


	int SetAppValue(XMLFoundationApp* pApp, const char *sSection, const char* sEntry, const char* sValue)
	{
		if (pApp != nullptr)
		{
			return pApp->SetAppValue(sSection, sEntry, sValue);
		}
		return -999;
	}
	
	int GetAppValue(XMLFoundationApp* pApp, const char* sSection, const char* sEntry, char* pInOutBuffer, __int64* nBufSize)
	{
		if (pApp != nullptr)
		{
			return pApp->GetAppValue(sSection, sEntry, pInOutBuffer, nBufSize);
		}
		return -999;
	}

	XMLResultset* GetAllAppValues(XMLFoundationApp* pApp, char* pResultsBuf, __int64* pBufLength, bool bAsXML /*=0*/, const char* pzSectionOnly /*= 0*/)
	{
		return pApp->GetAllAppValues(pResultsBuf, pBufLength, bAsXML, pzSectionOnly);
	}

	// in this case, pnDataLength is the length of the data NOT the size of the buffer
	int SetAllAppValues(XMLFoundationApp* pApp, char* pResultsBuf, __int64* pDataLen, bool bAsXML/* = 0*/)
	{
		if (pApp != nullptr)
		{
			return pApp->SetAllAppValues(pResultsBuf, pDataLen, bAsXML);
		}
		return -999;
	}

	
	int InsertOrUpdateCustomer(XMLFoundationApp* pApp, unsigned __int64 nID, const char* pzName, const char* pzColor, const char* strListItems)
	{
		if (pApp != nullptr)
		{
			return pApp->InsertOrUpdateCustomer( nID, pzName, pzColor, strListItems);
		}
		return -999;
	}
	int InsertOrUpdateAddress(XMLFoundationApp* pApp, unsigned __int64 nCustID, const char* strLine1, const char* strLine2, const char* strCity, const char* strState, const char* strZip, int nAddrType)
	{
		if (pApp != nullptr)
		{
			return pApp->InsertOrUpdateAddress(nCustID, strLine1, strLine2, strCity, strState, strZip, nAddrType);
		}
		return -999;
	}

	int FromXML(XMLFoundationApp* pApp, const char* pzXML)
	{
		if (pApp != nullptr)
		{
			return pApp->FromXML(pzXML);
		}
		return -999;
	}


	XMLResultset* ToXML(XMLFoundationApp* pApp, char* pResultsBuf, __int64* pBufLength, int nTabs/* = 0*/, const char* TagOverride/* = 0*/, unsigned int nSerializeFlags/* = 128*/, const char* pzSubsetByTagName/* = 0*/, bool bAppend/* = 0*/)
	{
		return pApp->ToXML(pResultsBuf, pBufLength, nTabs, TagOverride, nSerializeFlags, pzSubsetByTagName, bAppend);
	}

	void DeleteResultsetHandle(XMLResultset* pResultset)
	{
		if (pResultset != 0 && pResultset != g_pEmptyXMLResultSet)
		{
			delete pResultset;
			pResultset = 0;
		}
	}

	void DeleteAppHandle(XMLFoundationApp* pApp)
	{
		if (pApp != nullptr)
		{
			delete pApp;
			pApp = nullptr;
		}
	}

#ifndef XMLF_NO_SERVER_CORE_AUTO_LINK
#ifndef _NO_XMLF_SERVICES
	// Either #include "ServerCore.cpp"  ||or   add that source file to the project 
	#include "ServerCore.h"
	
	int ServerStart(const char* pzStartupMessage)
	{
		if (!isRunning)
		{
			try
			{
				GString s(1024);
#ifdef _UNICODE
				wchar_t buf[512];
				GetModuleFileName(0, buf, sizeof(buf));
				s.FromUnicode(buf);
#else
				GetModuleFileName(0, s._str, (unsigned long)s._len);
				s._len = strlen(s._str);
#endif
				GString strStartupMessage;
				strStartupMessage << "XMLFoundationWindows.DLL [" << s << "]";

				int nRet = server_start(strStartupMessage, &g_strLastError);
				if (nRet)
				{
					isRunning = 1;
					return 0; // success
				}
				return -1; // fail

			}
			catch (GException& e)
			{
				if (e.GetError() == 2)
				{
					g_strLastError = "Startup Password required";
				}
				else
				{
					g_strLastError = e.GetDescription();
				}
				return -2;
			}
			catch (...)
			{
				g_strLastError = "Unknown startup error";
				return -3;
			}
		}
		return 0; // success - it was already running
	}



	// The section/entry will be created when it does not already exist
	// The existing value will be replaced with the supplied value when it does exist
	void SetCoreValue(const char* pzSection, const char* pzEntry, const char* pzValue)
	{
		GetProfile().SetConfig(pzSection, pzEntry, pzValue);
	}

	int GetCoreValue(char* pInOutBuffer, __int64* nBufSize, const char* pzSection, const char* pzEntry)
	{
		// retrieves a character string value from the specified section
		const char* pzReturn = GetProfile().GetString(pzSection, pzEntry, false); // will not throw exceptions
		if (pzReturn && pzReturn[0])
		{
			// this will truncate the returned value if the supplied results buffer is too small .
			__int64 nReturnLen = strlen(pzReturn);
			memcpy((void*)pInOutBuffer, pzReturn, ((nReturnLen + 1) > *nBufSize) ? *nBufSize - 1 : nReturnLen + 1);
			*nBufSize = ((nReturnLen + 1) > *nBufSize) ? *nBufSize - 1 : nReturnLen;
			return 0; // success
		}
		else
		{
			g_strLastError.Empty();
			g_strLastError << "Section [" << pzSection << "] Entry [" << pzEntry << "] not found in XMLFoundationApp::GetAppValue()";
			return -1; // not found
		}
	}

	int GetLogFileData(char* pResultsBuf, __int64* pBufLength)
	{
		XMLResultset* pResultSet = new XMLResultset(pResultsBuf, 0, *pBufLength);
		// ONLY LOAD the tail [*pBufLength] bytes so that the result always fits in the supplied buffer
		
		GString strLogData;
		const char* pzFileName = GetProfile().GetString("System", "LogFile", false); // will not throw exceptions
		if (pzFileName)
		{
			strLogData.FromFile(pzFileName, 0);
		}
		else
		{
			return -1; 
		}
		if (!strLogData.Length())
		{
			return -2;
		}
		if (strLogData.Length() < *pBufLength)
		{
			memcpy((void*)pResultsBuf, strLogData.Buf(), strLogData.Length() + 1);
			*pBufLength = strLogData.Length();
		}
		else
		{
			memcpy((void*)pResultsBuf, strLogData.StartingAt(strLogData.Length() - *pBufLength - 1), *pBufLength - 1);
			pResultsBuf[*pBufLength - 1] = 0;
		}
		return 0;
	}

	XMLResultset* GetAllCoreValues(char* pResultsBuf, __int64* pBufLength, bool bAsXML/* = 0*/, const char* pzSectionOnly/* = 0*/)
	{
		try
		{
			// this XMLResultset is a GString that uses the (callers memory space)[pResultsBuf] and makes no allocation unless the data becomes larger 
			// than [*pBufLength] in which case it will then perform an allocation to memory in (our memory space) with room to grow and "own" the new block 
			// of memory unlike  the initial block that this ::GString [bOwns=false] so it leaves memory cleanup to the owner and ~GString dtor and will not delete it.
			XMLResultset* pResultSet = new XMLResultset(pResultsBuf, 0, *pBufLength);
			__int64 nLength = GetProfile().WriteCurrentConfig(pResultSet, bAsXML);

			if (pResultSet->Length() > *pBufLength)
			{
//				//  this can be handled with a callback or returned to the caller with an indexable resultset allowing the app to keep it for an ideal amount of time
//				return pResultSet; // success - with more data. The caller MUST call DeleteResultsetHandle() when finished.
				*pBufLength = pResultSet->Length();// the caller should know to get more data from the resultset because *pBufLength, the total bytes of result, was > sizeof(pResultsBuf)
				return pResultSet; // success - with more data. The caller MUST call DeleteResultsetHandle() when finished.  The first [sizeof(pResultsBuf)] bytes are already returned in [pResultsBuf] as the GString grew allocation space the first working block with the start of the results detached from thje GString

			}
			else // the entire resultset fit into the callers memory space, the entire result set is already returned
			{	 // let the caller know the length of the results.
				*pBufLength = pResultSet->Length();
				delete pResultSet; // deletes only the attached class - leaves the memory alone in destruction 
				return g_pEmptyXMLResultSet; // success.  The caller can call DeleteResultsetHandle() when finished, however the call is ignored for this special resultset [g_pEmptyXMLResultSet]
			}
		}
		catch (GException& e)
		{
			g_strLastError = e.GetDescription();
			return nullptr;
		}
	}
	
	int SetAllCoreValues(const char* pConfig, bool bAsXML/* = 0*/)
	{
		try
		{
			GProfile* pGPro = new GProfile(pConfig, strlen(pConfig), bAsXML);
			if (pGPro)
			{
				// deletes the old, assigns the new
				delete SetProfile(pGPro);
				return 0; // success
			}
			else
			{
				return -1; // out of memory
			}
		}
		catch (GException& e)
		{
			g_strLastError = e.GetDescription();
			return -2;
		}
		catch (...)
		{
			g_strLastError = "Runtime exception in SetAllCoreValues()";
			return -3;
		}
	}

	int CoreCommand(char* pInOutBuffer, __int64* nBufSize, const char* pzAuthKey, const char* pzCmd, const char* Arg1, const char* Arg2, const char* Arg3)
	{
		DWORD tid = GetCurrentThreadId();
		GString strLog;
		strLog << "CoreCommand() ----> tid:" << tid << " cmd[" << pzCmd << "] bufsize[" << *nBufSize << "] auth[" << pzAuthKey << "] 1[" << Arg1 << "] 2[" << Arg2 << "] 3[" << Arg3 << "]\r\n";
//		logg(strLog);
		strLog.Empty();

		// this XMLResultset is a GString that uses the (callers memory space)[pResultsBuf] and makes no allocation unless the data becomes larger 
		// than [*pBufLength] in which case it will then perform an allocation to memory in (our memory space) with room to grow and "own" the new block 
		// of memory unlike  the initial block that this ::GString [bOwns=false] so it leaves memory cleanup to the owner and ~GString dtor and will not delete it.
		// All the documentation found in GString.h could and probably should be an exposed interface
		XMLResultset* strResult = new XMLResultset(pInOutBuffer, 0, *nBufSize);
//		or simply...
//		GString* strResult = new GString(pInOutBuffer, 0, *nBufSize);
// 
// either way - next there must be a simple interface to GString with, at minimal - a way to "start at" the supplied index uptothe first null or 
// nMaxCount to return the next block in the result set produced by [pzCmd]. - Currently(2024) none of the commands create a return value that 
// will not fit within 32768 memory space which we know that we have - looking to the future that could and likely will change so - its where
// the new development for .Net is at July 22, 2024 There is enough accomplished that it can be useful already.

		try
		{
			GString cmd(pzCmd);

			//			Hashed and Salted User passwords stored in the system configuration settings
			//			The UserProfile is somewhat based on Linux/Unix system design with an Environment and default(home) folder per user - not using a shadow file currently.
			// 
			//			UserProfile User;
			//			Permission(pzAuthKey, pzCmd, &User);


			int nRetVal = 0; // success

			// information about configured socket ports
			if (cmd.CompareNoCase("ports") == 0)
			{
				viewPorts(strResult);

			}
			else if (cmd.CompareNoCase("active") == 0)
			{
				// returns 
				// tid,protocol,bytes,action,outPort,inPort,IP,time,"Action"\r\n
				// for each active thread
				showActiveThreads(strResult);
			}
			else if (cmd.CompareNoCase("start") == 0)
			{
				int nRet = ServerStart("Windows Interop DLL");
				return nRet; // ServerStart() already set g_strLastError if the start failed
			}
			else if (cmd.CompareNoCase("stop") == 0)
			{
				server_stop();
			}
			else
			{
				g_strLastError.Empty();
				g_strLastError << "Unknown command [" << pzCmd << "] passed to CoreCommand()";
				return -4;
			}

			return 0;
		}
		catch (GException& e)
		{
			g_strLastError = e.GetDescription();
			return -1;
		}
		catch (...)
		{
			g_strLastError = "Fatal error processing command[";
			g_strLastError << pzCmd << "] with Arg1[" << Arg1 << "]  Arg2[" << Arg2 << "]   Arg3[" << Arg3 << "]  Len:" << *nBufSize << " Buf[" << pInOutBuffer << "]";

			strLog << "CoreCommand() !!!!!!! ----> " << g_strLastError << " \r\n";
			strLog.ToFileAppend("c:\\software\\log.txt");
			strLog.Empty();

			return -2;
		}
		return -3;  // unreached code

	}

	int GetLastServerError(char* pInOutBuffer, __int64* nBufSize)
	try
	{
		if (g_strLastError.IsEmpty())
			g_strLastError = "Success";

		strncpy(pInOutBuffer, g_strLastError._str, *nBufSize);
		if (pInOutBuffer[*nBufSize - 1] != '\0')
		{
			// insuffient destination memory - truncate.
			pInOutBuffer[*nBufSize - 1] = 0;
		}
		*nBufSize = g_strLastError.Length();
		return 1;
	}
	catch (GException& e)
	{
		strncpy(pInOutBuffer, e.GetDescription(), *nBufSize);
		return 777;
	}
	catch (...)
	{
		strncpy(pInOutBuffer, "888", *nBufSize);
		return 888;
	}


	int GetMemberValue(char* pInOutBuffer, __int64* nBufSize, const char* pzOIDClassName, const char* pzTagName, int nSource)
	{
		GString strDest(pInOutBuffer, 0, *nBufSize, 0);
		if ( cacheManager.getMappedValue(pzOIDClassName, pzTagName, &strDest, nSource) )
		{
			*nBufSize = strDest.Length();
			return 1; //success
		}
		return 0; // an instance of pzOIDClassName was not found or [pzTagName] was not mapped
	}

	int SetMemberValue(const char* pzOIDClassName, const char* pzTagName, const char* pzValue)
	{
		if (cacheManager.setMappedValue(pzOIDClassName, pzTagName, pzValue))
		{
			return 1; //success
		}
		return 0; // an instance of pzOIDClassName was not found or [pzTagName] was not mapped
	}

	bool IsXMLObject(const char* pzOIDClassName)
	{
		if (cacheManager.findObject(pzOIDClassName))
		{
			return true; // object found
		}
		return false; // Object not found
	}

	XMLObject* GetXMLObjectRef(const char* pzOIDClassName)
	{
		XMLObject* pO = cacheManager.findObject(pzOIDClassName);
		if (pO)
		{
			pO->IncRef();
			return pO; //success
		}
		return 0; // NULL - an instance of pzOIDClassName was not found 
	}
	void FreeXMLObjectRef(XMLObject* pObj)
	{
		if (pObj)
		{
			pObj->DecRef();
		}
	}


#endif // _NO_XMLF_SERVICES
#endif

#pragma message("Done Building DLLExports.cpp......!!!")



