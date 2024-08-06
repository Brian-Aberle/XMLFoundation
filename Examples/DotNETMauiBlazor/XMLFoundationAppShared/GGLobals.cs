using System;
using System.Buffers;
using System.Collections.Generic;
using System.Text;

namespace XMLFoundation
{
    public class GGlobals
    {
        public static XMLFoundationApp XMLApp = new();

        // note: byte[] is special - unlike char[] or other types, a byte[] array is not marshalled.
        // The contents of the array are not copied between the managed C# and unmamaged C++
        private static int _bufSize = 32768;
        public static byte[] _buf = ArrayPool<byte>.Shared.Rent(_bufSize);
        public static byte[] Buf() => _buf;
        public static int BufSize() { return _bufSize; }

        GGlobals() { }
    }
}