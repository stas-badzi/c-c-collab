using System;
using Cpp;
using Cs;
using Utility;

namespace BinarySharp {
    public class Program {
        string exePath = AppContext.BaseDirectory;
        static int Main(string[] args) {
            FileSystem.PlayWAV(Path.Combine([exePath,"..","sounds","shoot.wav"]), true);
            return 0;
        }
    }
}