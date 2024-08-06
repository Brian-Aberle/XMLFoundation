// --------------------------------------------------------------------------
//						United Business Technologies
//			  Copyright (c) 2000 - 2014  All Rights Reserved.
//
// Source in this file is released to the public under the following license:
// --------------------------------------------------------------------------
// This toolkit may be used free of charge for any purpose including corporate
// and academic use.  For profit, and Non-Profit uses are permitted.
//
// This source code and any work derived from this source code must retain 
// this copyright at the top of each source file.
// --------------------------------------------------------------------------



//
//   GString.....
//
// 2X faster than using an ostream with a preallocated strstreambuf.
// 10X more portable.  You're covered if you have your GString on.
// It's more robust than CString - more rogue than RWString - and more std than std::string
//
// This class is actually a cross between a string and a stream.  It's a hybrid.
// It operates natively on both string data and binary content and it compiles everywhere.
//
//



#ifndef _G_STRING_HH_
#define _G_STRING_HH_



#ifndef ___max
#define ___max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef ___min
#define ___min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifdef ___IOS
	#include <inttypes.h>
	#define	__int64 long long
#else
	#ifndef _WIN32
		#ifndef __int64
			#define	__int64 long long
		#endif
	#endif
#endif

#if defined(_MSC_VER) && _MSC_VER <= 1200 
	#include <stdio.h>  // for wchar_t definition
#endif


#include <stdarg.h> // for va_list declaration


// Set this size to fit your own application.  Speed vs memory tradeoff - try to avoid most re-allocs during XML Parsing
// If you change the value, you must rebuild the library and your application.  This is the default size of the GString
// as you modify GSTRING_INITIAL_SIZE you will see the value of sizeof(GString) to change accordingly.
#define GSTRING_INITIAL_SIZE		64



// GString0 and GString32 were removed from the XMLFoundation in 2024  for now they remain commented out of the code.
//
//class GString32;  // GString32 uses a 32 byte initial buffer
//class GString0;  // GString0 uses no initial stack buffer, empty strings use less memory, they are SLOW to assign unless you preallocate them.





class GStringList;
// sizeof(GString0)  ==  48
// sizeof(GString32) ==  80
// sizeof(GString)   == 112


class GString 
{
public: 
	// _str and _len are conceptually protected, or private.  Best advice: UNDERSTAND HOW GSTRING WORKS.
	// Know that in MY code, I nearly NEVER change the length of a GString by assigning a value directly to _len.
	// Look how much wiser it is to use SetLength().  I like MY code to be clear and readable and i have found that
	// the publicity of this normally protected data can be helpful in that regard.  Look at GetLength() then
	//
	// look at the difference in this:		foo(s._len)     vs      foo(s.GetLength())
	//										
	// likewise consider this:				foo( (char *)(const char *)s )    or        
	//										foo( (char *)s.StartingAt(0) )    or   
	//										foo(s.Buf(0))   
	//								   vs.  foo(s._str)
	// Note that Buf() was already a little risque in style(aka 'in the buff').  It was added many years ago 
	// for this very reason of adding simplistic readability amid complex totality.
	// It would be wise to understand how your GString works or your might put it on backwards and everyone will laugh at you.
	//
	// however - the publicity of _str and _len allows direct memory addressing (guaranteed) without a method call.  
	// This allows a debugger to treat the variable a bit different:
	// For example - this is the 4th byte of a GString
	// ptrToGString->_str[3] - this code will NOT call the operator [] - so there is no index bounds checking
	// this also allows the debugger to 'view' a (guaranteed vs inlined) variable
	//
	// GString s(512);								// KNOW how big your _str is
	// GetModuleFileName(0, s._str, s._len);		// KNOW that GetModuleFileName() will not write beyond the length
	// s._len = strlen(s._str);						// KNOW that the value returned from strlen() is within the bounds WITHOUT exception
	//
	char *_str;	   // pointer to string data
	__int64  _len; // length of string data not counting terminating null
	wchar_t * _pWideStr; // Unicode working storage

protected:
	__int64  _max; // current memory allocation size of _str
	int _growby;	// size of memory growth chunks.
	void resize(); // grows _max and moves data from old memory to new larger contiguous memory

	// _initialbuf is allocated along with 'this' to reduces malloc/free calls.
	// When a GString < DEFAULT_INITIAL_SIZE bytes is constructed, _initialbuf is used.
	// As the string grows beyond DEFAULT_INITIAL_SIZE, _strIsOnHeap becomes true and
	// _str no longer points into _initialbuf, but now is on the heap.
	//
	char _initialbuf[GSTRING_INITIAL_SIZE]; // do not access this variable, use _str

	short _strIsOnHeap; // 0 when we're working in our _initialbuf
					  // 1 when were working in memory we allocated OR own
					  // 2 when working in Attached() memory

public:
	// System wide GString settings that affect all GStrings and GString32s on all threads
	static GString	g_FloatFmt;			// "%.7g"
	static GString	g_DoubleFmt;		// "%.7g"
	static GString	g_LongDoubleFmt;	// "%.7Lg"

	// frees the buffer
	virtual ~GString();

	// constructs an empty string
	GString(__int64 nInitialSize = GSTRING_INITIAL_SIZE, int nGrowByAllocationSize = -1);

	// constructs a copy of the source string 
//	GString(const GString0 &src);
//	GString(const GString32 &src);
	GString(const GString &src);
	GString(const GString &src, __int64 nSourceCount);

	// constructs a copy of the character string
	GString(const char *pzSource);
	GString(const char *pzSource, __int64 nSourceCount);

	// constructs a string with ch repeated nCnt times
	// if you get a compile error "2 overloads have similar conversions"
	// while using this ctor cast nCnt to a short:   GString strXs('X',(short)777)
	GString(char ch, short nTimesToRepeat);

	// This will create a GString from a double byte BSTR, or Platform::String^ in .NET managed code like this:
	// Platform::String^ MyString = "MyData";
	// new GString(MyString->Data());
	// wchar_t is a native type in VS2012. 
	// ****** Use GString::FromUnicode() if this is not an ASCII string
	GString(const wchar_t *strWide);
	

	// Attach to a memory buffer, WITHOUT making a copy of the memory.
	// ------------------------------------------------------------
	// constructs a string attached to supplied memory
	// so that the memory is NOT copied into this GString.
	// See notes for Attach(), they apply to this ctor as well
	// so to create a GString on the heap that uses memory on the stack:
	//
	//     char p[512]; strcpy(p,"hello world");
	//     GString *p2 = new GString(p,strlen("hello world"),512,0);
	//
	//   obviously, that stack frame must outlast the duration of p2, or it will crash violently.
	//		
	// This is the 'ctor version of:
	// void Attach(const char *pzSource, int nSourceBytes, long nSourceMemorySize, int nOwn);
	// Read Attach() comment immediately below - it all applies to this 'ctor as well
	GString(const char *pzSource, __int64 nSourceBytes, __int64 nSourceMemorySize, int nOwn);

	
	// Attach to a memory buffer, WITHOUT making a copy of the memory.
	// ------------------------------------------------------------
	// pzSource is a pointer to memory that you allocated
	// nSourceMemorySize is the allocated size of pzSource
	// nSourceBytes is the number of bytes currently in pzSource
	//
	// nOwn is true(1) or false(0) indicating if the GString object now owns the memory.
	// if nOwn == true, then you alloc(new) pzSource, and the GString will free(delete) it
	// when when the scope of the object has been reached(in the destructor).
	//
	// if nOwn == false, you alloc/dealloc(new/delete) the buffer and you ensure
	// that pzSource is valid for the duration of this object. When the object goes out of scope
	// buf is NOT deleted by this object because you indicated GString does not own it.
	//
	// If any append operation(like << or +=) extends past the region of memory YOU allocated 
	// that is nSourceMemorySize bytes  - GString will allocate new memory and the attached 
	// memory will be deleted if nOwn = true otherwise (if nOwn = false) the buffer will be 
	// unattached from the GString and left for you to delete.
	//
	// note: although pzSource is const, it may be modified depending on what methods you call.
	// note: pzSource may contain nulls, binary or unprintable bytes.  nSourceBytes is the length and
	// GString never tries to strlen(pzSource) to determine the size.
	void Attach(const char *pzSource, __int64 nSourceBytes, __int64 nSourceMemorySize, int nOwn);
	


	// WriteOn() overwrites any existing value with n bytes from the src 
	// if src is not null terminated it will be terminated in the GString owned copy of it.
	// 
	// s = "one two three four five six "; s.WriteOn("seven");
	// now the value of s is only "seven"
	void WriteOn(const char *pSource,__int64 nSourceBytes);
	void WriteOn(unsigned char *pSource,__int64 nSourceBytes){WriteOn((const char *)pSource,nSourceBytes);};

	// write() appends to the GString to mimic ostream 
	//
	// s = "Just when you think you've seen it all"; s.write("\0there is more.",15);
	// now the Length() is the length of the entire string contents, therefore
	// s.Length() > strlen(s);
	void write(const char *pSource,__int64 nBytes); // lower case for ostream interoperability just replace "ostream" with GString types and recompile for 2X string manipulation speed.
	void Write(const char *pSource,__int64 nBytes){write(pSource,nBytes);}; // upper case for Development tools method sorting
	void write(unsigned char *pSource,__int64 nBytes){write((const char *)pSource,nBytes);}; 
	void Write(unsigned char *pSource,__int64 nBytes){write((const char *)pSource,nBytes);}; // upper case for Development tools method sorting

	
	// When the entire XMLFoundation is compiled inline into an MFC app, not linked as a library  "write" may become defined as "_write" in MFC code somewhere.
	// In that case, by adding this method that behaves exactly the same as "write", the frameworks work well together.  Do not ever call str._write()
	void _write(const char *pSource, __int64 nBytes){ write(pSource, nBytes); }; 


	// Load this GString from a file and replace any contents in this GString - returns 1 for success, 0 for fail
	bool FromFile(const char* pzFileName, bool bThrowOnFail = 1);
	// Append the contents of the specified file to the end of this GString - returns 1 for success, 0 for fail
	bool FromFileAppend(const char* pzFileName, bool bThrowOnFail = 1);
	// Save this GString to a file - overwrite if file exists
	bool ToFile(const char* pzFileName, bool bThrowOnFail = 1);
	// Append this GString to a file - create file if not existing  returns 1 for success, 0 for failure
	bool ToFileAppend(const char* pzFileName, bool bThrowOnFail = 1);


	
	//	The mask can contain any printable character and is terminated 
	//	by a null.
	//
	//	The character '_' in the mask will be replaced by a single character
	//	in the source string.  The character '*' will be replaced by the
	//	remaining characters in the source string.  Both '_' and '*' can be
	//	escaped using the character '\'.  You can escape the escape character
	//	with an escape character, i.e. "\\".
	//
	//	Some example masks:
	//
	//	Phone number mask: (___)___-____ [ext:*]
	//	SSN mask: ___-__-____	(tip: don't use SSN as a key in your database!)
	//
	//	If the source data is larger than the mask and the mask doesn't contain
	//	the wild character '*' the source data will be truncated.
	void MergeMask(const char *szSrc, const char *szMask);

	// FormatNumber formats the value of the GString using the format
	// string specified by szFormat
	void FormatNumber(const char *szFormat, 
					  char decimal_separator = '.',
					  char grouping_separator = ',',
					  char minus_sign = '-',
					  const char *NaN = "NaN",
					  char zero_digit = '0',
					  char digit = '#',
					  char pattern_separator = ';',
					  char percent = '%',
					  char permille = '?');

	//  Appends an ASCII/HEX dump to this string. Looks like this:
	// pzLinePrefix  2F 31 2E 30 0D 0A 41 63 63 65 70 74 3A 20 2A 2F 2A 0D 0A 41 63 63 65 70 74      /1.0..Accept: */*..Accept
	// pzLinePrefix  2D 4C 61 6E 67 75 61 67 65 3A 20 65 6E 2D 75 73 0D 0A 55 73 65 72 2D 41 67      -Language: en-us..User-Ag
	//
	// You may set bIncludeAscii=0, to remove the line breaks and readable column of text from the result.
	// note: this appends to the string so you may first prefix the dump with context information:
	// Common usage: s="recv'd:"; s.FormatBinary(buf,N); s.ToFileAppend("debug.txt")
	void FormatBinary(unsigned char *pData, __int64 nBytes, int bIncludeAscii=1, const char *pzLinePrefix=0);
	void FormatBinary(const GString &strBinary, int bIncludeAscii =1);

	// CommaNumeric() - 123456789 will become 123,456,789
	// CommaNumeric() - 1000 will become 1,000
	// returns pointer to buffer inside 'this'
	const char *CommaNumeric();

	// convert 'this' to a user friendly byte notation with 2 digit unrounded precision
	// returns pointer to buffer inside 'this'
	//-----------------------------------------------------------------------------------
	// 999		=  999		1000000		=	1.0Mb	1000000000000	=1.0Tb
	// 1000		= 1.0Kb		10000000	=	 10Mb	9500000000000	=9.5Tb
	// 10000	=  10Kb		999999999	=	999Mb	999999999999999	=999Tb
	// 999999	= 999Kb		1000000000	=	1.0Gb
	const char *AbbreviateNumeric();


	// Call this member function to write formatted data to a  GString in the same way 
	// that sprintf formats data.  This works well for interoperability, However the  
	// operator << about 5 times faster than using Format().
	void Format(const char *lpszFormat, ...);// replaces 'this' value
	void FormatAppend(const char *lpszFormat, ...); // appends to 'this'
	void FormatPrepend(const char *lpszFormat, ...); // prepends to 'this'
	void FormatV(const char *lpszFormat, va_list argList);

	// removes leading and trailing quotation's from string
	void StripQuotes();

	// ------------  COMPARE ------------------------------
	// Returns zero if the strings are identical, 
	// < 0 if this GString object is less than pData, 
	// or > 0 if this GString object is greater than pData.
	//
	// so:
	// s = "MyFile.Exe";
	// s.Compare("YourFile.exe")		!=  0
	// s.Compare("myfile.exe")			!=  0
	// s.Compare("MyFile.exe")			==  0
	// s.CompareNoCase("myfile.exe")	==  0
	//
	// compare pData 'till it's null
	int Compare(const char *pData) const; 
	// compare pData 'for n bytes
	int Compare(const char *pData, int n) const; // compare pData for n bytes
	int Compare(GString &rData) const;
	// same thing as above case insensitive
	int CompareNoCase(const char *pData) const;
	int CompareNoCase(const char *pData, __int64 n) const;
	int CompareNoCase(const GString &) const;


	// compare a part of THIS's value to pData
	// nStartAtSelfIndex is the 0 based byte offset in this object that the comparison begins.
	// nSelfLen is the number of bytes to compare, -1 compares up to the null in THIS
	// so
	// to compare the last 4 bytes of THIS to pData
	// assert(s.Length() > 4) 
	// s.CompareSubNoCase(".exe",s.Length() - 4) == 0
	// --------------------------------------------------
	int CompareSub(const char *pData, __int64 nStartAtSelfIndex, __int64 nSelfLen = -1) const; 
	int CompareSubNoCase(const char *pData, __int64 nStartAtSelfIndex, __int64 nSelfLen = -1) const; 
	int CompareToFile(const char *pzFileAndPath, int nMatchCase=1);// default is exact case match

	// returns the length of the string
	inline __int64 Length() const { return _len; }
	inline __int64 GetLength() const { return _len; } // for CString interoperability
	inline __int64 size() const { return _len; } // for std::string interoperability

	// added in 2015 for CString interoperability, in many cases I can just repalce CString with GString.
	// several are inlined in GString.cpp to avoid #including the definition for strlen() in this .h file.
	char *GetBuffer(__int64 nMinLength)							{ SetPreAlloc(nMinLength); return _str; } 
	void ReleaseBuffer(__int64 nLength = -1 );				//	{ if (nLength == -1)  SetLength( strlen(_str) ); else SetLength(nLength);              }
	void Append(const char *pzValue, __int64 nLen = -1);	//	{ if (nLen > 0)	write(pzValue,nLen);  else 	write(pzValue,strlen(pzValue));	}


	// This was designed for CSimpleStringT interop,  
	// SetString is implemented as "append", so you must call Empty() first to make it "assign"
	void SetString(const char *pxSrc, int nBytes = -1);		//  { if ( nBytes < 0 ) write(pxSrc,strlen(pxSrc) ); else write(pxSrc,nBytes );		}

	// returns the number of [chCharToCount] in 'this'
	__int64 OccurrenceCount(char chCharToCount);
	//__int64 OccurrenceCount(const char *pzSubStringToCount, bool bMatchCase=1);


	// a fast 'truncate' of the string length
	//
	//	 if (s.CompareSubNoCase(".exe",s.Length() - 4) == 0)// <--if the last 4 bytes are ".exe"
	//		s.SetLength( s.Length() - 4 );					// <--remove them from the string
	//
	//	  if (s.GetAt(s.Length()-1) == '\\' )				// <-- If the last byte of the path is a slash
	//	    s.SetLength(s.Length() - 1);					// <-- shorten the string by 1 byte
	inline void SetLength(__int64 nLen) { if (nLen > -1 && nLen < _max) {_len = nLen; _str[_len] = 0;} } 

	// There is no Max length to a GString.  
	// This is the Maximum length that the string may grow until the next memory allocation
	// or said differently, it is the heap/stack allocation size of _str
	inline __int64 GetMaxLength() const { return _max; }

	// Reset() will empty the GString, and release any heap allocation it has made.
	// Only in a rare situation would you want to Reset(), that would be when you no longer want the
	// contents of the GString, but you are not going to destroy the GString object (suppose you are pooling GString Objects)
	// Empty() has the same effect, but does not release the buffers, so if you want to set a GString
	// to 0 length that will keep being reused - Empty() it so that it does not need to make new heap 
	// allocations next time you grow beyond DEFAULT_INITIAL_SIZE bytes.
	void Reset();
	
	// SetPreAlloc() sets the _max length obtained by GetMaxLength(), the contents of the string 
	// are unchanged and memory is pre-allocated for the amount of bytes specified.  The amount
	// specified may only grow the allocation - not reduce it, if you specify a value less than
	// current GetMaxLength() in your call to SetPreAlloc() the call is ignored.
	// SetPreAlloc() is the same as the "GString(nInitialSize)" 'ctor except that it works on pre-constructed GStrings
	void SetPreAlloc(__int64);
	// same as above, but it sets the additional allocated space desired
	void GrowPreAlloc(__int64 l){SetPreAlloc(_len+l);};


	// escapes <>&"\ in 'this' with &#ascii-number;
	void EscapeXMLReserved();
	
	// appends pzSource onto 'this', escaping <>&"\ and XML invalid chars to make valid XML
	// nLen = # bytes to append from pzSource - you must supply this value if pzSource is NOT null terminated otherwise use -1 to append until a null.
	// nSerializeFlags allows you to fully control what data gets escaped.
	void AppendEscapeXMLReserved(const char *pzSource, __int64 nLen = -1, int nSerializeFlags = 0);


	// returns a character pointer to the string�s data.
	inline operator const char *() const { return _str; }
	

	// same as a (const char *) cast above - easier to use through a pointer ->
	// turns this [ (const char *)(*myStr) ] into [ myStr->StrVal() ]
	inline const char *StrVal() const { return _str; }



	// given s == "10.9.8.7.7.7." 
	// s.StartingAt(7) == "7.7.7"
	// The string is not trimmed, just indexed. s still == "10.9.8.7.7.7"
	// same as Buf() with error checking and const return
	inline const char *StartingAt( __int64 n0BasedIndex ) const {return (n0BasedIndex < _len) ? &_str[n0BasedIndex] : 0;}

	// Buf() gets the 'non const' buffer for this GString without any bounds checking
	// Use StartingAt() if you want the results in non-const form.
	// Use StartingAt() if you want bounds checking.
	// Otherwise use this, faster, option via Buf()
	//
	// - if you modify the results
	//     1. MAKE SURE YOU DO NOT OVERWRITE THE BOUNDS 
	//     2. MAKE SURE YOU SET THE LENGTH AFTER MODIFYING THE STRING
	//     3. Don't index beyond  GetLength()
	//     Example:
	//     Assert(S.GetMaxLength() > strlen("Hello World"))
	//     strcpy(S.Buf(),"Hello World");
	//     S.SetLength(strlen("Hello World"));
	// - you may also want to use Buf() to avoid type casting off the 'const' for arguments to other function calls.
	char *Buf(__int64 nIndex = 0) { return &_str[nIndex]; }


	// returns true if the string is empty
	bool IsEmpty() const { return _len == 0; }

	// Makes this GString object an empty string.
	void Empty() { _len = 0; _str[_len] = 0; }

	// Load an error from the active ErrorProfile() owned by GException.cpp
	void LoadResource(const char* szSystem, int error, ...);

	// overloaded subscript ([]) operator returns a single character 
	// specified by the zero-based 32 or 64 BIT nIndex. 
	char& operator[](int nIdx);
	char& operator[](__int64 nIdx);
	// Note:			   char ch = s[0];			uses the 32 bit index in VC6 and other 32 bit builds, uses the 64 bit index in 64 bit builds.
	//	  or use 		   char ch = s.GetAt(0);	which will widen nIdx automatically
	// const char *p1 = &s[0];
	// const char *p2 = s.Buf();   <------- p1 == p2 : Using the p2 approach is easier to read.
	// const char *p3 = s._str;   <------- p1 == p2 == p3 : This is OK too - but dont tell anyone.
	//
	// Additional Note: operator[] provides a 32 and 64 bit overload because unlike class "methods" which NEED only a 64 bit interface
	//					to support 32 and 64 bit arguments,  "operator overloads" will NOT implicitly convert 32 bit arguments to 64
	//
	// By designing XMLFoundation interfaces for a 64 bit application, consequently it causes 32 bit applications to push an extra (unused) 4 bytes
	// onto the callstack every time we pass a 32 bit integer argument ----> UNLESS 32 bit overloads are defined.<-----------
	// 
	// In 2010 alone, producers of chips based on ARM architectures reported shipments of 6.1 billion ARM-based processors.
	// It is the most widely used 32-bit instruction set architecture in terms of quantity produced, 
	// justifying perhaps more 32 bit GString optimizations in the future



	char GetAt(__int64 nIdx) const;
	// The SetAt member function overwrites a single character 
	// specified by an zero based index number. 
	void SetAt(__int64 nIdx, char ch);

	// Pads the string object on the left with ch to make the string
	// a minimum of nCnt character's in length.  s="123" s.PadLeft(5) s = "00123"
	void PadLeft(__int64, char ch = ' ');


	// Removes the character ch from the left side of the string.
	void TrimLeft(char ch = ' ', short nCnt = -1);
	// trim leading white space (tabs, CR, LF, spaces) from the left(beginning) of the string
	void TrimLeftWS(); 
	// removes nCnt bytes from the beginning of the string
	void TrimLeftBytes(__int64 nCnt);
	// Pads the string object on the right with ch to make the string
	// a minimum of n character's in length  s="123" s.PadRight(5) s = "12300"
	void PadRight(__int64 nCnt, char ch = ' ');
	// Pads the string on the right with nCnt of ch
	void Append(__int64 nCnt, char ch = ' ');
	// Removes the character ch from the right side of the string. (max nCnt)
	void TrimRight(char ch = ' ', short nCnt = -1);
	// Shortens the sting by nCnt bytes
	void TrimRightBytes(__int64 nCnt);
	// trim White Space from the end of the string 
	// removes trailing tabs, spaces, carriage returns, new lines
	void TrimRightWS(); 

	// Extracts the first (that is, leftmost) nCount characters from 
	// this GString object and returns a copy of the extracted substring. 
	// If nCount exceeds the string length, then the entire string is extracted. 
	GString Left (__int64 nCnt) const;
	// Extracts a substring of length nCount characters from this GString object, 
	// starting at position nFirst (zero-based). 
	GString Mid  (__int64 nFirst) const;
	GString Mid  (__int64 nFirst, __int64 nCnt) const;
	GString substr(__int64 nFirst, __int64 nCnt) const{return Mid(nFirst, nCnt); }; // for std::string interoperability


	// Extracts the last (that is, rightmost) nCount characters from this GString 
	// object and returns a copy of the extracted substring. If nCount exceeds the
	// string length, then the entire string is extracted. 
	GString Right(__int64 nCnt) const;

	// Converts this GString object to an uppercase string.
	// Converts entire string by default or specify begin index
	void MakeUpper(__int64 nBeginIndex=0);
	// Converts this GString object to a lowercase string.
	void MakeLower(__int64 nBeginIndex=0);
	// Reverses the characters in the GString
	void MakeReverse();

	// search the string starting at 0 based index nStart
	// return -1 if not found otherwise a 0 based index of the found data
	__int64 FindCaseInsensitive( const char *lpszSub, __int64 nStart = 0 ) const; 
	__int64 Find( const char *pstr, __int64 nStart = 0 ) const;
	__int64 Find( char ch, __int64 nStart = 0 ) const;
	
	// return -1 when Nth occurrence is not found, otherwise index to the Nth occurrence of pstr
	// Nth is a 1 based index, so Nth=1 is the first occurrence, Nth=2 is the second, and so on.
	// Note: if you needed to split folder paths from Folder/Paths/To/File.txt
	// Consider using a GStringList like this: GStringList l("/","Folder/Paths/To/File.txt")
	// FindNth() is better suited for something that is always (before/at/after) the Nth occurrence
	__int64 FindNth( const char *pstr, int Nth, __int64 nStart = 0 ) const;
	__int64 FindNth( char ch, int Nth, __int64 nStart = 0 ) const;

	// return -1 if none of the [pzCharsToSearchFor] were found in 'this' 
	// otherwise returns the index of the first char in 'this' that is in the [pzCharsToSearchFor]
	__int64 FindOneOf(const char *pzCharsToSearchFor) const;

	// return a pointer into 'this' after the matched find
	// s="one=hello"     s2=FindStringAfter("one=")   s2 == "hello"
	// same as Find() but returns data rather than an index, and advances it past the match token - a common task
	const char *FindStringAfter(const char *pSearchFor) const;

	// a dynamic SubStr()
	// s="aaaBBBccc"    s2 = s.FindStringBetween("aaa","ccc")     s2 == "bbb"
	GString FindStringBetween(const char *pSearchForBegin, const char *pSearchForEnd) const;

	// All of the other Find* methods assume that this's string content is a null terminated string
	// This find searches this's contents for a binary match to the data in [strToFind] that may also be binary
	// nulls in either this or [strToFind] will not terminate the search.  Like the other finds, this returns 
	// the index that [strToFind] begins in [this] or -1 if it was not found, like the other find*'s [nStart]
	// is the 0 based index in [this] that the search begins at.
	__int64 FindBinary(GString &strToFind, __int64 nStart = 0);
	// nToFindLen is measured in bytes
	__int64 FindBinary(void *pBinary, int nToFindLen, __int64 nStart = 0);


	// search starting from the end of the string, 
	// nStart is a 0 based index of the starting place to search, by default nStart is _len (the last byte of the string)
	// the search just works toward the front - index 0.
	// if bMatchCase = 1, the ONLY EXacT Case matches will be found
	// returns -1, if [ToFind] is not found otherwise a 0 based index where it was found
	// 
	// example to obtain the extension from a file name if it has one
	// int nDot = strFile.ReverseFind(".");
	// GString strExtension( (nDot == -1) ? "" : strFile.StartingAt( nDot + 1 ) );
	__int64 ReverseFind( char chToFind ) const;
	__int64 ReverseFind( char chToFind, __int64 nStart ) const;
	__int64 ReverseFind( const char *pzToFind, __int64 nStart = -1, int bMatchCase = 0 ) const;
	// s = "c:\The\Path\Folder1\Folder2\File.exe"
	// example: s.SetLength(s.ReverseFindNth("\\",2));
	// s = "c:\The\Path\Folder1"
	__int64 ReverseFindNth( const char *pzToFind, int Nth) const;
	
	// starting from the end of the string, return the index of the first char in this that matches any char in pzToFind
	// s = "/usr/bin/file.ext" || s = "c:\the\path\file.ext"
	// s.SetLength(s.ReverseFindOneOf("/\\")); // cut off the file name for either a windows or unix path
	__int64 ReverseFindOneOf( const char *pzToFind ) const;
	__int64 ReverseFindOneOf( const char *pzToFind, __int64 nStart  ) const;

	// assign the value of 'this' from pFrom upto the first match of pzUpToTerm in pFrom
	// example to get only the first 'line' from pFrom into 'this':   str.SetFromUpTo(pFrom,"\r\n");
	// returns index into pFrom, where first of pzTerm was found or -1 if no termination was found
	// s.SetFromUpto("one|two","|");
	// s == "one"
	//
	// strURL = "http://www.abc.com/path/file.txt"
	// The 7th byte follows "http://" in the URL. The IP or server name is an unknown length, everything upto the first slash (or pipe)
	// strDomain.SetFromUpToFirstOf( strURL.StartingAt(nHTTPIndex + 7), "\\/|" );  
	// strDomain == www.abc.com
	__int64 SetFromUpTo(const char *pFrom, const char *pzUpToTerm);
	__int64 AppendFromUpTo(const char *pFrom, const char *pzUpToTerm); // appends to 'this'
	// same as above but pFrom terminates at the first occurrence of any character found in pzUpToTerm
	__int64 SetFromUpToFirstOf(const char *pFrom, const char *pzUpToTerm);
	__int64 AppendFromUpToFirstOf(const char *pFrom, const char *pzUpToTerm);


	// insert the the given value into 'this' at the given index.
	// s = "Rold"  s.Insert(2,"ck the Wor")  s == "Rock the World"
	// if you know the length of [str] or if it contains nulls as data pass the length of it as [nStrLen] otherwise [str] will be inserted up to the first NULL
	void Insert( __int64 nIndex, char ch );
	void Insert( __int64 nIndex, const char *str, __int64 nStrLen = -1 );
	
	// search 'this' string for [pzMatch], and insert [pzInsertThis] before [pzMatch] for InsertBefore() or after [pzMatch] for InsertAfter()
	// if bMatchCase is set to 1, then ONLY ExAcT case matches for [pzMatch] will be used.
	// returns the index that [pzInsertThis] was inserted or -1 if [pzMatch] was not found.
	__int64 InsertBefore( const char *pzMatch, const char *pzInsertThis, int bMatchCase = 0);
	__int64 InsertAfter( const char *pzMatch, const char *pzInsertThis, int bMatchCase = 0);

	// remove [nLen] bytes starting at index [nStart]
	void Remove ( __int64 nStart, __int64 nLen );
	// remove all occurrences of [ch], return count removed
	__int64 RemoveAll ( char ch );
	// remove all occurrences of [pStrToRemove], if [bMatchCase] is 1, then only EXaCT case matched are removed.  returns count removed.
	__int64 RemoveAll ( const char *pStrToRemove, int bMatchCase = 0 );
	// remove first occurrence of [ch], return 1 if removed otherwise 0
	int RemoveFirst ( char ch );
	// remove first occurrence of [pStrToRemove], if [bMatchCase] is 1, then only EXaCT case matched are removed. return 1 if removed otherwise 0
	int RemoveFirst ( const char *pStrToRemove, int bMatchCase = 0 );
	// remove last occurrence of [pStrToRemove], if [bMatchCase] is 1, then only EXaCT case matched are removed,  return 1 if removed otherwise 0
	int RemoveLast ( const char *pStrToRemove, int bMatchCase = 0 );
	// remove last occurrence of [ch], return 1 if removed otherwise 0
	int RemoveLast ( char ch );


	// Replace ALL occurrences of *What with *ReplaceWith, unless nFirstOccuranceOnly = 1, then only replace the first occurance
	void Replace( char chWhat, char chReplaceWith, int nFirstOccuranceOnly = 0 );
	void Replace( const char * szWhat, char chReplaceWith, int nFirstOccuranceOnly = 0  );
	void Replace( const char * szWhat, const char *szReplaceWith, int nFirstOccuranceOnly = 0 );
	void ReplaceCaseInsensitive( const char * szWhat, const char *szReplaceWith, __int64 nStart = 0, int nFirstOccuranceOnly = 0 );
	void Replace( char chWhat, const char *szReplaceWith, int nFirstOccuranceOnly = 0 );

	// within "this" object - replace all occurrences of each character in [pzAnyOfThese] with [chBecomesThis]
	// s = "717273"
	// s.ReplaceChars("123",'_')
	// s == "7_7_7_"
	void ReplaceChars(const char *pzAnyOfThese, char chBecomesThis);


	// overwrite the value of 'this' with pzNew beginning at nIndex
	// if nLength is -1, the entire pzNew will be written into 'this'
	// otherwise only nLength bytes from pzNew will be written.
	void OverWrite(const char *pzNew, __int64 nIndex = 0, __int64 nLength = -1);

	// replace each char in pzReplaceChars with %nn where
	// nn is a two byte hex value of the byte that was replaced.
	// example: GString s("A\nC");  
	//			s.EscapeWithHex("\r\n\t");
	//			s == "A%0AC"
	//
	const char *EscapeWithHex(const char *pzEscapeChars);
	
	// replace each %nn hex code with the ascii character it represents
	// s="A%0AC"    s.UnEscapeHexed()    s == "A\nC"
	void UnEscapeHexed();
	// exactly like UnEscapeHexed() but also converts '+' to ' ' (pluses to spaces)
	void UnEscapeURL();

	// Pads the string on the left with nCnt of ch
	void Prepend(__int64 nCnt, char ch );
	// Copy nBytes from pSrc or up to the NULL in pSrc if nBytes = -1 onto the beginning of 'this'
	void Prepend(const char *pSrc, int nBytes = -1);
	// Copy strSource onto the beginning of 'this'
	void Prepend(GString &strSource);
	// Copy the character value from native integers onto the beginning of 'this'
	void Prepend(__int64 n);


	// Generate a hashKey from the data in 'this' and put it in [pHashDest]
	// [pzHashName] can be "sha256, "sha512", or any one of the many hashes that OpenSSL supports
	//     and that have been compiled into the OpenSSL build being used.  For a list of supported
	//     hash digests - you can call HashKeyAlgorithms(), additionally [pzHashName] can be 
	//     "RotatingHash" = 32 bit non-cryptographic hash is extremely fast with low CPU overhead to hash 
	//      strings to indexes used for memory arrays.
	// IF THE HASH DIGEST > 64 bytes (which is 512bits), YOU MUST GString::PreAlloc() [pHashDest] 
	//     accordingly prior to calling GString::HashKey()
	bool HashKey(const char *pzHashName, GString* pHashDest);
	
	// supply either [pListDest] or [pStrDest] or both.  If both values are null HashKeyAlgorithms() does nothing.
	// Look at the implementation in GString.cpp to see the list of supported algorithms returned in OpenSSL 3.0 (Current as of July 2024)
//	static void HashKeyAlgorithms(GStringList* pListDest, GString* pStrDest);

	// --------------------------------------------------------------------------------------------
	// Compression and Cipher Notes:
	// Compression uses GZip, see GZip.cpp
	// Cipher uses a 256bit SHA-2 hash of [strKey] to key a TwoFish 128bit block cipher
	// --------------------------------------------------------------------------------------------
	// pSrc contains the unciphered data
	// strKey is the cipher password
	// 'this' contains the ciphered results when Cipher() returns 1
	bool Cipher( GString &strKey, GString *pSrc );
	// This Cipher() assumes 'this' contains the unciphered source
	// unciphered data in 'this' will be replaced with the ciphered version of that data.
	bool Cipher( GString &strKey );
	bool Cipher( const char *pzKey );
	// pSrc contains the ciphered data
	// strKey is the cipher password
	// 'this' contains the unciphered results when Cipher() returns 1
	bool DeCipher( GString &strKey, GString *pSrc );
	// This DeCipher() assumes 'this' contains the ciphered source - possibly loaded with FromFile()
	//  ciphered data in 'this' will be replaced with the unciphered version of that data.
	bool DeCipher( GString &strKey );
	bool DeCipher( const char *pzKey );
	// pSrc is the uncompressed GString
	// 'this' contains the compressed results when Compress() returns 1
	bool Compress( GString *pSrc );
	// this Compress() assumes that the current contents of 'this' are the uncompressed data
	// the source will be replaced with the compressed form of the data in 'this'
	bool Compress( );
	// Supposing you used ToFile() to write a GString compressed file, use this to decompress the file.
	// nDecompressedSize = The destination size that you expect after it is unzipped, 
	// if the nDecompressedSize value is too small DeCompress() will fail, if the value is too big it wastes memory.
	bool DeCompress( const char *pFileName, __int64 nDecompressedSize = -1);
	// pSrc is the compressed data in a GString
	// 'this' contains the uncompressed results when DeCompress() returns 1
	// nDecompressedSize = The destination size that you expect after it is unzipped, 
	// if the nDecompressedSize value is too small DeCompress() will fail, if the value is too big it wastes memory.
	bool DeCompress( GString *pSrc, __int64 nDecompressedSize = -1);
	// this DeCompress() assumes that the current contents of 'this' are the compressed data, perhaps assigned by FromFile()
	// the compressed source will be replaced with the uncompressed form of the data in 'this'
	// nDecompressedSize = The destination size that you expect after it is unzipped, 
	// if the nDecompressedSize value is too small DeCompress() will fail, if the value is too big it wastes memory.
	bool DeCompress(__int64 nDecompressedSize = -1);

	// --------------------------------------------------------------------------------------------
	// Standard Base16 conversions.
	const char *UUEncode();
	const char *UUDecode();

	// x-path
	void NormalizeSpace();
	void Translate(const char *szConvert, const char *szTo);
	
	// unlike the append operators(+= and <<), that expand packed numeric to alpha numeric, these
	// leave the packed numeric in binary form stored in network byte order.  If you are writing an
	// application that crosses BigEndian and LittleEndian platforms, this will ensure proper binary conversion.
	// example:
	// __int64 n = 18446744073709551615
	// s1 << n;								// length of s1 is 20
	// s2.AppendPackedInteger(n);			// length of s2 is 8		  (packed with an intel or sparc processor)
	// __int64 n2 = s2.GetPacked64(0);		// n2 == 18446744073709551615 (unpacked with intel or sparc processor)
	void AppendPackedInteger(unsigned short);
	void AppendPackedInteger(unsigned long);
	void AppendPackedInteger(unsigned __int64);
	unsigned short GetPackedShort(__int64 index);  // index into 'this' GString that the PackedShort is stored
	unsigned long GetPackedLong(__int64 index);
	unsigned __int64 GetPacked64(__int64 index);


	// Treat this's string value as an Integer.
	////////////////////////////////////////////////////////////////////////////////
	// Return the integer value of the string
	__int64 Int() const;
	// if the GString value is numeric, change it's value.  For example
	// s = "700"; s.Inc(77); s == "777"
	void Inc(__int64 nAmount = 1);				// Increment
	void Dec(__int64 nAmount = 1);				// Decrement
	// same as above with an ASCII value. For example
	// s = "700"; s.Inc("77"); s == "777"
	void Inc(const char *pzAmount);			
	void Dec(const char *pzAmount);
	////////////////////////////////////////////////////////////////////////////////


	// assignment operators
	GString & operator=(const wchar_t *);
	GString & operator=(char);
	GString & operator=(unsigned __int64);
	GString & operator=(__int64);
	GString & operator=(const char *);
	GString & operator=(const signed char *);
	GString & operator=(unsigned char);
	GString & operator=(signed char);
	GString & operator=(short);
	GString & operator=(unsigned short);
	GString & operator=(int);
	GString & operator=(unsigned int);
	GString & operator=(long);
	GString & operator=(unsigned long);
	GString & operator=(float);
	GString & operator=(double);
	GString & operator=(long double);
	GString & operator=(const GString &);
//	GString & operator=(const GString0 &);
//	GString & operator=(const GString32 &);

	// append operators, enables a GString to mimic an ostream for simple porting to faster GStrings
	GString & operator<<(unsigned __int64);
	GString & operator<<(__int64);
	GString & operator<<(const char *);
	GString & operator<<(const signed char *);
	GString & operator<<(char);
	GString & operator<<(unsigned char);
	GString & operator<<(signed char);
	GString & operator<<(short);
	GString & operator<<(unsigned short);
	GString & operator<<(int);
	GString & operator<<(unsigned int);
	GString & operator<<(long);
	GString & operator<<(unsigned long);
	GString & operator<<(float);
	GString & operator<<(double);
	GString & operator<<(long double);
	GString & operator<<(const GString &);
//	GString & operator<<(const GString0 &);
//	GString & operator<<(const GString32 &);

	GString & operator+=(unsigned __int64);
	GString & operator+=(__int64);
	GString & operator+=(const signed char *);
	GString & operator+=(const char *);
	GString & operator+=(char);
	GString & operator+=(unsigned char);
	GString & operator+=(signed char);
	GString & operator+=(short);
	GString & operator+=(unsigned short);
	GString & operator+=(int);
	GString & operator+=(unsigned int);
	GString & operator+=(long);
	GString & operator+=(unsigned long);
	GString & operator+=(float);
	GString & operator+=(double);
	GString & operator+=(long double);
	GString & operator+=(const GString &);
//	GString & operator+=(const GString0 &);
//	GString & operator+=(const GString32 &);

	// friend functions for addition
	friend GString operator+(GString &, GString &);
	friend GString operator+(GString &, const char *);
	friend GString operator+(const char *, GString &);
	friend GString operator+(GString &, const signed char *);
	friend GString operator+(const signed char *, GString &);
	friend GString operator+(GString &, char);
	friend GString operator+(char, GString &);
	friend GString operator+(GString &, unsigned char);
	friend GString operator+(unsigned char, GString &);
	friend GString operator+(GString &, signed char);
	friend GString operator+(signed char, GString &);

/*
	friend GString operator+(GString &, GString32 &);
	friend GString operator+(GString32 &, GString &);
	friend GString operator+(GString0 &, GString &);
	friend GString operator+(GString &, GString0 &);

	friend GString operator+(GString32 &, const char *);
	friend GString operator+(GString32 &_p1, GString32 &_p2);
	friend GString operator+(const char *_p1, GString32 &_p2);
	friend GString operator+(GString32 &_p1, const signed char *_p2);
	friend GString operator+(const signed char *_p1, GString32 &_p2);

	friend GString operator+(GString0 &_p1, const char *_p2);
	friend GString operator+(const char *_p1, GString0 &_p2);
	friend GString operator+(GString0 &_p1, const signed char *_p2);
	friend GString operator+(const signed char *_p1, GString0 &_p2);
*/

	// Comparative operators with other GString classes
	int operator >  (const GString &) const;
	int operator >= (const GString &) const;
	int operator == (const GString &) const;
	int operator <  (const GString &) const;
	int operator <= (const GString &) const;
	int operator != (const GString &) const;
/*
	// Comparative operators with other GString0 classes
	int operator >  (const GString0 &) const;
	int operator >= (const GString0 &) const;
	int operator == (const GString0 &) const;
	int operator <  (const GString0 &) const;
	int operator <= (const GString0 &) const;
	int operator != (const GString0 &) const;

	// Comparative operators with other GString32 classes
	int operator >  (const GString32 &) const;
	int operator >= (const GString32 &) const;
	int operator == (const GString32 &) const;
	int operator <  (const GString32 &) const;
	int operator <= (const GString32 &) const;
	int operator != (const GString32 &) const;
*/
	// Comparative operators with char *'s
	int operator >  (const char *) const;
	int operator >= (const char *) const;
	int operator == (const char *) const;
	int operator <  (const char *) const;
	int operator <= (const char *) const;
	int operator != (const char *) const;

	// these two methods convert from [char *] to [wchar_t *]
	// the first  - wide char operator overload - is conditionally compiled into only Unicode applications
	// the second - Unicode() is compiled into all builds
#ifdef 	_NATIVE_WCHAR_T_DEFINED 
	#ifdef _UNICODE
		operator const wchar_t * ();   // removed const 2024
	#endif
#else
	// wchar_t is sometimes defined as unsigned short *, which adds ambiguity and forces us to add type casts, therefore we cannot have an implicit Unicode conversion in all builds
#endif

	wchar_t *Unicode();
	// if nTerminateCount is -1 then pWideSource is NULL terminated otherwise it's the length of pWideSource
	void FromUnicode(const wchar_t *pWideSource, __int64 nTerminateCount = -1);

#ifdef _UNICODE
	GString & operator<<(const wchar_t *);
	GString & operator+=(const wchar_t *);
#endif
};




class XMLResultset : public GString
{
	friend class GlobalInit;  // with access to the private 'ctor - all other instances used 'Attached' memory
	XMLResultset() : GString() {}
public:
	// see the notes for this overloaded GString() 'ctor and for GString::Attach() to understand more about how the attached memory is managed.
	XMLResultset(const char* pzSource, __int64 nSourceBytes, __int64 nSourceMemorySize) : GString(pzSource, nSourceBytes, nSourceMemorySize, false) {};
};

// implemented in GString.cpp
class GlobalInit
{
public:
	GlobalInit();
	~GlobalInit();
};


// [g_pEmptyXMLResultSet] is found in GString.cpp - This is the one and only instance of the empty resultset.
// It is designed for .NET interop using "SafeHandles" which requires that an object will never be NULL.
// In many [XMLResultset] use cases we would have returned a null XMLResultset indicating that all the results were returned 
// in the supplied memory space and there was no need for an interface to obtain more data.  For that situation a SafeHandle to 
// [g_pEmptyXMLResultSet] is returned.  This special handle must never be deleted by the routine that deletes all other [XMLResultset] 
// instances because it is both allocated and deleted by [GlobalInit]
extern XMLResultset *g_pEmptyXMLResultSet;  



// The following function declarations are portable.  The need for portability is why they are here.
/////////////////////////////////////////////////////////////////////////////////////////////////////

// case insensitive string compare - finds [str2] inside [str1]
char * stristr(const char * str1, const char *str2); 

// ascii to Int64 - the (X) prevents link errors in OS's that already have  atoi64() defined
__int64 Xatoi64(const char* s);	

// __int64 to ascii (again the X keeps the function signature unique (so this code works anywhere) )
// note: returns [buf] with the ascii character value of [val]
// note: aka lltoa()
char *Xi64toa ( __int64 val, char *buf, int radix = 10);

// faster implementation - works ONLY for unsigned __int64 and radix 10(aka decimal) which can never have a negative value.
char *Xi64toa ( unsigned __int64 val, char *buf);


/////////////////////////////////////////////////////////////////////////////////////////////////////

extern const char* g_TABS;
extern const int MAX_TAB_WRITE;

// Gstrlen is a strlen that takes char * or unicode strings
//__int64 Gstrlen( const char *str );
__int64 Gstrlen(const wchar_t* str);
#ifdef _UNICODE
 wchar_t *DLAsciiToUnicode(const char *a, char *b);
#endif


#undef NEED_ICOMP

#ifdef WINCE
 #define NEED_ICOMP
#endif
#ifndef _WIN32
 #define NEED_ICOMP
#endif

#ifdef NEED_ICOMP
	int stricmp( const char *string1, const char *string2 );
	int strnicmp(const char *p1, const char *p2, unsigned int nMax);
#else
	#define strcasecmp	_stricmp
#endif






/*	The following comments only apply when using a GString for COM programming.

	NOTE 1:	The following code is for COM programming, it is intentionally NOT included
			in the GString class so that GString has no dependencies on COM datatypes or Windows.

	NOTE 2:	When building ATL com objects that use GString::ToFile*()
			If you call ToFile() you must remove _ATL_MIN_CRT from the preprocessor directives.

	NOTE 3:	You can #include "GStringCOM.h" for an inlined string implementation that can convert to and from BSTR
			or you can use the standard GString class along with these functions that you must copy
			into your application so that the XMLfoundation does not have any dependencies on COM
			datatypes or Windows kernel translation routines.

			void BSTR2GString(GString &pStr, BSTR bstr)
			{
				int nLen = SysStringLen(bstr);
				int nBytes = WideCharToMultiByte(CP_ACP, 0, bstr, nLen, NULL, NULL, NULL,NULL);
				pStr.SetPreAlloc(nBytes + 1);
				LPSTR lpsz = pStr.Buf();
				WideCharToMultiByte(CP_ACP, 0, bstr, nLen, lpsz, nBytes, NULL, NULL);
				pStr.SetLength(nBytes);
			}

			void GString2BSTR(GString &pStr, BSTR FAR* bstr)
			{
				int nStrLen = pStr.GetLength();
				int nLen = MultiByteToWideChar(CP_ACP, 0, pStr.Buf(),	nStrLen, NULL, NULL);
				::SysReAllocStringLen(bstr,NULL,nStrLen);
				MultiByteToWideChar(CP_ACP, 0, pStr.Buf(), nStrLen,*bstr, nLen);
			}
*/

#endif //_G_STRING_HH_

