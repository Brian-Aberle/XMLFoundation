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


#ifdef _WIN32
	// symbols truncated in the MSVC debug browser database 
	#pragma warning (disable:4786)
#endif
#ifndef _CRT_RAND_S
    #define _CRT_RAND_S // needed for rand_s() must be defined before #include <stdlib.h"
#endif


#include "xmlDefines.h"

#include "GString.h"
#include "xmlAttribute.h"
#include "xmlElement.h"
#include "xmlElementTree.h"
#include "xmlObject.h"
#include "FactoryManager.h"
#include "GException.h"
#include "xmlException.h"
#include "ObjQuery.h"
#include "ObjResultSet.h"
#include "ObjectPointer.h"
#include "ObjQueryParameter.h"
#include "SocketHTTPDataSource.h"
#include "GList.h"
#include "GHash.h"
#include "GStringList.h"
#include "GBTree.h"
#include "AbstractionsGeneric.h"
#include "FrameworkAuditLog.h"
#include "GThread.h"
#include "ExceptionHandler.h"
#include "GDirectory.h"
#include "CacheManager.h"
#include "Base64.h"
#include "TwoFish.h"
#include "GZip.h"
#include "SHA256.h"
#include "SHA512xmlf.h"
#include "GProfile.h"

