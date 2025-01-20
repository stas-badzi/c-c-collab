using System.Security.Cryptography;
using System;
using Cpp;
using Cs;
using Utility;

namespace BinarySharp {
    public class Program {
        static int Main(string[] args) {
            SoundSystem.PlaySound(Path.Combine([AppContext.BaseDirectory,"..","sounds","shoot.wav"]), true);
            return 0;
        }
    }
}