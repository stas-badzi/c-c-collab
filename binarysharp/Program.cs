using Cpp;
using Cs;
using CSCore;
using CSCore.Codecs;
using CSCore.SoundOut;
using Utility;

namespace BinarySharp {
    public class Program {
        static int Main(string[] args) {
            Cs.FileSystem.PlayMP3("boom.mp3");
            return 0;
        }
    }
}