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


#include "GHash.h"
#include "GException.h"
#include <string.h> // for strlen



// Here is an excellent Hash index algorithm comparison online:    
// http://www.burtleburtle.net/bob/hash/doobs.html
unsigned __int64 GHash::HashKey(const char *key) const
{

	// Currently using 64 bit FNV1 hash
    unsigned __int64 nHash = 14695981039346656037;
    while(*key)
	{
        nHash = (nHash * 1099511628211) ^ *key++;
    }
    return nHash;

}												
												
												
// number of elements							
__int64  GHash::GetCount() const						
{
	return m_nCount;
}

bool GHash::IsEmpty() const
{
	return (m_nCount == 0);
}

// use the memory address as the hash key
void *GHash::Lookup(void *key, __int64 nOccurance/* = 1*/) const
{
	union CAST_THIS_TYPE_SAFE_COMPILERS
	{
		// this union has this variable sized type 'void *' = 32 or 64 bits
		void *			 mbrVoid;
		unsigned __int64 mbrInt64;
		unsigned int	 mbrInt;
	}Member;
	Member.mbrVoid = key;

#if defined(___LINUX64) || defined(_WIN64)  || defined(___IOS)
    // The memory address is a 64 bit integer - convert to a 32 bit integer using modulo/modulus
	// as opposed to throwing out the high order bytes by using a static cast from a 64 to a 32 bit integer
	unsigned int nHashKey = Member.mbrInt64 % (unsigned int)-1;
	return Lookup( nHashKey, nOccurance);
#else
	return Lookup(Member.mbrInt, nOccurance);
#endif


}


//
__int64 GHash::GetCountByKey(const char *key) const
{
	if (!m_table)
	{
		// throw an exception - could not allocate hash table
		throw GException("Hash", 0);
	}

	unsigned __int64 nHashKey = HashKey(key);
	unsigned __int64 nHash = nHashKey % m_nHashTableSize;


	GBTree &rAssoc = m_table[nHash];
	return rAssoc.getOccurCount(key);
}



// Lookup
void *GHash::Lookup(const char *key, __int64 nOccurance/* = 1*/) const
{
	if (!m_table)
	{
		// throw an exception - could not allocate hash table
		throw GException("Hash", 0);
	}

	unsigned __int64 nHashKey = HashKey(key);
	unsigned __int64 nHash = nHashKey % m_nHashTableSize;


	GBTree &rAssoc = m_table[nHash];
	return rAssoc.search(key,nOccurance);
}

void *GHash::Lookup(unsigned __int64 nKey, __int64 nOccurance/* = 1*/) const
{
	if (!m_table)
	{
		// throw an exception - could not allocate hash table
		throw GException("Hash", 0);
	}

	unsigned __int64 nHash = nKey % m_nHashTableSize;
	GBTree &rAssoc = m_table[nHash];
	
	GString strKey;
	strKey << nKey;
	return rAssoc.search(strKey,nOccurance);
}

void *GHash::operator[](const char *key) const
{
	return Lookup(key);
}


void GHash::Insert(void *key, void *value)
{
	union CAST_THIS_TYPE_SAFE_COMPILERS
	{
		void *       mbrVoid;
		unsigned __int64 mbrInt64;
//		unsigned int mbrInt;
	}Member;
	Member.mbrInt64 = 0;  // zero all 64 bits
	Member.mbrVoid = key; // can be 32 or 64 bits

	Insert(Member.mbrInt64, value);

/*
#if defined(___LINUX64) || defined(_WIN64) || defined(___IOS)
	unsigned int nHashKey = Member.mbrInt64 % (unsigned int)-1;
	Insert(nHashKey, value);
#else
	Insert(Member.mbrInt, value);
#endif
*/

}


// add a new (key, value) pair
void GHash::Insert(const char *key, void *value)
{
	if (!m_table)
	{
		// throw an exception - could not allocate hash table
		throw GException("Hash", 0);
	}

	unsigned __int64 nHashKey = HashKey(key);
	unsigned __int64 nHash = nHashKey % m_nHashTableSize;

	GBTree &rAssoc = m_table[nHash];
	rAssoc.insert(key, value);

	m_nCount++;
}



// add a new (key, value) pair
void GHash::Insert(unsigned __int64 nHashKey, void *value)
{
	if (!m_table)
	{
		// throw an exception - could not allocate hash table
		throw GException("Hash", 0);
	}
	unsigned __int64 nHash = nHashKey % m_nHashTableSize;

	GBTree &rAssoc = m_table[nHash];
	GString strKey;
	strKey << nHashKey;
	rAssoc.insert(strKey, value);

	m_nCount++;
}



void GHash::reassignKeyPair(const char*key, void *pOld, void*pNew)
{
	if (!m_table)
	{
		// throw an exception - could not allocate hash table
		throw GException("Hash", 0);
	}
	unsigned __int64 nHashKey = HashKey(key);
	unsigned __int64 nHash = nHashKey % m_nHashTableSize;

	GBTree &rAssoc = m_table[nHash];

	rAssoc.reassignKeyPair(key, pOld, pNew);
}


// removing existing (key, ?) pair
void *GHash::RemoveKey(const char *key, __int64 nOccurance/* = 0*/)
{
	if (!m_table)
	{
		// throw an exception - could not allocate hash table
		throw GException("Hash", 0);
	}

	unsigned __int64 nHashKey = HashKey(key);
	unsigned __int64 nHash = nHashKey % m_nHashTableSize;

	GBTree &rAssoc = m_table[nHash];

	if (nOccurance == 0) // remove them all, return the 1st of them if there was 1 or >
	{
		void *pRet = rAssoc.search(key);
		void *pCur = pRet;
		while (pCur)
		{
			pCur = rAssoc.removeGet(key);
			if (pCur)
				m_nCount--;
		}
		return pRet;
	}
	else // remove exactly what is specified
	{
		return rAssoc.removeGet(key, nOccurance);
	}
}


void *GHash::RemoveKey(void *key, __int64 nOccurance/* = 0*/)
{
	union CAST_THIS_TYPE_SAFE_COMPILERS
	{
		void *       mbrVoid;
		unsigned __int64 mbrInt64;
		unsigned int mbrInt;
	}Member;
	Member.mbrVoid = key;
#if defined(___LINUX64) || defined(_WIN64)  || defined(___IOS)
    // The memory address is a 64 bit integer - convert to a 32 bit integer using modulo/modulus
	// as opposed to throwing out the high order bytes by using a static cast from a 64 to a 32 bit integer
	unsigned int nHashKey = Member.mbrInt64 % (unsigned int)-1;
	return RemoveKey(nHashKey, nOccurance);
#else
	return RemoveKey(Member.mbrInt, nOccurance);
#endif

}

void *GHash::RemoveKey(unsigned int nHashKey, __int64 nOccurance/* = 0*/)
{
	if (!m_table)
	{
		// throw an exception - could not allocate hash table
		throw GException("Hash", 0);
	}

	unsigned int nHash = nHashKey % m_nHashTableSize;

	GBTree &rAssoc = m_table[nHash];
	GString strKey;
	strKey << nHashKey;
	void *pRet = rAssoc.search(strKey);
	if (pRet)
	{
		rAssoc.remove(strKey);
		m_nCount--;
	}
	return pRet;
}

void GHash::InitTable()
{
	if (m_table)
		delete [] m_table;
	m_table = new GBTree[m_nHashTableSize];
	m_nCount = 0;
}

void GHash::RemoveAll()
{
	InitTable();
}

GHash::GHash(unsigned int nPrime /* = 2503 */) :
	m_nHashTableSize(nPrime),
	m_table(0),
	m_nDeferDestruction(0),
	m_nCount(0)
{
	InitTable();
}

GHash::~GHash()
{
	if (!m_nDeferDestruction)
	{
		delete [] m_table;
		m_table = 0;
	}

}

void GHash::DeferDestruction()
{
	m_nDeferDestruction = 1;
}

void GHash::Destruction()
{
	if (m_table)
	{
		delete [] m_table;
		m_table = 0;
	}

}

GHashIterator::GHashIterator(const GHash *hash) :
	m_hash(*hash),
	m_nHashIndex(0),
	m_pLookAhead(0),
	m_itOK(0)
{
}


void *GHashIterator::Increment()
{
	if (m_pLookAhead)
	{
		void *pRet = m_pLookAhead;
		m_pLookAhead = 0;
		return pRet;
	}

	// return the next in the tree at this hash index
	if ( m_itOK && m_it() )
		return m_it++;

	// we're at the end of this tree's iterator
	m_itOK = 0;

	// look at the next hash index
	if (m_hash.m_table)
	{
		for (; m_nHashIndex < m_hash.m_nHashTableSize;)
		{
			if (m_hash.m_table[m_nHashIndex].getNodeCount())
			{
				// move to the beginning of the next tree's iterator
				m_it.Reset(&m_hash.m_table[m_nHashIndex], 2);
				m_itOK = 1;
				m_nHashIndex++;
				return m_it++;
			}
			m_nHashIndex++;
		}
	}
	return 0;
}

int GHashIterator::operator ()(void)
{
	if (m_pLookAhead)
		return 1;
	else
		m_pLookAhead = (*this)++;
	return (m_pLookAhead == 0) ? 0 : 1;
}

void * GHashIterator::operator ++ (int)
{
	void *pRet = Increment();
	while(pRet == (void *)-1)
		Increment();
	return pRet;
}

__int64 GHash::BulkInsertFromFile(const char *pzDelimiter, const char* pzFileName, void* pValue )
{
	GString g;
	if (g.FromFile(pzFileName, 0))
	{
		return BulkInsert(pzDelimiter, g._str, pValue);
	}
	return 0;
}

__int64 GHash::BulkInsert(const char *pzDelimiter, const char* pzKeyList, void* pValue)
{
	if (!pzDelimiter || !pzDelimiter[0] || !pzKeyList || !pzKeyList[0])
		return 0;

	__int64 nAdded = 0;
	char* pzBegin = (char*)pzKeyList;
	size_t nDelimiterLen = strlen(pzDelimiter);

	// find first delimiter
	char* found = (char*)strstr(pzBegin, pzDelimiter);

	while (1)
	{
		if (!found)
		{
			// there is only one entry in the list 
			Insert(pzBegin, pValue);
			nAdded++;
			break;
		}
		char chSave = *found;

		// add this entry
		*found = 0; // terminate the string at the delimiter
		Insert(pzBegin, pValue);
		*found = chSave;

		nAdded++;

		// advance to the next string
		pzBegin = &found[nDelimiterLen];

		// advance to the next delimiter, break if none
		found = (char*)strstr(pzBegin, pzDelimiter);
		if (!found)
		{
			Insert(pzBegin, (void*)1L);
			nAdded++;
			break;
		}
	}
	return nAdded;
}

