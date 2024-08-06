// --------------------------------------------------------------------------
//						United Business Technologies
//			  Copyright (c) 2000 - 2010  All Rights Reserved.
//
// Source in this file is released to the public under the following license:
// --------------------------------------------------------------------------
// This toolkit may be used free of charge for any purpose including corporate
// and academic use.  For profit, and Non-Profit uses are permitted.
//
// This source code and any work derived from this source code must retain 
// this copyright at the top of each source file.
// --------------------------------------------------------------------------
#include "GlobalInclude.h"
#ifndef _LIBRARY_IN_1_FILE
static char SOURCE_FILE[] = __FILE__;
#endif


#include "StackFrameCheck.h"
#include "xmlObject.h"

#ifdef _WIN32			
#include <intrin.h>
#else			
#include <signal.h>
#endif

StackFrameCheck::StackFrameCheck( void *pCurrentFrame, StackFrameCheck *pStack )
{
	m_pStack = 0;
	m_bOwner = false;

	if (pStack && (pStack->m_pStack->m_nNext > pStack->m_pStack->m_nSize))
	{
		return;
	}


	if ( pStack )
	{
		m_bOwner = false;
		m_pStack = pStack->m_pStack;
	}
	else // this is the first frame in the call stack
	{
		m_pStack = new GStack(512,512);
		m_bOwner = true;		// only frame 0 is the owner of the stack
	}
	if (m_pStack)
	{
		m_bNestedFrame = false;
//		__int64 nFrames = m_pStack->Size();   //2024
		__int64 nFrames = m_pStack->m_nNext;
		if ( nFrames )
		{
			for(__int64 i=0;i<nFrames-1; i++)
			{
				if (m_pStack->m_arrPtr[i] == pCurrentFrame)
				{
					m_bNestedFrame = true;
					break;
				}
			}

		}
		GStackPush( (*m_pStack) , pCurrentFrame  );
	}

}

StackFrameCheck::~StackFrameCheck(  )
{
	if (m_pStack)
	{
		if (m_pStack->m_nNext < m_pStack->m_nSize)
		{
			m_pStack->RemoveAll();
			//			GStackPop((*m_pStack));  // 2024

			if (m_bOwner)
			{
				delete m_pStack;
			}
		}
	}
}
