using System.IO;
using System.Runtime.InteropServices;
namespace Cs
{
    public class FileSystem // Impotowanie plików .txt
    {
        // export
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_ImportText")]
        public static IntPtr ImportText(IntPtr file)
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
                Console.WriteLine($"Unhandled exception at Cs.FileSystem.ImportText (FileSystem.cs): " + e.Message);
            }
            result += '\n';
            IntPtr output = Marshal.StringToHGlobalUni(result);
            return output;
        }
        // export
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_ExportText")]
        public static void ExportText(IntPtr path, IntPtr content)
        {
            string str_path = Marshal.PtrToStringAuto(path);
            string str_content = Marshal.PtrToStringAuto(content);

            try
            {
                if (File.Exists(str_path))
                    File.Delete(str_path);
                
                using (StreamWriter sw = File.CreateText(str_path))
                {
                    sw.WriteLine(str_content);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine($"Unhandled exception at Cs.FileSystem.ExportText (FileImport.cs): " + e.Message);
            }
        }
    }
}