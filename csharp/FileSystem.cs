using System.IO;

namespace Cs
{
    public class FileSystem // Impotowanie plików .txt
    {
        public static List<string> ImportText(string str_file)
        {
            string line;
            List<String> result = new List<String>();

            try
            {
                StreamReader sr = new StreamReader(str_file);
                line = sr.ReadLine();
                while (line != null) {
                    result.Add(line);
                    line = sr.ReadLine();
                }
                sr.Close();
            }
            catch (Exception e)
            {
                Environment.FailFast($"Unhandled exception at Cs.FileSystem.ImportText (FileSystem.cs:12): " + e.Message);
            }
            
            return result;
        }
        
        public static void ExportText(string str_path, List<string> str_content)
        {
            try
            {
                if (File.Exists(str_path)) File.Delete(str_path);
                
                using (StreamWriter sw = File.CreateText(str_path))
                {
                    for (int i = 0; i < str_content.Count(); ++i) {
                        sw.WriteLine(str_content[i]);
                    }
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