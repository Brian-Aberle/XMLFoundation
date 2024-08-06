/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Jan 05 07:46:02 2016
 */
/* Compiler settings for C:\XMLFoundation\Examples\C++\ExATLCOM\ExATLCOM.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IMyATLObj = {0x49B53179,0xA937,0x4560,{0xA0,0x06,0x64,0x14,0x2A,0x62,0xED,0x4F}};


const IID LIBID_EXATLCOMLib = {0x945B8ABD,0x7BAD,0x4C02,{0x9B,0xDF,0x25,0x64,0x1F,0x84,0xC2,0x79}};


const CLSID CLSID_MyATLObj = {0x7373BC84,0x8181,0x47CC,{0xB4,0xCF,0x53,0x2D,0x55,0xE4,0xB8,0x28}};


#ifdef __cplusplus
}
#endif

