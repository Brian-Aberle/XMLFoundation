#include <XMLFoundation.h>

#include "Simple.h"


IMPLEMENT_FACTORY(MyCustomObject, Customer);
IMPLEMENT_FACTORY(AddressObject, Address);
IMPLEMENT_FACTORY(XMLFoundationApp, ObjResultSet);


void MyCustomObject::MapXMLTagsToMembers()
{
	// for the first 2 MapMember() calls TO [_strList] and [&_AddressList] the 3rd argument is optional to denote an outer XML Element grouping 
	MapMember(&_strList, "Item", "ItemListName");
	MapMember(&_AddressList, AddressObject::GetStaticTag(), "AddressList");

	MapMember(&_strName, "String");
	MapMember(&_CustID, "ID");
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

// insert or update an [AddressObject]
int XMLFoundationApp::InsertOrUpdateAddress(unsigned __int64 nCustID, const char* strLine1, const char* strLine2, const char* strCity, const char* strState, const char* strZip, int nAddrType)
{
	MyCustomObject* pCO = (MyCustomObject*)_appObjs.Lookup(nCustID);
	if (!pCO)
	{
		m_strLastAppErr = "Customer [";
		m_strLastAppErr << nCustID << "] not found.  An Address must relate to a known Customer.";
		return -1;
	}

	bool bNewAddrObj = false;
	AddressObject* pAO = 0;
	GListIterator it(&pCO->_AddressList);
	while (it())
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
	strKey << nID;
	
	MyCustomObject* pCO = (MyCustomObject*)_appObjs.Lookup(strKey);
	bool bNewCustObj = (!pCO); // bNewCustObj = true on insert, false on update

	if (bNewCustObj)
		pCO = new MyCustomObject();  // this object remains in the cache at application exit.  If we manually ::new() the object we must manually ::delete() it

	if (pzName)
		pCO->_strName = pzName;
	if (pzColor)
		pCO->_strColor = pzColor;
	pCO->_strList.RemoveAll();
	pCO->_strList.DeSerialize("|", strListItems);
	
	if (bNewCustObj)
	{
		pCO->_CustID = nID; // OID's never change.  A changed OID indicates a different object instance.

		// Here we put a manually created object instance into the cache so that FromXML() can make make updates to it
		// calling getOID() here is important.  This sets the XMLObject base class OID internally from the current object data state.
		// so although [strKey] has the same value, pCO->getOID() is the way to get it.  pCO->getOID() builds the oid, from keyparts if necessary.
		cacheManager.enterObject(pCO->getOID(), pCO->GetVirtualType(), pCO);
		_appObjs.Insert(strKey, pCO);
	}



	return 0;
}



int main(int argc, char* argv[])
{

	// If any tags in the FromXML() do not match up with the object mapping the warning will be in this debug file.
	SetLogFile("c:\\XMLFoundation\\MyAppDebugLog.txt");


	try
	{

		// 1st: Object Creation --> App Logic Layer -or- XMLObjectFactory  == either way the identical operation happens
		bool bUseAppLogic = 0;

		// if [bUseAppLogic] is true, the object created by InsertOrUpdateCustomer() is still in the cache after ~XMLFoundation() dtor is called  - manually ::new()'d must be manually ::deleted()
		// if [bUseAppLogic] is false, all objects were destroyed before application exit 

		XMLFoundationApp* pApp = new XMLFoundationApp;
		GString sXML("<ObjResultSet><Customer Color=\"Ultraviolet\"><ItemListName><Item>aaa</Item><Item>bbb</Item><Item>ccc</Item></ItemListName><String>Root</String><ID>777</ID></Customer></ObjResultSet>");
		if (bUseAppLogic)
			pApp->InsertOrUpdateCustomer(777, "Root", "Ultraviolet", "aaa|bbb|ccc");
		else
			pApp->FromXMLX(sXML); // FromXMLX() wont throw exceptions, FromXML() will.
		MyCustomObject* pCO = (MyCustomObject*)pApp->_appObjs.Lookup("777");
		if (pCO->GetRefCount() != 1)
			printf("This line will never print - something went wrong with ref counting if it did");
		printf(pApp->ToXML());
		printf("\r\n\r\n\r\n");

		GString strDest;
		cacheManager.getMappedValue("777MyCustomObject", "Color", &strDest, 0);


		// The Update will change everything except for the Color so we can see that some data set with the Insert above remains.
		// 
		// 2nd: Object Update --> App Logic Layer -or- XMLObjectFactory  == either way the identical operation happens
		bool bUpdateUseAppLogic = !bUseAppLogic; // default to the opposite as above to demonstrate how the two object management models coexist
		// you can manually set bUpdateUseAppLogic to either 1 or 0 and the identical Update occurs

		// an XML based update of the Object instance using a "Late Bound OID" assembled from the Object Data to update the existing object instance 
		sXML = "<ObjResultSet><Customer><ItemListName><Item>33rd</Item></ItemListName><String>Root!!!!!!!!!!!!</String><ID>777</ID></Customer></ObjResultSet>";
		if (bUpdateUseAppLogic)
			pApp->InsertOrUpdateCustomer(777, "Root!!!!!!!!!!!", 0, "33rd");
		else
			pApp->FromXMLX(sXML); // FromXMLX() wont throw exceptions, FromXML() will.
		printf("\r\n\r\n\r\n");
		printf(pApp->ToXML());
		printf("\r\n\r\n\r\n");

		delete pApp;



	}
	catch( GException &rErr )
	{
		// you probably have invalid XML if you get here...
		printf( rErr.GetDescription() );
	}

	return 0;
}


