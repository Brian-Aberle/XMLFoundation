#pragma once

#include "XMLObject.h"	
#include "GProfile.h"	


namespace XMLFoundationNative
{

    class XMLFoundationApp : public XMLObject
    {
		GProfile *_appData; // contains only user defined application settings globally accessable to this app 
        GHash _appObjs;		// contains MyCustomObject instances - because the XMLObjectFactory will be adding objects using a string representation of the OID as opposed to a numeric key, any objects added by this application must also use the string representation of the OID as a key when adding or searching for objects in the GHash.
		void MapXMLTagsToMembers(); // only 1 map : MyCustomObject's into the indexed GHash _appObjs

	public:
		DECLARE_FACTORY(XMLFoundationApp, ObjResultSet);

    public:
        GString m_strLastAppErr;
        XMLFoundationApp();
        ~XMLFoundationApp();

        GString m_strExampleOutput;
        int Example(int nType);

        int SetAppValue(const char* pzSection, const char *pzEntry, const char *pzValue);
		int GetAppValue(const char* pzSection, const char* pzEntry, char* pResultsBuf, __int64 *pBufLength);
		const char* GetAppValue(const char* pzSection, const char* pzEntry);
		XMLResultset* GetAllAppValues(char* pResultsBuf, __int64* pBufLength, bool bAsXML = 0, const char* pzSectionOnly = 0 );
		int SetAllAppValues(char* pResultsBuf, __int64* pBufLength, bool bAsXML = 0);

//		void DeleteResultsetHandle(GString* p);

	    // insert or update a [MyCustomObject]
		int InsertOrUpdateCustomer(unsigned __int64 nID, const char* pzName, const char* pzColor, const char* strListItems);
		// insert or update an [AddressObject]
		int InsertOrUpdateAddress(unsigned __int64 nCustID, const char* strLine1, const char* strLine2, const char* strCity, const char* strState, const char* strZip, int nAddrType);

		
		// add (or update on matching OID's) any number of new[MyCustomObject]'s, with any number of [AddressObject]'s from the supplied XML into [_appObjs]
		// returns 1 on success, 0 for failure
		int FromXML(const char* pzXML);
		XMLResultset* ToXML(char* pResultsBuf, __int64* pBufLength, int nTabs = 0, const char* TagOverride = 0, unsigned int nSerializeFlags = FULL_SERIALIZE, const char* pzSubsetOfObjectByTagName = 0, bool bAppend = 0);
//		GString* ToXML(char* pResultsBuf, __int64* pBufLength, int nTabs = 0, const char* TagOverride = 0, unsigned int nSerializeFlags = FULL_SERIALIZE, const char* pzSubsetOfObjectByTagName = 0, bool bAppend = 0);
		void DeleteResultsetHandle2(XMLResultset* pResultset);
//
    };


	// ----------------------------------------------------------------------------------------------
	class AddressObject : public XMLObject
	{
	public: // public for brievity of the example - MapMember() will map to private members
		GString _strLine1;
		GString _strLine2;
		GString _strCity;
		GString _strState;
		GString _strZip;
		int _nType; // 1 = home address, 2= work address, 3 = billing address, 4 = shipping address
		long long _nCustID; // the owning Objects unique index key

		virtual void MapXMLTagsToMembers(); // implemented in XMLFoundationApp.cpp
		// 'this' type, followed by the XML Element name, normally DECLARE_FACTORY() is in an .h file
		DECLARE_FACTORY(AddressObject, Address)
		AddressObject() { _nType = 0; }
		~AddressObject() {}
	};

	class MyCustomObject : public XMLObject
	{
	public: // public for brievity of the example - MapMember will map to private members
		long long _CustID;			// a custom id, a unique index key for this object
		GString _strName;			// An XML Element
		GString _strColor;			// An XML Attribute because MapAttribute() was used in MapXMLTagsToMembers()
		GStringList _strList;      // A String List
		GList _AddressList;		// a list of Addresses associated to this object

		virtual void MapXMLTagsToMembers(); // implemented in XMLFoundationApp.cpp
		// 'this' type, followed by the XML Element name, normally DECLARE_FACTORY() is in an .h file
		DECLARE_FACTORY(MyCustomObject, Customer);

		// keep one constructor with no arguments
		MyCustomObject() { _CustID = 0; }
		~MyCustomObject() {};
	};



}
