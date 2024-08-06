using Microsoft.Win32.SafeHandles;
using System;


namespace XMLFoundation
{

    
    public class ResultsetSafeHandle : SafeHandleZeroOrMinusOneIsInvalid
    {
        public ResultsetSafeHandle() : base(true) { }

        public IntPtr Ptr => handle;

        protected override bool ReleaseHandle()
        {
            XMLFAppWrapper.DeleteResultsetHandle(this);
            return true;
        }
    }
  

    internal class XMLFAppSafeHandle : SafeHandleZeroOrMinusOneIsInvalid
    {
        public XMLFAppSafeHandle() : base(true) { }

        public IntPtr Ptr => handle;

        protected override bool ReleaseHandle()
        {
            XMLFoundation.XMLFAppWrapper.DeleteAppHandle(this);
            return true;
        }
    }
}


