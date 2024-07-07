using Cs;

namespace BinarySharp {
    public class Program {
        static void Main(string[] args) {
            string filename = @"ha.dat";
            Console.WriteLine("Acessing file: " + filename);
            //Console.WriteLine(FileSystem.ImportText(filename));
            FileSystem.ExportText(filename, "Hejka\njjflajljdl");
            Console.WriteLine("Acessing file again: " + filename);
            Console.WriteLine(FileSystem.ImportText(filename));
        }
    }
}