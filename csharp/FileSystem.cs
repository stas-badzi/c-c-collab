using System;
using System.IO;
using System.Collections.Generic;
using System.CommandLine;
//using Microsoft.VisualBasic.FileIO;
using Cpp;
using Microsoft.VisualBasic.FileIO;

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
        
        public static List<List<Terminal.Symbol>>? TextureFromFile(string filename)
        {
            int width, height;

            var fileImported = ImportText(filename); // Imported List<string>
            var file = new List<List<char>>(); // Every character of file
            var symbols = new List<List<Terminal.Symbol>>(); // Final symbol list

            if (!int.TryParse(fileImported[0], out width))
            {
                return null;
            }
            if (!int.TryParse(fileImported[1], out height))
            {
                return null;
            }

            for (int i = 2; i < fileImported.Count; i++) // Assign var file
            {
                file.Add(ToCharList(fileImported[i]));
            }

            int remainingSymbols = width * 3 * height;
            for (int i = 0; i < height; i++)
            {
                
                var symbolLine = new List<Terminal.Symbol>();

                for (int j = 0; j < width * 3; j+=3)
                {
                    symbolLine.Add(new Terminal.Symbol(file[i][j], HexToByte(file[i][j+1]), HexToByte(file[i][j+2])));
                }
                symbols.Add(symbolLine);
            }
            

            return symbols;
        }

        public static void FileFromTexture(string filepath, List<List<Terminal.Symbol>> texture, bool recycle = false)
        {
            var file = new FileInfo(filepath);
            int width, height;

            if (file.Exists) // Move to recycle bin if exists (safety issues)
            Microsoft.VisualBasic.FileIO.FileSystem.DeleteFile(file.FullName, UIOption.OnlyErrorDialogs, recycle?RecycleOption.SendToRecycleBin:RecycleOption.DeletePermanently);
            file.Create();

            try
            {
                using (StreamWriter sw = file.CreateText())
                {
                    width = texture[0].Count;
                    sw.WriteLine(width);

                    height = texture.Count;
                    sw.WriteLine(height);

                    for (int i = 0; i < height; i++)
                    {
                        for (int j = 0; j < texture[i].Count; j++)
                        {
                            sw.Write(texture[i][j].character());
                            sw.Write(ByteToHex(texture[i][j].foreground()));
                            sw.Write(ByteToHex(texture[i][j].background()));
                        }
                        sw.Write('\n');
                    }
                    sw.Close();
                }
            }
            catch (Exception e)
            {
                Environment.FailFast($"Unhandled exception at Cs.FileSystem.FileFromTexture (FileSystem.cs:118): " + e.Message);
            }

        }

        private static List<char> ToCharList(string input)
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

        private static byte HexToByte(char input)
        {
            byte num;
            if (byte.TryParse(input.ToString(), out num)) // 0-9
            {
                if (num >= 0 && num <= 9)
                return num;
            }
            switch (input) // A-F
            {
                case 'A': return 10;
                case 'B': return 11;
                case 'C': return 12;
                case 'D': return 13;
                case 'E': return 14;
                case 'F': return 15;
                default: return 16;
            }
        }
        private static char ByteToHex(byte input)
        {;
            if (input >= 0 && input <= 9) // 0-9
            {
                return input.ToString().ToCharArray()[0];
            }
            switch (input) // A-F
            {
                case 10: return 'A';
                case 11: return 'B';
                case 12: return 'C';
                case 13: return 'D';
                case 14: return 'E';
                case 15: return 'F';
                default: return '-';
            }
        }
    }
}