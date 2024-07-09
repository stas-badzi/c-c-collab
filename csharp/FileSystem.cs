using System.IO;

namespace Cs
{
    public class FileSystem // Impotowanie plików .txt
    {
        public static string ImportText(string str_file)
        {
            string line;
            string result = "";

            try
            {
                StreamReader sr = new StreamReader(str_file);
                line = sr.ReadLine();
                if (line != null) {
                    result = line;
                    line = sr.ReadLine();
                    while (line != null) {
                        result = $"{result}\n{line}";
                        line = sr.ReadLine();
                    }
                }
                sr.Close();
            }
            catch (Exception e)
            {
                Environment.FailFast($"Unhandled exception at Cs.FileSystem.ImportText (FileSystem.cs:15): " + e.Message);
            }
            
            return result;
        }
        
        public static void ExportText(string str_path, string str_content)
        {
            try
            {
                if (File.Exists(str_path)) File.Delete(str_path);
                
                using (StreamWriter sw = File.CreateText(str_path))
                {
                    sw.WriteLine(str_content);
                    sw.Close();
                }

            }
            catch (Exception e)
            {
                Environment.FailFast($"Unhandled exception at Cs.FileSystem.ExportText (FileSystem.cs:40): " + e.Message);
            }
        }
    }
}