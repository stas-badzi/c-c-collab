using System;
using System.IO;
using System.Collections.Generic;
using System.CommandLine;
using Cpp;

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
        
        public static List<List<Terminal.Symbol>> TextureFromFile(int width, int height, string filename)
        {
            var fileImported = ImportText(filename); // Imported List<string>
            var file = new List<List<char>>(); // Every character of file
            var symbols = new List<List<Terminal.Symbol>>(); // Final symbol list

            for (int i = 0; i < fileImported.Count; i++) // Assign var file
            {
                file.Add(ToCharList(fileImported[i]));
            }

            int remainingSymbols = width * 3 * height;
            for (int i = 0; i < height; i++)
            {
                
                var symbolLine = new List<Terminal.Symbol>();

                for (int j = 0; j < width * 3; j+=3)
                {
                    symbolLine.Add(new Terminal.Symbol(file[i][j], HexToByte(file[i][j+1]), HexToByte (file[i][j+2])));
                }
                symbols.Add(symbolLine);
            }
            

            return symbols;
        }

        public static List<char> ToCharList(string input)
        {
            var array = input.ToCharArray();
            var list = new List<char>();
            for (int i  = 0; i < array.Length; i++)
            {
                if (array[i] != '\n') // DOESN'T ADD NEW LINE CHARACTERS (Correct if wrong)
                    list.Add(array[i]);
                else
                    break;
            }

            return list;
        }
    }
}