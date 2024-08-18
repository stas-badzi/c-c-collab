using Cpp;
using Cs;
using Utility;

namespace BinarySharp {
    public class Program {
        static int Main(string[] args) {
            List<bool> list = [true, false, false, false, true];
            Console.WriteLine(FileSystem.Add(2149312222,255));
            Console.WriteLine(FileSystem.DoSomething(true, '#'));
            Console.WriteLine(FileSystem.DoSomething(false, '#'));
            Console.WriteLine(FileSystem.DoSomeThings(list, "abcde"));
            Console.WriteLine(FileSystem.DoSomeThings(list, "abcdefg"));
            Console.WriteLine(FileSystem.DoSomeThings(list, "defg"));
            Console.WriteLine(FileSystem.DoMoreThings(["asudhfaHDHLFjdadjdjdjdjddjdjdjd8942075755757575757757575757757777777777777777777777777777777777777777jddddddddddddddddddddddddddddddddddddddddddddddak"])[0]);
            FileSystem.PlayMP3("/home/stas/Documents/c-c-collab/binarysharp/bin/exe/boom.mp3");
            return 0;
        }
    }
}