using System;
using Cpp;
using Cs;
using Utility;

namespace BinarySharp {
    public class Program {
        static int Main(string[] args) {
            FileSystem.PlayWAV(@"c:\Windows\Media\chimes.wav", true);
            return 0;
        }
    }
}