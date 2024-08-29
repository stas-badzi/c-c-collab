using Cpp;
using Cs;
using CSCore;
using CSCore.Codecs;
using CSCore.SoundOut;
using Utility;

namespace BinarySharp {
    public class Program {
        static int Main(string[] args) {
            Cs.FileSystem.PlayWAV("boom.mp3", true);
            return 0;
        }
    }
}