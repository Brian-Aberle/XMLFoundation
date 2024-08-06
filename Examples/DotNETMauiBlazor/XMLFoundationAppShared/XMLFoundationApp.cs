using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Buffers;
using System.Data;
using System.Text.RegularExpressions;
using System.Reflection.Metadata;
using System.Security.Cryptography;
using static System.Collections.Specialized.BitVector32;


namespace XMLFoundation
{
    public class XMLFoundationApp : IDisposable
    {
        private XMLFAppSafeHandle _handle;
        public ResultsetSafeHandle _hEmptyResultset;

        public XMLFoundationApp()
        {
            try
            {
                _handle = XMLFAppWrapper.CreateAppHandle();
                _hEmptyResultset = new ResultsetSafeHandle();
            }
            catch (Exception e)
            {
               

            }
        }
        public void Dispose() => Dispose(true);

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (_handle != null && !_handle.IsInvalid)
                    _handle.Dispose();
            }
        }

        public string GetDllName() => XMLFAppWrapper.GetDllName();

        public int SetAppValue(string sSection, string sEntry, string sValue)
        {
            if (_handle == null || _handle.IsInvalid)
                return -1;

            return XMLFAppWrapper.SetAppValue(_handle, sSection, sEntry,  sValue);
        }
        public string GetAppValue(string sSection, string sEntry)
        {
            if (_handle == null || _handle.IsInvalid)
                return new string("");
            long BufSize = GGlobals.BufSize();

            if (XMLFAppWrapper.GetAppValue(_handle, sSection, sEntry, GGlobals.Buf(), ref BufSize) == 0)
            {
                return System.Text.Encoding.UTF8.GetString(GGlobals.Buf(), 0, (int)BufSize);
            }
            return new string("");
        }


        // The documentation exists in the C++ XMLObject.h file for information on the methods and their arguments
        public ResultsetSafeHandle ToXML(ref string strDestination, int nTabs = 0, string TagOverride = "", int nSerializeFlags = 128, string pzSubsetOfObjectByTagName = "", bool bAppend = false)
        {
            long BufSize = GGlobals.BufSize();
            ResultsetSafeHandle h = XMLFAppWrapper.ToXML(_handle,GGlobals._buf, ref BufSize, nTabs,TagOverride,nSerializeFlags,pzSubsetOfObjectByTagName,bAppend);
            strDestination = System.Text.Encoding.UTF8.GetString(GGlobals.Buf(), 0, (GGlobals.BufSize() > BufSize) ? (int)BufSize : GGlobals.BufSize());
            return h;
        }
        public void DeleteResultsetHandle(ResultsetSafeHandle handleRS)
        {
            XMLFAppWrapper.DeleteResultsetHandle(handleRS);
        }
        public int FromXML(string sXML)
        {
            return XMLFAppWrapper.FromXML(_handle, sXML);
        }


        public int InsertOrUpdateCustomer(ulong nID, string sName, string sColor, string sListItems, int nType)
        {
            if (_handle == null || _handle.IsInvalid)
                return -1;

            return XMLFAppWrapper.InsertOrUpdateCustomer(_handle, nID, sName, sColor, sListItems, nType); // 0 is success

        }
        public int InsertOrUpdateAddress(ulong nCustID, string sLine1, string sLine2, string sCity, string sState, string sZip, int nAddrType)
        {
            if (_handle == null || _handle.IsInvalid)
                return -1;
            return XMLFAppWrapper.InsertOrUpdateAddress(_handle, nCustID, sLine1, sLine2, sCity, sState, sZip, nAddrType);
        }




        public void SetCoreValue(string sSection, string sEntry, string sValue)
        {
            XMLFAppWrapper.SetCoreValue(sSection, sEntry, sValue);
        }
        public string GetCoreValue(string sSection, string sEntry)
        {
            long BufSize = GGlobals.BufSize();
            if (XMLFAppWrapper.GetCoreValue(GGlobals.Buf(), ref BufSize, sSection, sEntry) == 0)
            {
                return System.Text.Encoding.UTF8.GetString(GGlobals.Buf(), 0, (int)BufSize);
            }
            return new string("");
        }
        public string GetLogFileData()
        {
            long BufSize = GGlobals.BufSize();
            // The value of BufSize now is the size of GGlobals.Buf()
            int nRetVal = XMLFAppWrapper.GetLogFileData(GGlobals.Buf(), ref BufSize);
            if (nRetVal == 0)
            {
                // The value of BufSize now is the total length of the entire results which will not exceed the bounds
                // return the entire results in the return value string
                return System.Text.Encoding.UTF8.GetString(GGlobals.Buf(), 0, (int)BufSize);
            }
            return "";
        }

        public string GetAllCoreValues(bool bAsXML = false, string sSectionOnly = "")
        {
            long BufSize = GGlobals.BufSize();
            // The value of BufSize now is the size of GGlobals.Buf()
            ResultsetSafeHandle resultset = XMLFAppWrapper.GetAllCoreValues(GGlobals.Buf(), ref BufSize, bAsXML, sSectionOnly);
            // The value of BufSize now is the total length of the entire results
            if (BufSize < GGlobals.BufSize()) 
            {
                // return the entire results in the return value string
                return System.Text.Encoding.UTF8.GetString(GGlobals.Buf(), 0, (int)BufSize);
            }
            else  // currently unfinished logic:  the results were larger than our working buffer - this wont happen in the example application
            {
                // We can build a return string where the first piece is what we already have:
                String sResults = System.Text.Encoding.UTF8.GetString(GGlobals.Buf(), 0, (int)BufSize);
                long nStartingIndex = GGlobals.BufSize();
                long GGLobalsBufSize = GGlobals.BufSize();
                long TotalResultsSize = BufSize;

                // loop this
                {
                    // the resultset is actually a GString, we need to essentially substr() the next piece from it
                    BufSize = GGlobals.BufSize(); // set BufSize to the working memory space size
    //              sResults += resultset.indexAt(nStartingIndex, GGlobals.Buf(), 0, (int)BufSize)
                    // now BufSize is the size of the whole result set again - 
                }

                return sResults;
            }
        }
        public int SetAllCoreValues(string sConfig, bool bAsXML = false)
        {
            long BufSize = GGlobals.BufSize();
            return XMLFAppWrapper.SetAllCoreValues(sConfig, bAsXML);
        }

        // if return != 0 then check GetLastServerError()
        public int CoreCommand(string Cmd, ref string OutResults, string Arg1 = "", string Arg2 = "", string Arg3 = "")
        {
            // BufSize is always set by the caller to the allocated size of GGlobals().Buf prior to calling CoreCommand().
            long BufSize = GGlobals.BufSize();
            int nReturnVal = XMLFAppWrapper.CoreCommand(GGlobals.Buf(), ref BufSize, "user:password", Cmd, Arg1, Arg2, Arg3);

            // BufSize is also set by CoreCommand() to the length of the result data that was put into GGlobals().Buf after returning


            if (nReturnVal == 0) // 0 is success
            {
                // starting at position 0 of the array find the index of the first null which is the end of the string
                int nLen = Array.FindIndex(GGlobals.Buf(), 0, (x) => x == 0);
                if (BufSize != nLen)
                {
                    //  this is unexpected - unless the results intentionally contained nulls

                }


                // starting at position 0 of the array convert the 8 bit array bytes to 16 bit string bytes - stop at the null which is [nLen]
                OutResults = System.Text.Encoding.UTF8.GetString(GGlobals.Buf(), 0, nLen);
            }
    
            return nReturnVal;
        }

        public String GetLastServerError()
        {
            long BufSize = GGlobals.BufSize();
            XMLFAppWrapper.GetLastServerError(GGlobals.Buf(), ref BufSize);

            // starting at position 0 of the array find the index of the first null which is the end of the string
//          int nLen = Array.FindIndex(GGlobals.Buf(), 0, (x) => x == 0);

            // starting at position 0 of the array convert the 8 bit array bytes to 16 bit string bytes - stop at the null which is [nLen]
            String s =  System.Text.Encoding.UTF8.GetString(GGlobals.Buf(), 0, /*nLen*/ (int)BufSize);
            return s;
        }
        public String GetLastAppError()
        {
            long BufSize = GGlobals.BufSize();
            XMLFAppWrapper.GetLastAppError(_handle, GGlobals.Buf(), ref BufSize);

            // starting at position 0 of the array find the index of the first null which is the end of the string
            int nLen = Array.FindIndex(GGlobals.Buf(), 0, (x) => x == 0);
            // starting at position 0 of the array convert the 8 bit array bytes to 16 bit string bytes - stop at the null which is [nLen]
            String s = System.Text.Encoding.UTF8.GetString(GGlobals.Buf(), 0, nLen);
            return s;
        }

        // nSource = 1 for XMLElement or nSource = 0 for XMLAttribute
        public String GetMemberValue(string sOID, string sXMLObjectCPPClassName, string sTagName, int nSource)
        {
            long BufSize = GGlobals.BufSize();
            string searchKey = sOID + sXMLObjectCPPClassName;
            if (XMLFAppWrapper.GetMemberValue(GGlobals.Buf(), ref BufSize, searchKey, sTagName, nSource) == 1)
            {
                return System.Text.Encoding.UTF8.GetString(GGlobals.Buf(), 0, (int)BufSize);
            }
            return new string(""); // no instance [sOID] of [Class]
        }
        public int SetMemberValue(string sOID, string sXMLObjectCPPClassName, string sTagName, string sValue)
        {
            string searchKey = sOID + sXMLObjectCPPClassName;
            return XMLFAppWrapper.SetMemberValue(searchKey, sTagName, sValue);
        }



        public String GetAppResultString(int nType)
        {
            long BufSize = GGlobals.BufSize();
            int nReturnVal =  XMLFAppWrapper.GetAppResultString(_handle, GGlobals.Buf(), ref BufSize, nType);
            if (nReturnVal != 0)
            {   
                // error
                return new string("Error");
            }

            // Using FindIndex to obtain the string length is not necessary because [BufSize] has the length 
            int nLen = Array.FindIndex(GGlobals.Buf(), 0, (x) => x == 0);
            if (BufSize != nLen)
            {
                //  this is unexpected - unless the results are binary data that intentionally contain nulls
            }
            return System.Text.Encoding.UTF8.GetString(GGlobals.Buf(), 0, (int)BufSize );

        }


    }
}
