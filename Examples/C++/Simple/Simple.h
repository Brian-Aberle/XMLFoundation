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


class XMLFoundationApp : public XMLObject
{
	void MapXMLTagsToMembers(); // only 1 map : MyCustomObject's into the indexed GHash _appObjs
public:
	GHash _appObjs; // contains MyCustomObject instances
	DECLARE_FACTORY(XMLFoundationApp, ObjResultSet);

public:
	GString m_strLastAppErr;
	XMLFoundationApp() {};
	~XMLFoundationApp() {};

	GString m_strExampleOutput;
//	int Example();

	// insert or update a [MyCustomObject]
	int InsertOrUpdateCustomer(unsigned __int64 nID, const char* pzName, const char* pzColor, const char* strListItems);
	// insert or update an [AddressObject]
	int InsertOrUpdateAddress(unsigned __int64 nCustID, const char* strLine1, const char* strLine2, const char* strCity, const char* strState, const char* strZip, int nAddrType);

};
