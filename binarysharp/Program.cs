using Cs;

namespace BinarySharp {
    public class Program {
        static int Main(string[] args) {
            List<string> file = new List<string>();
            file.Add("Badzi");
            FileSystem.ExportText("a.dat",file);
            //FileSystem.PlayMP3(@"C:\Users\micha\Desktop\goat.mp3");
            return 0;
        }
    }
}