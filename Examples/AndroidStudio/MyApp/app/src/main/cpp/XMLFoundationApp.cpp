#include "XMLFoundationApp.h"   // This custom application class header
#include "XMLFoundation.h"   

// This empty XMLFoundationApp can implement anything from any of the C++ example programs.  ExIndexObjects shows off the performance aspects in the XMLFoundation
// I duplicated the "ContainOrInherit" example program here inside this source file to show the application layer object design for handling XML.
#include "ExampleObjectModeling.h"




namespace XMLFoundationNative
{
	// IMPLEMENT_FACTORY() should exist in a .CPP file - not an .h file - one for every DECLARE_FACTORY() in the .h file
	IMPLEMENT_FACTORY(XMLFoundationApp, ObjResultSet);
	IMPLEMENT_FACTORY(MyCustomObject, Customer);
	IMPLEMENT_FACTORY(AddressObject, Address);

	void MyCustomObject::MapXMLTagsToMembers()
	{
		MapMember(&_CustID, "ID");

		// for the next 2 MapMember() [_strList and _AddressList] the 3rd argument is optional to denote an outer XML Element grouping 
		MapMember(&_strList, "Item", "ItemListName");
		MapMember(&_AddressList, AddressObject::GetStaticTag(), "AddressList");

		MapMember(&_strName, "String");
		MapAttribute(&_strColor, "Color");

		MapObjectID("ID", 1);
	}
	void AddressObject::MapXMLTagsToMembers()
	{
		MapMember(&_strLine1, "Line1");
		MapMember(&_strLine2, "Line2");
		MapMember(&_strCity, "City");
		MapMember(&_strState, "State");
		MapMember(&_strZip, "Zip");
		MapMember(&_nType, "Type");            // only 1 instance of each type is allowed in this data model
		MapAttribute(&_nCustID, "CustID");
		MapObjectID("CustID", 0, "Type", 1);  // CustID + Type defines a unique key.  The 0 after CustID indicates that CustID an XML Attribute, the 1 after Type indicates that Type is an XML Element
	}


	void XMLFoundationApp::MapXMLTagsToMembers()
	{
		MapMember(&_appObjs, MyCustomObject::GetStaticTag());
	}




	XMLFoundationApp::XMLFoundationApp() :  _appObjs(19) // init a GHash table of 19 b-trees, for large datasets increase the initial prime size
	{
		_appData = new GProfile();

		// this is how to auto-load object instances
//		InsertOrUpdateCustomer(777, "Root", "Ultraviolet", "aaa|bbb|ccc");
//		InsertOrUpdateAddress (777, "21 Park Place", "Boardwalk Building", "UniverseCity", "Stateless", "90210", 1);
	}

	XMLFoundationApp::~XMLFoundationApp()
	{
	}

	
	// insert or update an [AddressObject]
	int XMLFoundationApp::InsertOrUpdateAddress(unsigned __int64 nCustID, const char* strLine1, const char* strLine2, const char* strCity, const char* strState, const char* strZip, int nAddrType)
	{
		GString strKey;
		strKey << nCustID; // convert the numeric key to an alpha numeric string

		MyCustomObject* pCO = (MyCustomObject*)_appObjs.Lookup(strKey);
		if (!pCO)
		{
			m_strLastAppErr = "Customer [";
			m_strLastAppErr << nCustID << "] not found.  An Address must relate to a known Customer.";
			return -1;
		}

		bool bNewAddrObj = false;
		AddressObject* pAO = 0;
		GListIterator it( &pCO->_AddressList );
		while ( it() )
		{
			AddressObject* p = (AddressObject*)it++;
			if (p->_nType == nAddrType)
			{
				pAO = p;  // update the existing object
			}
		}
		if (!pAO) // true on insert, false on update
		{
			pAO = new AddressObject(); // create the new address object
			bNewAddrObj = true;  // this is an insert of a new Address Object
		}

		pAO->_strLine1 = strLine1;
		pAO->_strLine2 = strLine2;
		pAO->_strCity = strCity;
		pAO->_strState = strState;
		pAO->_strZip = strZip;
		pAO->_nType = nAddrType;
		pAO->_nCustID = nCustID;

		if (bNewAddrObj)
			pCO->_AddressList.AddLast(pAO); // only add to the list for Insert, not for Update

	}
	

	int XMLFoundationApp::InsertOrUpdateCustomer(unsigned __int64 nID, const char* pzName, const char* pzColor, const char* strListItems)
	{
		GString strKey;
		strKey << nID; // convert the numeric key to an alpha numeric string

		MyCustomObject* pCO = (MyCustomObject*)_appObjs.Lookup(strKey);
		bool bNewCustObj = (!pCO); // bNewCustObj = true on insert, false on update

		if (bNewCustObj)
			pCO = new MyCustomObject();

		pCO->_CustID = nID;
		pCO->_strName = pzName;
		pCO->_strColor = pzColor;
		pCO->_strList.RemoveAll();
		pCO->_strList.DeSerialize("|", strListItems); 

		if (bNewCustObj)
		{
			// Here we put a manually created object instance into the cache so that FromXML() can make make updates to it
			// calling getOID() here is important.  This sets the XMLObject base class OID internally from the current object data state.
			// so although [strKey] has the same value, pCO->getOID() is the way to get it.  pCO->getOID() builds the oid, from keyparts if necessary.  type AddressObject uses keyparts - this type MyCustomObject does not.
			cacheManager.enterObject(pCO->getOID(), pCO->GetVirtualType(), pCO);
			_appObjs.Insert(strKey, pCO);

			// additionally this instance[pCO] will exist for the duration of this entire application unless we at some point manually delete it
		}

		return 0;
	}


	int XMLFoundationApp::FromXML(const char* pzXML)
	{
		return XMLObject::FromXMLX(pzXML, &m_strLastAppErr);
	}

	// This is not used, rather a single global DeleteResultsetHandle found in DLLExports.cpp
	void XMLFoundationApp::DeleteResultsetHandle2(XMLResultset* pResultset)
	{
		if (pResultset && pResultset != g_pEmptyXMLResultSet)
			delete pResultset;
	}

	XMLResultset* XMLFoundationApp::ToXML(char* pResultsBuf, __int64* pBufLength, int nTabs/* = 0*/, const char* TagOverride/* = 0*/, unsigned int nSerializeFlags/* = FULL_SERIALIZE*/, const char* pzSubsetOfObjectByTagName/* = 0*/, bool bAppend/* = 0*/)
	{
		// this XMLResultset is a GString that uses the (callers memory space)[pResultsBuf] and makes no allocation unless the data becomes larger 
		// than [*pBufLength] in which case it will then perform an allocation to memory in (our memory space) with room to grow and "own" the new block 
		// of memory unlike  the initial block that this ::GString [bOwns=false] so it leaves memory cleanup to the owner and ~GString dtor and will not delete it.
		XMLResultset*pResultSet = new XMLResultset(pResultsBuf, (bAppend) ? strlen(pResultsBuf) : 0, *pBufLength);
		
		XMLObject::ToXML(pResultSet, nTabs, TagOverride, nSerializeFlags, 0, pzSubsetOfObjectByTagName);

		if (pResultSet->Length() > *pBufLength) // the results are larger than the working memory space [pResultsBuf]
		{
			*pBufLength = pResultSet->Length();// the caller should know to get more data from the resultset because *pBufLength, the total bytes of result, was > sizeof(pResultsBuf)
			//  this can also be handled with a callback
			//  or handed as-is returned to the caller with an indexable resultset that the application layer can determine when to destroy.
			return pResultSet; // success - with more data. The caller MUST call DeleteResultsetHandle() when finished.  The first [sizeof(pResultsBuf)] bytes are already returned in [pResultsBuf] as the GString grew allocation space the first working block with the start of the results detached from thje GString
		}
		else // the entire resultset fit into the callers memory space, the entire result set is already returned
		{	 // let the caller know the length of the results.
			*pBufLength = pResultSet->Length();
			delete pResultSet; // deletes only the attached class - leaves the memory alone in destruction 
			return g_pEmptyXMLResultSet; // success.  The caller can call DeleteResultsetHandle() when finished, however the call is ignored for this special resultset [g_pEmptyXMLResultSet]
		}

	}

	int XMLFoundationApp::SetAppValue(const char* pzSection, const char* pzEntry, const char *pzValue)
	{
		// The section/entry will be created when it does not already exist
		// The existing value will be repalced with the supplied value when it does exist
		_appData->SetConfig(pzSection, pzEntry, pzValue);
		return 0;
	}
	const char* XMLFoundationApp::GetAppValue(const char* pzSection, const char* pzEntry)
	{
		// retrieves a character string value from the specified section
		return _appData->GetString(pzSection, pzEntry, false); // will not throw exceptions
	}

	int XMLFoundationApp::GetAppValue(const char* pzSection, const char* pzEntry, char* sValueDest,__int64 *nBufSize)
	{
		// retrieves a character string value from the specified section
		const char*pzReturn = _appData->GetString(pzSection, pzEntry, false); // will not throw exceptions
		if (pzReturn && pzReturn[0])
		{
			// this will truncate the returned value if the supplied results buffer is too small .
			int nReturnLen = strlen(pzReturn);
			memcpy((void*)sValueDest, pzReturn, ((nReturnLen + 1) > *nBufSize) ? *nBufSize - 1 : nReturnLen + 1);
			*nBufSize = ((nReturnLen + 1) > *nBufSize) ? *nBufSize - 1 : nReturnLen;
			return 0; // success
		}
		else
		{
			m_strLastAppErr.Empty();
			m_strLastAppErr << "Section [" << pzSection << "] Entry [" << pzEntry << "] not found in XMLFoundationApp::GetAppValue()";
			return - 1; // not found
		}
	}

	XMLResultset* XMLFoundationApp::GetAllAppValues(char* pResultsBuf, __int64* pBufLength, bool bAsXML/* = 0*/, const char* pzSection/* = 0*/)
	{
		try
		{
			// this XMLResultset is a GString that uses the (callers memory space)[pResultsBuf] and makes no allocation unless the data becomes larger 
			// than [*pBufLength] in which case it will then perform an allocation to memory in (our memory space) with room to grow and "own" the new block 
			// of memory unlike  the initial block that this ::GString [bOwns=false] so it leaves memory cleanup to the owner and ~GString dtor and will not delete it.
			XMLResultset* pResultSet = new XMLResultset(pResultsBuf, 0, *pBufLength);
			__int64 nLength = _appData->WriteCurrentConfig(pResultSet, bAsXML);

			if (pResultSet->Length() > *pBufLength)
			{
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
			m_strLastAppErr = e.GetDescription();
			return nullptr;
		}
		catch (...)
		{
			m_strLastAppErr = "Runtime exception in XMLFoundationApp::GetAllAppValues()";
			return nullptr;
		}

	}
	
	// in this case, pnDataLength is the length of the data NOT the size of the buffer
	int XMLFoundationApp::SetAllAppValues(char* pResultsBuf, __int64* pnDataLength, bool bAsXML/* = 0*/)
	{
		try 
		{
			GProfile* pGPro = new GProfile(pResultsBuf, *pnDataLength, bAsXML);
			if (pGPro)
			{
				delete _appData;
				_appData = pGPro;
				return 0; // success
			}
			else
			{
				return -1; // out of memory
			}
		}
		catch (GException& e)
		{
			m_strLastAppErr = e.GetDescription();
			return -2;
		}
		catch (...)
		{
			m_strLastAppErr = "Runtime exception in XMLFoundationApp::SetAllAppValues()";
			return -3;
		}
	}




	int XMLFoundationApp::Example(int nType)
	{
		// nType == 1 - return an object Dump
		if (nType == 1)
		{
			m_strExampleOutput = "";
			Dump(m_strExampleOutput);
			return 0;
		}

		// nType == 1 - return an object Dump
		if (nType == 2)
		{
			m_strExampleOutput = "Put any value here to test how XMLFoundation works";
			return 0;
		}


		// --------------------------------------

		// This empty XMLFoundationApp can implement anything from any of the C++ example programs.  ExIndexObjects shows off the performance aspects in the XMLFoundation
		// I duplicated the "ContainOrInherit" example program here inside this source file to show the application layer object design for handling XML.


		m_strExampleOutput = "For ANY of this output to make sense - you would need to step through the code with a debugger - and in the process see how Mapped Object inheritance works and realize that all the XMLFoundation C++ examples apply to a C# app\n";

		// set the global to trace object constructor calls
		g_pstrOutput = &m_strExampleOutput;

		try
		{
			// take your pick, Design A==B==C
			// 
			MyExistingObjectA A;
			MyExistingObjectB B;
			MyExistingObjectC C;
			A.ReadDataFormat(pzXML);
			B.ReadDataFormat(pzXML);
			C.ReadDataFormat(pzXML);

			GString a;
			GString b;
			GString c;


			m_strExampleOutput << A.WriteDataFormat(&a);
			m_strExampleOutput << "\r\n ---------- \r\n";
			m_strExampleOutput << B.WriteDataFormat(&b);
			m_strExampleOutput << "\r\n ---------- \r\n";
			m_strExampleOutput << C.WriteDataFormat(&c);


			// as we saw above, there is a time to contain(or inherit)
			// and as you can see below there is a time to inherit only inherit

			MappingAndInheritance(&m_strExampleOutput);

			// puts two different object types into the same list
			AdvancedInheritMapping(&m_strExampleOutput);
		}
		catch (GException& e)
		{
			m_strLastAppErr = e.GetDescription();
			return -1;
		}
		catch (...)
		{
			m_strLastAppErr = "Runtime exception in XMLFoundationApp::Example";
			return -2;
		}
		return 0; // success
	}
}
