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

            Cpp.Console.Symbol a_sym = new Cpp.Console.Symbol('A');
            short width = Cpp.Console.GetWindowWidth();
            short height = Cpp.Console.GetWindowHeight();
            List<List<Cpp.Console.Symbol>> smbls = new List<List<Cpp.Console.Symbol>>();
            for (int i = 0; i < height; i++)
            {
                smbls.Add(new List<Cpp.Console.Symbol>());
                for (int j = 0; j < width; j++)
                {
                    smbls[i].Add(a_sym);
                }
            }

            Cpp.Console.FillScreen(smbls);
        }
    }
}