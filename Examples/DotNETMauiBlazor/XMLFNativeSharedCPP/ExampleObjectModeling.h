#include "XMLFoundation.h"


#ifndef _OBJ_MODEL_EXAMPLE_H__
#define _OBJ_MODEL_EXAMPLE_H__
// This code builds on Android, Linux, and iOS as well as Windows

//
// Suppose you have an application that currently outputs a data format.  
// This could be ANY data format, the data format for a complex word processing application
// might be RTF, and a data format for a transactional application might be ACH or EDI.
// The data format for GProfile was INI until March 2014 when the same (simple) object model
// was modified to include XML as a native input/output format. The "data format"  (which ever)
// is generated from Application layer objects which must me modeled in memory often referred to 
// as the object model, which is an instance of the data model.

// "Containment Design"  vs "Inherited Design"

//
// This is the object model: [name,value] pairs that exist in a list, each list has a name.
// This is INI, it is also a sub-data structure concept used in EDI, RTF, and many formats
// ***********************************************************************************
class INIEntry : public XMLObject
{
public:
	GString m_strName;
	GString m_strValue;
	virtual void MapXMLTagsToMembers()
	{
		MapAttribute(&m_strName, "name");
		MapAttribute(&m_strValue, "value");
	}
	DECLARE_FACTORY(INIEntry, setting)
};

class INISection : public XMLObject
{
public:
	GString		m_strName;
	GList		m_lstNVP;
	virtual void MapXMLTagsToMembers()
	{
		MapAttribute(&m_strName, "name");
		MapMember(&m_lstNVP, GProfileEntry::GetStaticTag());
	}
	DECLARE_FACTORY(INISection, section)
};
// ***********************************************************************************


// Now suppose that [MyExistingObject] represents some class that you have already developed
// and you want to integrate a native XML format in addition to any pre-existing formats - 
// this is the path to systems integrations.  A, B, and C are 3 designs that acconplish this.
// choosing one over the other is largely a matter of preference.  I selected design "C"
// over "B" in GProfile ONLY because even when privately inherited from XMLObject my C++
// development IDE lists all the members/methods of XMLObject in GProfile, so for example
// when I type "GetProfile." the IDE shows a list of methods in GProfile and ONLY because
// I don't want to see XMLObject base class members/methods in my GProfile, I switched from "B" to "C"



///////////////   MyObject.h   ///////////////////////////////
class MyExistingObjectA  
{
public:
	GString m_strString;
	GList m_lstSections;
	const char* WriteDataFormat(GString* pstrDest);
	void ReadDataFormat(const char* pzData);
};


////////////////////////////////////////////////////////////
///////////////   MyObject.h   ///////////////////////////////
class MyExistingObjectB : private XMLObject   // this object now 'is' an XMLObject 
{
public:
	GString m_strString;
	GList m_lstSections;
	const char* WriteDataFormat(GString* pstrDest);
	void ReadDataFormat(const char* pzData);
	MyExistingObjectB() {}
	~MyExistingObjectB() {};

	// Note: this needed to be added to MyExistingObject to use direct inheritance
	virtual void MapXMLTagsToMembers()
	{
		MapMember(&m_lstSections, GProfileSection::GetStaticTag());
	}
	DECLARE_FACTORY(MyExistingObjectB, MyObject)
};

////////////////////////////////////////////////////////////
class MyExistingObjectC
{
public:
	XMLRelationshipWrapper m_objectContainer; // added to our existing .h file
	GString m_strString;
	GList m_lstSections;
	const char* WriteDataFormat(GString* pstrDest);
	void ReadDataFormat(const char* pzData);
	MyExistingObjectC() : m_objectContainer("configuration")
	{
		m_objectContainer.AddReference(GProfileSection::GetStaticTag(), &m_lstSections, &gGListHandler, 0, 1);
	}
	~MyExistingObjectC() {};
};



// ******************************************************************
// ******************************************************************
// ******************************************************************
extern GString* g_pstrOutput;  
//extern const char* pzXML;
extern const char pzXML[];
class CMatter : public XMLObject
{
public:
	GString m_strWeight;
	virtual void MapXMLTagsToMembers()
	{
		MapMember(&m_strWeight, "Weight");
	}
	DECLARE_FACTORY(CMatter, Matter)
	CMatter() {}
	~CMatter() {};
};
//------------------------------------------------
class CLife : public CMatter
{
public:
	GString m_strDNA;
	virtual void MapXMLTagsToMembers()

	{
		MapMember(&m_strDNA, "DNA");
		CMatter::MapXMLTagsToMembers(); // explicit base class call
	}

	DECLARE_FACTORY(CLife, Life)
	CLife() { if (g_pstrOutput) *g_pstrOutput << "new Clife\r\n"; }
	~CLife() {  };
};
//------------------------------------------------
//
class CHuman : public CLife
{
public:
	GString m_strFingerPrint;
	GString m_strGender;

	virtual void MapXMLTagsToMembers()
	{
		MapMember(&m_strFingerPrint, "FingerPrint");
		MapMember(&m_strGender, "Gender");
		CLife::MapXMLTagsToMembers(); // explicit base class call
	}

	DECLARE_FACTORY(CHuman, Human)

	CHuman() { if (g_pstrOutput) *g_pstrOutput << "new Human\r\n"; }
	~CHuman() {};
};
//------------------------------------------------
class ModelContainer : public XMLObject
{
	virtual void MapXMLTagsToMembers();
public:
	GList	m_lstMaterials;
	ModelContainer() {};
	~ModelContainer() {};
	DECLARE_FACTORY(ModelContainer, Model)
};




void AdvancedInheritMapping(GString *strDest);



void MappingAndInheritance(GString *strDest);




#endif // _OBJ_MODEL_EXAMPLE_H__

