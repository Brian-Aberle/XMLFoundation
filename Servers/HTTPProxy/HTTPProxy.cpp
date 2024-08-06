// HTTPProxy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
//#include <sys/types.h>
//#include <string.h>




#define MAX_SOCKET_CHUNK	65536

#include "../Core/ServerCore.cpp"


GString strServerName("HTTPProxy"); 
GString strServerDescription("Share your Internet connection via HTTP Proxy");
GString strServerPassword;
char pzServer[256];
char g_ThisEXE[512]; // the runtime path and file name of this executable == argv[0]


char* pzBoundStartupConfig =
"[System]\r\n"
"Pool=200\r\n"
"ProxyPool=200\r\n"
"LogFile=C:\\HTTPProxyLog.txt\r\n"

"[Trace]\r\n"
"Enable=yes\r\n"
"HTTPProxyTrace=0\r\n"
"HTTPProxyReturnTrace=0\r\n"
"ConnectTrace=1\r\n"

// 2024 note:  The HTTP proxy uses old Encryption Algorithms within an HMAC authentication that uses OpenSSL.
// XMLFoundation 2024 contains SHA512 and will also completely contain the HMAC algorithm to break the dependancy on OpenSSL.
// This 'old' implementation of the HTTP Proxy probably wont authenticate with most modern software that uses authentication.
// Thew HTTP Proxy code in ServerCore.cpp is under re-construction in 2024.
"[HTTPProxy]\r\n"
"Enable=yes\r\n"
"HTTPProxyAgent=HTTPProxy\r\n"
"RequireAuth=0\r\n"
"Port=10888\r\n"				// defaults to port 10888 - feel free to change it.
"LogEnabled=0\r\n"
"LogFileName=\r\n"			// if LogEnabled is true set LogFileName=c:\MyLogFile.txt 
"Timeout=300\r\n";

/*
[HTTPProxy]
Enable=1
Port=8080
HTTPProxyAgent=InternetViaProxy
RequireAuth=1
Timeout=300
BlockedDomains=
LogEnabled=0
RestrictIPs=1
Only443or80=1
NegotiateBasic=1
BlockHTTPProxyAds=1
EnableOutboundConnectCache=0
NegotiateNTLMv2=1
LogHTTPTxnFiles=0
RestrictBeginMatch=10.0.;192.168.
LogFileName=C:\Users\theboss\Desktop\HTTPProxyLog.txt
*/

int g_isRunning = 1;

void ConsoleCommand(char* pzCommand)
{
	GString strCmdIn(pzCommand);

	if (strCmdIn.CompareNoCase("debug") == 0)
	{
		printf("Enter 1 to enable debug or 0 to disable it\n");
		int nDebug = GetUserBoolean();

		g_TraceConnection = nDebug;
		g_bTraceHTTPProxy = nDebug;
//		g_bTraceHTTPProxyReturn = 0;  // normally too much data top look at
//		g_bTraceHTTPProxyFiles = 0;	  // enable to write individual log files, 1 per HTTP transaction kind.
		g_bTraceHTTPProxyBinary = nDebug;
		g_bHTTPProxyLogExtra = nDebug;

		g_strHTTPProxyAttemptsFile = "HTTPProxyLogAttempts.txt";
		g_strLogFile = "HTTPProxyLog.txt"; // setting g_strLogFile = "" will improve performance and disable all logging

		printf("OK\n");
	}
	else if (strCmdIn.CompareNoCase("subnets") == 0 )
	{
		printf("Enter a ; seperated list of IP subnet prefixes to restrict proxy operations for\n");
		printf("for example      192.168;10.0.");
		printf("or press enter to proxy for all networks");
		GString strRestrictBeginMatch ( GetUserCommand() );
		if (strRestrictBeginMatch.IsEmpty())
		{
			g_bHTTPProxyRestrictToSubnet = 1;
			if (g_pstrHTTPProxyRestrictList)
				delete g_pstrHTTPProxyRestrictList;
				g_pstrHTTPProxyRestrictList = new GStringList(";", strRestrictBeginMatch);
		}
		printf("OK\n");
	}
	else if (strCmdIn.CompareNoCase("stop") == 0 ||
		strCmdIn.CompareNoCase("quit") == 0 ||
		strCmdIn.CompareNoCase("exit") == 0)
	{
		printf("Stopping and releasing all ports.\n");
		stopListeners();
		printf("All ports released, no new transactions are being accepted.\n");


		printf("System Shutdown in progress.");
		g_isRunning = 0;
	}

}



int main(int argc, char* argv[])
{
	char g_szAppName[] = "HTTPProxy";

	strcpy(g_ThisEXE, argv[0]);
#ifndef _WIN32
	struct termio original_io;
	SetTermIO(1, &original_io, 1);
#endif

	///////////////////////////////////////////////////////////////////////////////////
	GString strPort("10888");

	GString strCfgData;
	strCfgData.Format(pzBoundStartupConfig, (const char*)strPort);
	SetProfile(new GProfile((const char*)strCfgData, (int)strCfgData.Length(), 0));

	//Load BlackListed.txt from the same folder as this .exe or from the working directory
	GString strSameDirAsStartupFile(g_ThisEXE);
	strSameDirAsStartupFile.SetLength(strSameDirAsStartupFile.ReverseFind('\\'));
	strSameDirAsStartupFile << '\\' << "BlackListed.txt";
	__int64 nAddedToBlacklist = g_BlacklistedDomains.BulkInsertFromFile("\r\n", strSameDirAsStartupFile);
	if (!nAddedToBlacklist)
	{
		// try the working directory
		nAddedToBlacklist = g_BlacklistedDomains.BulkInsertFromFile("\r\n", "BlackListed.txt");
	}
	printf("%lld entries loaded from BlackListed.txt\n", nAddedToBlacklist);




	GString strPrompt("\n");
	strPrompt << "HTTPProxy" << ">";

	ExceptionHandlerScope durationOfprocess;
NEVER_DIE:
	___TRY_ALL
	{
		try
		{
			if (!server_start("Starting HTTPProxy Server....."))
				return 0;
			printf(" Server Started.\n");
			printf(strPrompt._str);


			char* pzFirstCommandRead = GetUserCommand();
			printf(strPrompt._str);


			while (g_isRunning)
			{
				char* pzCommand = GetUserCommand();
				if (pzCommand)
				{
					ConsoleCommand(pzCommand);
					printf(strPrompt._str);
					fflush(stdout);
				}
				else
				{
					// this can happen on some linux when compiling without the -maccumulate-outgoing-args option in gcc

					// it also happens when the process is launched through a linux System V script as a background process
					// argc will be 2 in this case because the startup file was passed to the process from the shell script

					if (argc == 1) // if startup arguments are passed in then do not display the warning or non functioning prompt
					{
						printf("\nCommand line input has been disabled\n");
						printf(strPrompt._str);

					}
					else // since we have no console prompt, write a pidfile 
					{
						#ifndef _WIN32
							GString strUnixPIDFile("var/run/");
							strUnixPIDFile << g_szAppName << ".pid";
							strUnixPIDFile.MakeLower();

							const char* pzPIDFile = GetProfile().GetStringOrDefault("System", "PidFile", strUnixPIDFile);

							GString strPIDFile;
							strPIDFile << (long)getpid();
							unlink(pzPIDFile);
							strPIDFile.ToFile(pzPIDFile);
						#endif
					}

					// 10 second Sleep
					gtimespec ts;
					ts.tv_sec = time(NULL) + 10;
					ts.tv_nsec = 0;
					gthread_cond_t _TimeoutCond;
					gthread_mutex_t _TimeoutLock;
					gthread_mutex_init(&_TimeoutLock,0);
					gthread_cond_init(&_TimeoutCond,0);
					gthread_mutex_lock(&_TimeoutLock);
					gthread_cond_timedwait(&_TimeoutCond, &_TimeoutLock, &ts);
				}
			}
		}
		catch (GException& e)
		{
			if (e.GetError() == 2) // the config file could not be found
			{
				// todo: try to find the file then 
				// goto NEVER_DIE;
				}

			// otherwise print the "Cant find config file" error, and exit this application.
			printf(e.GetDescription());
			return 0;

		}

		catch (...)
		{
			printf("Error C runtime exception.\n");
		}
	}
	___CATCH_ALL(ex)
	{
		printf("XML_CATCH=%s\n", (const char*)ex.GetDescription());
	}

	if (g_isRunning)
		goto NEVER_DIE;

#ifndef _WIN32
	SetTermIO(0, &original_io, 1);
#endif
	server_stop();
	return 0;
}
