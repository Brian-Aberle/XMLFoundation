#include <XMLFoundation.h>

#include <string.h> // for: strlen()


class MyCustomObject : public XMLObject
{
public:
	GString m_strString;		// A String Member
	GString m_strColor;			// An attribute , not an element
	int m_nInteger;				// An Integer Member
	GStringList m_strList;      // A String List

	virtual void MapXMLTagsToMembers()
	{
	//			Member variable		XML Element		
		MapMember( &m_strList,		"StringList", "Wrapper");
		MapMember(	&m_nInteger,	"Number");
		MapMember(	&m_strString,	"String");
		MapAttribute(&m_strColor,	"Color");
	}
	
	// 'this' type, followed by the XML Element name, normally DECLARE_FACTORY() is in an .h file
	DECLARE_FACTORY(MyCustomObject, Thing) 

	MyCustomObject(){} // keep one constructor with no arguments
	~MyCustomObject(){};
};
// IMPLEMENT_FACTORY() must exist in a .CPP file - not an .h file - one for every DECLARE_FACTORY()
IMPLEMENT_FACTORY(MyCustomObject, Thing)



//
// This is the XML we'll process.
//
char pzXML[] = 
"<Thing Color='Red White and Blue'>"
	"<![CDATA[<data>x</data>]]>"
	"-Object Data-="
	"<String>Capitol Capital G</String>"
	"<Number>777</Number>"
	"<Wrapper>"
	     "<StringList>one</StringList>"
	     "<StringList>two</StringList>"
	"</Wrapper>"
	"=-More Object Data-"
"</Thing>";


void ObjectDataAndCDataExample()
{
	MyCustomObject O;
	O.FromXMLX(pzXML);

	GString *pG = O.GetCDataStorage();
	printf(*pG); // prints out "<data>x</data>"
	printf("\n\n");  

	// Notice that the memory address of the (unparsed)CDATA start is 43 bytes past &pzXML[0]
	// This CDATA buffer will always be in the original memory, not a copy of that buffer.
	// This unparsed data from the source input XML stream is handed directly to the application layer
	__int64 nOffsetFromStartofXML = pG->Buf() - pzXML; // nOffsetFromStartofXML is 43


	pG = O.GetObjectDataStorage();
	// you can see by this offset that we are in a different memory region now 
	// (because the fragmented Object data is now contiguous)
	nOffsetFromStartofXML = pG->Buf() - pzXML;    // nOffsetFromStartofXML is way out there.
	printf(*pG);  // prints out "-Object Data-==-More Object Data-"
	printf("\n\n");

	// notice that the 'normal' data mapped to String, StringList, and Int 
	// are in the members and data structures where they have been mapped to.
	

	// Notice how the Object Data is rearranged in the output to a lexically equal notation.
	// Also notice that no whitespace (carriage returns or tabs) can be used to beautify this ugly Object Data
	// or it would alter the data, unlike the whitespace added between XML element tags to beautify them.
	// If Object Data has a Carriage Return in it, that is part of the data.
	printf(O.ToXML());
	printf("\n\n");
/*
<Thing Color="Red White and Blue">
        <![CDATA[<data>x</data>]]>-Object Data-==-More Object Data-
        <Wrapper>
                <StringList>one</StringList>
                <StringList>two</StringList>
        </Wrapper>
        <Number>777</Number>
        <String>Words</String>
</Thing>
*/
	//  ------------------------------
	
	// now inversely, from a fresh object O2, make the xml
	MyCustomObject O2;
	O2.m_nInteger = 777;
	O2.m_strString = "G.G.G.Guru";
	O2.m_strColor = "Gold, Green and White";
	*(O2.GetCDataStorage()) << "x<data>x";							// notice how this CData is not parsed
	O2.SetObjectDataValue("-- object <data> is parsed --");			// and how the object data is parsed
	printf(O2.ToXML());												// when they turn to xml
/*																	// it looks like this:
<Thing Color="Gold, Green and White">								
        <![CDATA[x<data>x]]>-- object &#60;data&#62; is parsed --	
        <Number>777</Number>
        <String>G.G.G.Guru</String>
</Thing>
*/
	printf("\n\n");

}

// ******************************************************************
// ******************************************************************
// ******************************************************************

class NewMemberMaps2012 : public XMLObject
{
public:
	char m_ch1;
	char m_ch2;
	short m_s;
	char m_buf1[10];
	char m_buf2[10];
	char m_buf3[10];

	virtual void MapXMLTagsToMembers()
	{
	//			Member variable		XML Element		
		MapMember( &m_ch1,	"Byte1");
		MapMember( &m_ch2,	"Byte2");
		MapMember( &m_s,	"Short");
		MapMember( m_buf1,	"Buf1", sizeof(m_buf1) );
		MapMember( m_buf2,	"Buf2", sizeof(m_buf2) );
		MapMember( m_buf3,	"Buf3", sizeof(m_buf3) );
	}
	
	// 'this' type, followed by the XML Element name, normally DECLARE_FACTORY() is in an .h file
	DECLARE_FACTORY(NewMemberMaps2012, New) 

	NewMemberMaps2012(){} // keep one constructor with no arguments
	~NewMemberMaps2012(){};
};
// IMPLEMENT_FACTORY() must exist in a .CPP file - not an .h file - one for every DECLARE_FACTORY()
IMPLEMENT_FACTORY(NewMemberMaps2012, New)


//
// This is the XML we'll process.
//
char pzXML2[] = 
"<New>"
	"<Byte1>G</Byte1>"
	"<Byte2>&#168;</Byte2>"		  
	"<Short>777</Short>"
	"<Buf1>My Rating</Buf1>"	 // 9 bytes + a null fits inside Buf1
	"<Buf2>They YellXXX</Buf2>"  // this will be truncated, the triple X will be removed.
	"<Buf3>G.G.G</Buf2>"		 
"</New>";




void NewKindsOfMemberMapsIn2012()
{
	NewMemberMaps2012 O;
	O.FromXMLX(pzXML2);

	// m_ch2 mapped to <Byte2>&#168;</Byte2>  
	// The debugger will display m_ch2 as -88 because, its a signed char
	// (char)-88 is equal to (unsigned char)168 and equal to "11000000" in binary
	printf(O.ToXML());
	// and when it gets printed out in XML its back to the escaped format of &#168;
}





class AdvancedDataMaps : public XMLObject
{
public:
	GString m_strString;		// A String Member

	enum DataTypes 
	{ 
		Byte,
		Short,
		Int,
		Int64, 
	} m_DataType;

	union STATE_OF_THE_UNION
	{
		char n1Byte;
		short n2Bytes;
		int n4Bytes;
		__int64 n8Bytes;
	}m_MutualExclusion;


	struct MEMBER_STATE {
		int N;
		char buf[64];
	}m_State;


	virtual void MapXMLTagsToMembers()
	{
		// notice the cast required for the Enum
		MapMember(	(int *)&m_DataType,	"DataType");

		// only one of the following may be present in the XML, they are mutually exclusive
		MapMember(	&m_MutualExclusion.n1Byte,	"Byte"); 
		MapMember(	&m_MutualExclusion.n2Bytes,	"Short"); 
		MapMember(	&m_MutualExclusion.n4Bytes,	"Int"); 
		MapMember(	&m_MutualExclusion.n8Bytes,	"Int64"); 

		// Objects OR structs of direct containment are mapped directly 
		MapMember(	&m_State.N,	"N");
		MapMember(	m_State.buf,	"buf", sizeof(m_State.buf));
	}

	// if the XML contains words that must translate to integer value, as in the case of an enumeration
	virtual void *ObjectMessage( int nCase, const char *pzTagName, const char *pzData, __int64 nDataLen, void *pUnused = 0 )
	{
		// note: the MSG_NON_NUMERIC handler is only required if the xml contains string name 
		// descriptions of the enumerated type, a numeric value will map directly with no need for
		// translation through this overloaded ObjectMessage() handler
		if (nCase == MSG_NON_NUMERIC)
		{
			if (strcmp(pzTagName,"DataType") == 0)
			{
				if (strcmp(pzData,"Byte") == 0)
				{
					// convert from string in the xml to enumerated type in the object
					m_DataType = AdvancedDataMaps::Byte;
				}
				else if (strcmp(pzData,"Short") == 0)
				{
					m_DataType = AdvancedDataMaps::Short;
				}
				else if (strcmp(pzData,"Int") == 0)
				{
					m_DataType = AdvancedDataMaps::Int;
				}
				else if (strcmp(pzData,"Int64") == 0)
				{
					m_DataType = AdvancedDataMaps::Int64;
				}
			}
		}
		else if (nCase == MSG_MEMBER_UPDATE)
		{
			if (strcmp(pzTagName,"DataType") == 0)
			{
				if (strcmp(pzData,"Byte") == 0)
				{
					SetMemberSerialize("Byte"	,1);
					SetMemberSerialize("Short"	,0);
					SetMemberSerialize("Int"	,0);
					SetMemberSerialize("Int64"	,0);
				}
				else if (strcmp(pzData,"Short") == 0)
				{
					SetMemberSerialize("Byte"	,0);
					SetMemberSerialize("Short"	,1);
					SetMemberSerialize("Int"	,0);
					SetMemberSerialize("Int64"	,0);
				}
				else if (strcmp(pzData,"Int") == 0)
				{
					SetMemberSerialize("Byte"	,0);
					SetMemberSerialize("Short"	,0);
					SetMemberSerialize("Int"	,1);
					SetMemberSerialize("Int64"	,0);
				}
				else if (strcmp(pzData,"Int64") == 0)
				{
					SetMemberSerialize("Byte"	,0);
					SetMemberSerialize("Short"	,0);
					SetMemberSerialize("Int"	,0);
					SetMemberSerialize("Int64"	,1);
				}
			}
		}
		return 0;
	};
	
	DECLARE_FACTORY(AdvancedDataMaps, Maps) 

	AdvancedDataMaps(){ ModifyObjectBehavior(MEMBER_UPDATE_NOTIFY); }
	~AdvancedDataMaps(){};
};
IMPLEMENT_FACTORY(AdvancedDataMaps, Maps)
char pzXML4[] = 
"<Maps>"
	"<DataType>Byte</DataType>"
	"<Byte>X</Byte>"
	"<N>777</N>"
	"<Buf>hello world</Buf>"
"</Maps>";


char pzXML5[] = 
"<Maps>"
	"<DataType>Int</DataType>"
	"<Int>1234567890</Int>" // 1.2 billion
	"<N>7</N>"
	"<Buf>Global Hello</Buf>"
"</Maps>";


void MapUnionStructAndEnum()
{
	AdvancedDataMaps O;
	O.FromXML(pzXML4);

	GString strDebug;
	strDebug << "\n\nEnum=" << O.m_DataType << "   Union=" << O.m_MutualExclusion.n1Byte << "\n"
		     << "m_State.N=" << O.m_State.N << "   m_State.buf=" <<  O.m_State.buf << "\n\n";
	printf(strDebug);
///////////////////////////////
//Enum=0   Union=X
//m_State.N=777   m_State.buf=hello world
///////////////////////////////
	
	printf(O.ToXML());
///////////////////////////////
//<Maps>
//	<DataType>0</DataType>
//	<Byte>X</Byte>
//  <N>777</N>
//  <buf>hello world</buf>
//</Maps>	
///////////////////////////////

	// notice that now the XML is pzXML5
	O.FromXML(pzXML5);
	strDebug << "\n\nEnum=" << O.m_DataType << "   Union=" << O.m_MutualExclusion.n4Bytes << "\n"
		     << "m_State.N=" << O.m_State.N << "   m_State.buf=" <<  O.m_State.buf << "\n\n";
	printf(strDebug);
///////////////////////////////
//Enum=2   Union=1234567890
//m_State.N=7   m_State.buf=Global Hello
///////////////////////////////

	printf(O.ToXML());
///////////////////////////////
//<Maps>
//     <DataType>2</DataType>
//     <Int>1234567890</Int>
//     <N>7</N>
//     <buf>Global Hello</buf>
//</Maps>
///////////////////////////////
}
// ******************************************************************
// ******************************************************************
// ******************************************************************


int main(int argc, char* argv[])
{
try
{
	ObjectDataAndCDataExample();

	NewKindsOfMemberMapsIn2012();

	MapUnionStructAndEnum();


/*  GString0  and GString32  depreciated in 2024

	// A Note on the 2 new classes, GString0 and GString32 added to the XMLFoundation on 4/6/2014,
	// normally you will not bother with anything but a GString, the difference between the various
	// GStrings types is ONLY the size of the base allocation.  If the objects data can fit inside
	// the size of the GString object you selected, then you are rewarded with performance by avoiding a call to new()
	// If you No data or Small data into a GString then you are punished with excess memory use, 
	// often its a small punishment we choose to bear to avoid the worse punishment of the performance penalty for calling new().
	// Life is all about tradeoffs. 

	// sizeof(GString0)  ==  48
	// sizeof(GString32) ==  80
	// sizeof(GString)   == 112
	int a = sizeof(GString0);
	int b = sizeof(GString32);
	int c = sizeof(GString);

	GString0 zero;
	GString0 zero2;
	zero2 << "zero"; // this is slow, step through it and watch call new() 3 times to assign the value - you should pre-alloc a GString0 when you use it.
	GString one("one");
	GString32 two("two");
	
	GString three = "X";
	three += zero;
	three += "X";
	three += zero2;
	three += "X";
	three += one;
	three += "X";
	three += two;

	GString four;
	four << "X" << zero << "X" << zero2 << "X" << one << "X" << two;

	GString five;
	five += "X";
	five += zero;
	five += "X";
	five += zero2;
	five += "X";
	five += one;
	five += "X";
	five += two;

	if (three != four)
	{
		printf("this cant happen");
	}
	if (five != four)
	{
		printf("this cant happen");
	}
*/

}
catch( GException &rErr )
{
	// you probably have invalid XML if you get here...
	printf( rErr.GetDescription() );
}
	return 0;
}


