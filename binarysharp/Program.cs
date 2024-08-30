using Cpp;
using Cs;
using Utility;

namespace BinarySharp {
    public class Program {
        static int Main(string[] args) {
            Cs.FileSystem.PlayWAV("boom.mp3", true);
            return 0;
        }
    }
}