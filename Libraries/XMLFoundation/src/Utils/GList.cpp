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


#include "GList.h"
#include "GListNodeCache.h"
#include "GException.h"

static GListNodeCache g_GListNodeCache;
int GList::bEnableGlobalNodeCache = 1;


// This works, to cache within an overloaded operator delete, however it may not always be desired
/*
void GList::Node::operator delete(void*pN)
{
	if (g_GListNodeCache.m_Init == 777)
		g_GListNodeCache.Put((GList::Node*)pN);

};
*/

void GList::AppendList(const GList *pListToCopy)
{
	if (pListToCopy)
	{
		GListIterator it( pListToCopy );
		while (it())
		{
			void *p = (void *)it++;
			AddLast(p);
		}
	}
}
void GList::DeferDestruction()
{
	nDeferDestruction = 1;
}

// if bUseGlobalCache = true then it will use 1 cache for all GLists application wide
// if bUseGlobalCache = false then it will use a cache dedicated for this instance of a GList
void GList::EnableCache(bool bAutoGarbageCollect, bool bUseGlobalCache)
{
	if (!pNodeCache)
	{
		if (bUseGlobalCache)
		{
			pNodeCache = &g_GListNodeCache;
			bOwnsCache = 0;
		}
		else
		{
			pNodeCache = new GListNodeCache();
			bOwnsCache = 1;
		}
	}

	pNodeCache->AutoGarbageCollect(bAutoGarbageCollect);
}


void GList::Destruction()
{
	if (nDeferDestruction)
	{
		while (FirstNode)
		{
			if (CurrentNode == FirstNode) CurrentNode = FirstNode->NextNode;
			if (LastNode == FirstNode) LastNode = 0;
			Node *Save = FirstNode->NextNode;

			// put the node in the cache or delete it
			if (pNodeCache)
				pNodeCache->Put(FirstNode); 
			else 
				::delete FirstNode;

			FirstNode = Save;
			iSize--;
		}
	}
}

GList::GList(const GList &cpy)
{
	// constructs an initially empty list
	FirstNode = LastNode = CurrentNode = 0;
	iSize = 0;
	nDeferDestruction = 0;
	pNodeCache = 0;
	bOwnsCache = 0;
	
	EnableCache(0, 1);

	AppendList(&cpy);
}

void GList::operator=(const GList &cpy)
{
	AppendList(&cpy);
}

GList::GList()
{
	// constructs an initially empty list
	FirstNode = LastNode = CurrentNode = 0;
	iSize = 0;
	nDeferDestruction = 0;
	pNodeCache = 0;
	bOwnsCache = 0;

	EnableCache(0, 1);

}

GList::~GList()
{
	if (!nDeferDestruction)
	{
	
		while (FirstNode)
		{
			// the following 3 lines are an inline RemoveFirst()
			if (CurrentNode == FirstNode) CurrentNode = FirstNode->NextNode;
			if (LastNode == FirstNode) LastNode = 0;
			Node *Save = FirstNode->NextNode;
			
			if (pNodeCache)
			{
				// we always cache the Node UNLESS the process is ending and the global cache we are using has already been destroyed.  In that case pNodeCache->m_Init will == 21
				if (pNodeCache->m_Init == 777)
					pNodeCache->Put(FirstNode);
			}
			else
			{
				delete FirstNode;
			}
			
			FirstNode = Save;
			iSize--;
		}


		if (pNodeCache && bOwnsCache)
			delete pNodeCache;
	}
}

void * GList::First() const
{
	if (!FirstNode) 
	{
		return 0;
	}
	GList *pThis = (GList *)this;
	pThis->CurrentNode = FirstNode;
	return FirstNode->Data;
}

void * GList::Last() const
{
	if (!LastNode) 
	{
		return 0;
	}
	GList *pThis = (GList *)this;
	pThis->CurrentNode = LastNode;
	return LastNode->Data;
}

void * GList::Current() const
{
	if (!CurrentNode) 
	{
		return 0;
	}
	return CurrentNode->Data;
}

void * GList::Next() const
{
	if (!CurrentNode) 
	{
		return 0;
	}
	GList *pThis = (GList *)this;
	pThis->CurrentNode = CurrentNode->NextNode;
	return Current();
}

void * GList::Previous() const
{
	if (!CurrentNode) 
	{
		return 0;
	}
	GList *pThis = (GList *)this;
	pThis->CurrentNode = CurrentNode->PrevNode;
	return Current();
}

void * GList::Tail() const
{
	if (!CurrentNode) 
	{
		return 0;
	}
	return CurrentNode->NextNode->Data;
}

void GList::AddBeforeCurrent( void * Data )
{
	// get a new node - from the cache if possible
	Node *pN = (pNodeCache) ? pNodeCache->Get(CurrentNode, 0) : ::new Node(CurrentNode, 0);
	if (!pN)
	{
		throw GException("GList", 0);
	
	}

	// Add maintains list integrity by adding the new node before the current node
	// if the current node is the first node, the new node becomes the first node.
	if (FirstNode == 0)
		FirstNode = LastNode = CurrentNode = pN;
	if (FirstNode->PrevNode)
		FirstNode = FirstNode->PrevNode;
	iSize++;
	pN->Data = Data;
}

void GList::AddAfterCurrent( void * Data )
{
	// get a new node - from the cache if possible
	Node *pN  = (pNodeCache) ? pNodeCache->Get(CurrentNode, 1) : ::new Node(CurrentNode);
	if (!pN)
	{
		throw GException("GList", 0);
	
	}

	// AddLast maintains list integrity by adding the new node after the current
	// node, if the current node is the last node, the new node becomes the last node.
	if (FirstNode == 0)
		FirstNode = LastNode = CurrentNode = pN;
	if (LastNode->NextNode)
		LastNode = LastNode->NextNode;
	iSize++;
	pN->Data = Data;
}

// AddHead maintains list integrity by making the new node the first node.
void GList::AddHead( void * Data)
{
	// get a new node - from the cache if possible
	Node *pN = (pNodeCache) ? pNodeCache->Get(FirstNode, 0) : ::new Node(FirstNode, 0);
	if (!pN)
	{
		throw GException("GList", 0);
	}

	if (LastNode == 0)
		LastNode = CurrentNode = pN;
	iSize++;
	pN->Data = Data;
	FirstNode = pN;
}

// AddLast maintains list integrity by making the new node the last node.
void GList::AddLast(void * Data)
{
	// get a new node - from the cache if possible
	Node *pN  = (pNodeCache) ? pNodeCache->Get(LastNode, 1) : ::new Node(LastNode);
	if (!pN)
	{
		throw GException("GList", 0);
	
	}

	if (FirstNode == 0)
		FirstNode = CurrentNode = pN;
	iSize++;
	pN->Data = Data;
	LastNode = pN;
}

int GList::Remove(void *Data)
{
	int nRet = 0;
	void *p = First();
	while(p)
	{
		if (p == Data)
		{
			RemoveCurrent();
			nRet = 1;
			break;
		}
		p = Next();
	}
	return nRet;
}

void GList::RemoveAll()
{
	while (FirstNode)
	{
		// inline RemoveFirst()
		if (CurrentNode == FirstNode) CurrentNode = FirstNode->NextNode;
		if (LastNode == FirstNode) LastNode = 0;
		Node *Save = FirstNode->NextNode;

		// put the node in the cache or delete it
		if (pNodeCache) pNodeCache->Put(FirstNode); else ::delete FirstNode;

		FirstNode = Save;
		iSize--;
	}
}

void *GList::RemoveFirst()
{
// RemoveFirst maintains list integrity by making the first node the node
// after the first node.  If the first node is the only node in the list,
// removing it produces an empty list.
	void *ret = 0;
	if (FirstNode) 
	{
		ret = FirstNode->Data;
		if (CurrentNode == FirstNode) CurrentNode = FirstNode->NextNode;
		if (LastNode == FirstNode) LastNode = 0;
		Node *Save = FirstNode->NextNode;
		
		// put the node in the cache or delete it
		if (pNodeCache) pNodeCache->Put(FirstNode); else ::delete FirstNode;

		FirstNode = Save;
		iSize--;
	}
	return ret;
}

void * GList::RemoveLast()
{
	void *ret = 0;

	if (LastNode) 
	{
		ret = LastNode->Data;

		if (CurrentNode == LastNode) 
			CurrentNode = LastNode->PrevNode;
		if (FirstNode == LastNode) 
			FirstNode = 0;
		Node *Save = LastNode->PrevNode;

		// put the node in the cache or delete it
		if (pNodeCache) pNodeCache->Put(LastNode); else ::delete LastNode;

		LastNode = 0;

		LastNode = Save;
		iSize--;
	}

	return ret;
}

void GList::RemoveCurrent()
// RemoveCurrent maintains list integrity by making the current node
// the next node if it exists, if there is not a next node, current node
// becomes the previous node if it exists, otherwise, current = NULL.
// If the current node is also the first node, then the first node becomes
// the next node by default.  If the current node is also the last node,
// then the last node becomes the previous node by default.  If current ==
// last == first, then removing the current node produces an empty list
// and current = first = last = NULL.
{
	if (CurrentNode) 
	{
		Node *Save;
		if (!CurrentNode->PrevNode) FirstNode = CurrentNode->NextNode;
		if (CurrentNode->NextNode) Save = CurrentNode->NextNode;
		else if (CurrentNode->PrevNode) Save = LastNode = CurrentNode->PrevNode;
		else Save = FirstNode = LastNode = 0;

		// put the node in the cache or delete it
		if (pNodeCache) pNodeCache->Put(CurrentNode); else ::delete CurrentNode;

		CurrentNode = Save;
		iSize--;
	}
}

void GList::RemoveNext()
{
// RemoveNext maintains list integrity by checking if the next node is the
// last node, if this is the case, then the last node becomes the current
// node.
	if (CurrentNode) 
	{
		if (CurrentNode->NextNode) 
		{
			if (!CurrentNode->NextNode->NextNode) LastNode = CurrentNode;
			Node * DeleteNode = CurrentNode->NextNode;

			// put the node in the cache or delete it
			if (pNodeCache) pNodeCache->Put(DeleteNode); else ::delete DeleteNode;

			iSize--;
		}
	}
}

void GList::RemovePrevious()
{
// RemovePrevious maintains list integrity by checking if the next node is
// the first node, if this is the case, then the first node becomes the
// current node.
	if (CurrentNode) 
	{
		if (CurrentNode->PrevNode) 
		{
			if (!CurrentNode->PrevNode->PrevNode) FirstNode = CurrentNode;
			Node * DeleteNode = CurrentNode->PrevNode;

			// put the node in the cache or delete it
			if (pNodeCache) 
				pNodeCache->Put(DeleteNode); 
			else 
				::delete DeleteNode;

			iSize--;
		}
	}
}


GListIterator::GListIterator(const GList *iList, int IterateFirstToLast)
{
	pTList = (GList *)iList;
	iDataNode = 0;
	if (pTList && pTList->Size())
	{
		if (IterateFirstToLast) 
			iDataNode = (GList::Node *)((GList *)iList)->FirstNode;
		else 
			iDataNode = ((GList *)iList)->LastNode;
	}
}

void GListIterator::reset(int IterateFirstToLast /* = 1 */)
{
	if (IterateFirstToLast) 
		iDataNode = (GList::Node *)((GList *)pTList)->FirstNode;
	else 
		iDataNode = ((GList *)pTList)->LastNode;
}

void * GListIterator::operator ++ (int)
{
	void *pRet = iDataNode->Data;
	iCurrentNode = iDataNode;
	iDataNode = iDataNode->NextNode;
	return pRet;
}

void * GListIterator::operator -- (int)
{
	void *pRet = iDataNode->Data;
	iCurrentNode = iDataNode;
	iDataNode = iDataNode->PrevNode;
	return pRet;
}

int GListIterator::operator ()  (void) const
{
	return iDataNode != 0;
}



