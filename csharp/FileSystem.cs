using System.IO;
using System.Runtime.InteropServices;
namespace Cs
{
    public class FileSystem // Impotowanie plików .txt
    {
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_ImportText")] // Eksport do binarysharp
        public static IntPtr ImportText(IntPtr file) // Funkcja
        {
            string str_file = Marshal.PtrToStringAuto(file);
            string line;
            string result = "";

            try
            {
                StreamReader sr = new StreamReader(str_file);
                line = sr.ReadLine();
                result = line;
                while (line != null)
                {
                    line = sr.ReadLine();
                    result = $"{result}\n{line}";
                }
            }
            catch (Exception e)
            {
                Console.WriteLine($"Unhandled exception at Cs.FileImport.ImportText (FileImport.cs): " + e.Message);
            }
            result += '\n';
            IntPtr output = Marshal.StringToHGlobalUni(result);
            return output;
        }

    }
}