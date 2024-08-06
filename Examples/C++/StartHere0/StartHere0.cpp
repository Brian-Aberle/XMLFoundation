//---------------------------------------------------------------------------------------
// Beginning with XMLFoundation....
//
// This example is for Linux, Unix, and Windows.
//
// This is the smallest simplest working example
//----------------------------------------------------------------------------------------
 


// ------------ Makefile or Project settings --------------------
// make sure you have "/XMLFoundation/Libraries/XMLFoundation/inc" in the compile include path
// link to  "/XMLFoundation/Libraries/Debug/XmlFoundation.lib" (libXMLFoundation.a in Linux)
// In Windows verify the default C Runtime Libraries is "Multithreaded DLL" or "Debug Multithreaded DLL"
// Because that is the default build setting for XMLFoundation - just as it is for MFC apps.
#include <XMLFoundation.h>

#include <string.h> // for strcpy()


//////////////////////////////////////////////////////////////////
// Define a simple object
//////////////////////////////////////////////////////////////////
class MyCustomObject : public XMLObject
{
public:							// make public here for example simplicity - this is not required
	GString m_strString;		// A String Member
	GString m_strColor;			// An XML attribute , not an XML element
	int m_nInteger;				// An Integer Member
	char m_szNative[10];		// a fixed 10 byte buffer
	bool m_bBool;				// bool
	GStringList m_strList;      // A String List

	unsigned char m_bits;	    // 8 bits  
//	unsigned __int64 m_bits;	//  or map 64 bits - notice the comment above the final call to MapMemberBit() in MapXMLTagsToMembers() below

	virtual void MapXMLTagsToMembers()
	{
	//			Member variable		XML Element		
		MapMember( &m_strList,		"StringList", "Wrapper");
		MapMember(	&m_nInteger,	"Number");
		MapMember(	&m_strString,	"String");
		MapMember(	&m_bBool,		"Bool");
		MapMember( m_szNative,		"FixedBuffer", sizeof(m_szNative) );
 
		MapAttribute(&m_strColor,	"Color");
 
		MapMemberBit( &m_bits,		"Seven77thBit", 7, "False,No,Off,0",  "True,Yes,On,1");
		MapMemberBit( &m_bits,		"OnItsSide8",   8, "Black",			  "White");

		// if 55 is out of range because m_bits  was mapped to an 8 bit unsigned char then it will not map 
		// --- LOOK IN THE ERROR LOG ---  to find many design time errors like this
		MapMemberBit( &m_bits,		"Fifty5",		55, "no",			  "yes"); 
		
	}
	
	// 'this' type, followed by the XML Element name, normally DECLARE_FACTORY() is in an .h file
	DECLARE_FACTORY(MyCustomObject, Thing) 
 
	// keep one constructor with no arguments
	MyCustomObject(){m_bits=0;}
	~MyCustomObject(){};
};
// IMPLEMENT_FACTORY() should exist in a .CPP file - not an .h file - one for every DECLARE_FACTORY()
IMPLEMENT_FACTORY(MyCustomObject, Thing)
 
//////////////////////////////////////////////////////////////////
// Define a simple object
//////////////////////////////////////////////////////////////////
class MyCustomObject2 : public XMLObject
{
public:							// make public here for example simplicity - this is not required
	GString m_strString2;		// A String Member

	virtual void MapXMLTagsToMembers( ) {
		//			Member variable		XML Element		
		MapMember( &m_strString2 , "String2" );
 
	}
 
	// 'this' type, followed by the XML Element name, normally DECLARE_FACTORY() is in an .h file
	DECLARE_FACTORY( MyCustomObject2 , Thing2 )
 
	MyCustomObject2( ) { } // keep one constructor with no arguments
	~MyCustomObject2( ) { };
};
// IMPLEMENT_FACTORY() must exist in a .CPP file - not an .h file - one for every DECLARE_FACTORY()
IMPLEMENT_FACTORY( MyCustomObject2 , Thing2 )
 
//////////////////////////////////////////////////////////////////
// Define a simple object
//////////////////////////////////////////////////////////////////
class MyCustomObject3 : public MyCustomObject , public MyCustomObject2
{
public:							// make public here for example simplicity - this is not required
	GString m_strString3;		// A String Member

	virtual void MapXMLTagsToMembers( ) 
	{
		MyCustomObject::MapMember( &m_strString3 , "String3" );
	}
 
	// 'this' type, followed by the XML Element name, normally DECLARE_FACTORY() is in an .h file
	DECLARE_FACTORY( MyCustomObject3 , Thing3 )
 
	MyCustomObject3( ) { } // keep one constructor with no arguments
	~MyCustomObject3( ) { };
};
// IMPLEMENT_FACTORY() must exist in a .CPP file - not an .h file - one for every DECLARE_FACTORY()
IMPLEMENT_FACTORY_MI( MyCustomObject3 , Thing3, MyCustomObject )



//
// This is the XML we'll process.
//
char pzXML[] = 
"<Thing Color='Black and White'>"
	"<String>Hello World!</String>"  
	"<Number>1984</Number>"
	"<Bool>True</Bool>"
	"<FixedBuffer>native</FixedBuffer>"
	"<Wrapper>"
	     "<StringList>one</StringList>"
	     "<StringList>two</StringList>"
	"</Wrapper>"
	"<Seven77thBit>on</Seven77thBit>"
	"<OnItsSide8>white</OnItsSide8>"
	"<Fifty5>yes</Fifty5>"
"</Thing>";



#ifdef _WIN32
	#define LONG_ONE 1i64
#else
	#define LONG_ONE 1LL
#endif



int main(int argc, char* argv[])
{
	MyCustomObject3 x;
	
	MyCustomObject O;
	O.FromXMLX(pzXML);

	// look at the Object "O".
	GString strDebug;
	strDebug << "After FromXMLX()=[" << O.m_strString << "][" << O.m_nInteger << "][" << O.m_strColor << "][" << O.m_szNative << "]\n\n\n";


	// print out the value of each individual bit in m_bits - we only made changes within the first 8 bits
	// Change "<OnItsSide8>" from "white" to "black" in pzXML and watch how the bits change
	for(int i=0;i<8;i++)
		strDebug.FormatAppend("bit%d=%d\n",i+1,((O.m_bits&(LONG_ONE<<i))!=0));  // :)
//		this is the output:
//			bit1=0
//			bit2=0
//			bit3=0
//			bit4=0
//			bit5=0
//			bit6=0
//			bit7=1
//			bit8=1

	

	// set some data 
	O.m_strString = "Root was here";
	
	// or any kind of "invalid" XML value  (notice how it automatically becomes escaped XML proper in the output)
	O.m_strString.Empty();
	for (int ch=32; ch < 256; ch++)
	{
		O.m_strString << (char)ch;
	}
	// attributes escape a little differently
	O.m_strColor = O.m_strString;


//
// It's the whole set of characters, escaped where need be.
//
// <Thing Color=" !&#34;#$&amp;'()*+,-./0123456789:;&lt;=&gt;?@ABCDEFGHIJKLMNOPQRST
// UVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~���������������������������ܢ��P������
// Ѫ����������������++��++++++--+-+��++--�-+----++++++++�_���a�GpSs�tFTOd8fen=�==(
// )�����vn���">


	O.m_nInteger = 21;

	strcpy(O.m_szNative,"so native");
	O.m_bBool = !O.m_bBool;

	// turn the 8th bit off - changes the value to "Black" in the XML - remove the following line and it stays "White" in the output XML
	O.m_bits &= ~(1<<7);

	// add any encoding tags or doctype you need - if you need them other wise skip the next two lines
//	GString strXMLStreamDestinationBuffer = "<?xml version=\"1.0\" encoding='windows-1252'?>\r\n";
	GString strXMLStreamDestinationBuffer = "<?xml version=\"1.0\" standAlone='yes'?>\n";
//	strXMLStreamDestinationBuffer << "<!DOCTYPE totallyCustom SYSTEM \"http://www.IBM.com/example.dtd\">\r\n";
	O.ToXML( &strXMLStreamDestinationBuffer);
  
//	// this is the xml in strXMLStreamDestinationBuffer 
//	<?xml version="1.0" standAlone='yes'?>
//	<Thing Color="Black and White">
//			<Wrapper>
//					<StringList>one</StringList>
//					<StringList>two</StringList>
//			</Wrapper>
//			<Number>21</Number>
//			<String>Root was here</String>
//			<String2>Root</String2>
//			<Bool>False</Bool>
//			<FixedBuffer>so native</FixedBuffer>
//			<Seven77thBit>True</Seven77thBit>
//			<OnItsSide8>Black</OnItsSide8>
//	</Thing>.

//	strXMLStreamDestinationBuffer.ToFile("MyOutputFile.xml");

	strDebug << strXMLStreamDestinationBuffer;
	printf(strDebug);

	return 0;
}


