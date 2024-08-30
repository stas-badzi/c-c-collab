using System;
using Cpp;
using Cs;
using Utility;

namespace BinarySharp {
    public class Program {
        static int Main(string[] args) {
            FileSystem.PlayWAV(@"C:\Users\micha\Documents\GitHub\c-c-collab\boom.wav", true);
            
            return 0;
        }
    }
}