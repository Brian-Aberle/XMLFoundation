#include "ExampleObjectModeling.h"
#ifdef _WIN32
#include <direct.h> // for _getcwd()
#endif

// for every DECLARE_FACTORY() in the Object Model definition in ["ExampleObjectModeling.h] 
// there must be an IMPLEMENT_FACTORY() in a .cpp file where the Objects are implemented.
// The general process for XML-To-Object mapping is explained in the example C++ programs.
IMPLEMENT_FACTORY(INISection, section);
IMPLEMENT_FACTORY(INIEntry, setting);
//
IMPLEMENT_FACTORY(CMatter, Matter)
//------------------------------------------------
IMPLEMENT_FACTORY(CLife, Life)
//------------------------------------------------
IMPLEMENT_FACTORY(CHuman, Human)
//------------------------------------------------
IMPLEMENT_FACTORY(ModelContainer, Model)
//------------------------------------------------
//
IMPLEMENT_FACTORY(MyExistingObjectB, MyObject)



////////////////////////////////////////////////////////////
//
// This is the XML we'll process.
//
const char pzXML[] =
"<configuration>"
"<section name='Section A'>"
"<setting name='Path' value='/XMLFoundation/Examples/'/>"
"</section>"
"<section name='System'>"
"<setting name='Pool' value='20'/>"
"<setting name='ProxyPool' value='0'/>"
"</section>"
"<section name='HTTP'>"
"<setting name='Port' value='8088'/>"
"<setting name='Enable' value='yes'/>"
"<setting name='Home' value='/XMLFoundation'/>"
"<setting name='Index' value='XMLFoundation2014.html'/>"
"<setting name='EnableServerExtensions' value='yes'/>"
"</section>"
"</configuration>";




void MyExistingObjectA::ReadDataFormat(const char* pzData)
{
	// This creates temporary XMLObject, that essentially calls MapMember() on non-member variables
	// the call to AddReference() mapped the list in 'this' object
	XMLRelationshipWrapper objectContainer("configuration");
	objectContainer.AddReference(GProfileSection::GetStaticTag(), &m_lstSections, &gGListHandler, 0, 1);
	objectContainer.FromXMLX(pzData);
	// so now [objectContainer] is destroyed and [m_lstSections] was populated by FromXMLX()
}
const char* MyExistingObjectA::WriteDataFormat(GString* pstrDest)
{
	XMLRelationshipWrapper objectContainer("configuration");
	objectContainer.AddReference(GProfileSection::GetStaticTag(), &m_lstSections, &gGListHandler, 0, 1);
	objectContainer.ToXML(pstrDest);
	return *pstrDest;
}

////////////////////////////////////////////////////////////
void MyExistingObjectB::ReadDataFormat(const char* pzData)
{
	FromXMLX(pzData);  // it cant get much easier than this.
}
const char* MyExistingObjectB::WriteDataFormat(GString* pstrDest)
{
	ToXML(pstrDest);  // it cant get much easier than this.
	return *pstrDest;
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void MyExistingObjectC::ReadDataFormat(const char* pzData)
{
	m_objectContainer.FromXMLX(pzData);  // it cant get much easier than this.
}
const char* MyExistingObjectC::WriteDataFormat(GString* pstrDest)
{
	m_objectContainer.ToXML(pstrDest);  // it cant get much easier than this.
	return *pstrDest;
}


// ******************************************************************
// ******************************************************************
// ******************************************************************
GString* g_pstrOutput = 0;  

void ModelContainer::MapXMLTagsToMembers()
{
	// Example of a Mixed member map, normally you would map CHumans 
	// into one list and CLife into another.  Here they go in 1 list.
	MapMember(&m_lstMaterials, CHuman::GetStaticTag());
	MapMember(&m_lstMaterials, CLife::GetStaticTag());
}




void AdvancedInheritMapping(GString *strDest)
{
//	printf("\r\n\r\n");
	*strDest << "\r\n\r\n";

	ModelContainer m_modelContainer;


	GString strPath;
	GString strEXE;
	GDirectory::EXEPathAndName(&strPath, &strEXE);
	strPath << "ContainOrInherit.xml";



	CHuman* x = new CHuman;
	x->m_strDNA = ("m_strDNA_x");
	x->m_strFingerPrint = ("m_strFingerPrint_x");
	x->m_strGender = ("m_strGender_x");
	x->m_strWeight = ("m_strWeight_x");

	CLife* y = new CLife;
	y->m_strDNA = ("m_strDNA_y");
	y->m_strWeight = ("m_strWeight_y");

	CHuman* z = new CHuman;
	z->m_strDNA = ("m_strDNA_z");
	z->m_strFingerPrint = ("m_strFingerPrintz");
	z->m_strGender = ("m_strGender_z");
	z->m_strWeight = ("m_strWeightz");

	m_modelContainer.m_lstMaterials.AddLast(x);
	m_modelContainer.m_lstMaterials.AddLast(y);
	m_modelContainer.m_lstMaterials.AddLast(z);

	// Create the XML
	m_modelContainer.ToXMLFile(strPath);

	// Empty our list
	m_modelContainer.m_lstMaterials.RemoveAll();

	// re-load the list from the XML file
	m_modelContainer.FromXMLFile(strPath);

	GListIterator it(&m_modelContainer.m_lstMaterials);
	while (it())
	{

		//		XMLObject *pO = (XMLObject *)it++;    // you can do this ,  or this....   
		CMatter* pO = (CMatter*)it++;

		// this opens up use cases for the RTTI (Run Time Type Information) within XMLFoundation  
		// Search the source of XMLFoundation for uppercase RTTI and see what I am referring to.

		//printf(pO->m_strWeight);
		*strDest << pO->m_strWeight << "\r\n";
//		printf("\r\n");
	}
}




char pzXML3[] =
"<Human>"
"<Gender>Male</Gender>"
"<DNA>1101010001010101101011000010101010</DNA>"
"<FingerPrint>Unique</FingerPrint>"
"<Weight>777</Weight>"
"</Human>";



void MappingAndInheritance(GString *strDest)
{
	CHuman O;

	//"<Human>"
	//	"<Gender>Male</Gender>"
	//	"<DNA>1101010001010101101011000010101010</DNA>"
	//	"<FingerPrint>Unique</FingerPrint>"
	//	"<Weight>777</Weight>"
	//"</Human>";  
	O.FromXMLX(pzXML3);

	// O conatins all of the data although it only made 2 direct MapMembers
//	GString strDebug;
	*strDest << "\n\n\nGender:" << O.m_strGender << "      FingerPrint:"
		<< O.m_strFingerPrint << "\n" << "DNA:" << O.m_strDNA
		<< "      Weight:" << O.m_strWeight << "\n\n";
//	printf(*strDest);
	//
	// Here you can see that although a CHuman only mapped 2 items it has all 4
	//
	// Gender:Male      FingerPrint:Unique
	// DNA:1101010001010101101011000010101010      Weight:777	
	//////////////////////


	// and likewise it writes out all 4 in XML although it only mapped 2
	*strDest << O.ToXML();
//	printf(O.ToXML());
	//////////////////////
	//<Human>
	//        <FingerPrint>Unique</FingerPrint>
	//		  <Gender>Male</Gender>
	//        <DNA>1101010001010101101011000010101010</DNA>
	//        <Weight>777</Weight>
	//</Human>	
	//////////////////////



	//Notice that CLife is being created with the same xml that the CHuman was created with.
	CLife life;
	life.FromXML(pzXML3);// Gender and FingerPrint are now unmapped data
//	strDebug.Empty();
	*strDest << "\n\nDNA:" << life.m_strDNA << "      "
		<< "Weight:" << life.m_strWeight << "\n\n";
//	printf(strDebug);
	//////////////////////
	// DNA:1101010001010101101011000010101010      Weight:777
	//////////////////////


//	printf(life.ToXML());
	*strDest << life.ToXML();
	//////////////////////
	//<Life>
	//        <DNA>1101010001010101101011000010101010</DNA>
	//        <Weight>777</Weight>
	//</Life>
	//////////////////////


	// So- for example, you may create an object CPlant that like
	// the CHuman is derived from CLife.  A CPlant would contain
	// the elements of CLife (DNA) and of CMatter (Weight) by inheritance.

	// If each XML message represents a transaction it is wise
	// to map the commonalities of all transactions, or groups of
	// transactions into a base class that allows derivatives
	// to inherit the base elements of the transaction that
	// will only be maintained in one place.

}







