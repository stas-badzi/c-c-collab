using System;
using Microsoft.VisualBasic.FileIO;
using NetCoreAudio;
using Cpp;

namespace Cs
{
    public class FileSystem // Impotowanie plików .txt
    {
        public static ulong Add(uint a, byte b) {
            return a + b;
        }

        public static char DoSomething(bool tak, char c) {
            return tak? 'Y' : c;
        }

        public static string DoSomeThings(List<bool> taki, string s) {
            System.Console.WriteLine(s);
            string o = "";
            for (int i = 0; i < s.Length; i++) 
            {
                if (i < taki.Count && taki[i]) 
                {
                    o += '#';
                } 
                else 
                {
                    o += s[i];
                }
            }
            return o;
        }

        public static List<ulong> DoMoreThings(List<string> ls) {
            List<ulong> output = new List<ulong>();

            for (int i = 0; i < ls.Count; ++i) {
                output.Add(Convert.ToUInt64(ls[i].Length) * Convert.ToUInt64(ls[i].Length));
            }

            return output;
        }

        public static List<string> ImportText(string str_file)
        {
            string? line;
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
                Environment.FailFast($"Unhandled exception at Cs.FileSystem.ImportText: " + e.Message);
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
                Environment.FailFast($"Unhandled exception at Cs.FileSystem.ExportText: " + e.Message);
            }
        }
        
        public static List<List<Terminal.Symbol>> TextureFromFile(string filepath)
        {

            var fileImported = ImportText(filepath); // Imported List<string>
            var file = new List<List<char>>(); // Every character of file
            var symbols = new List<List<Terminal.Symbol>>(); // Final symbol list

            if (!int.TryParse(fileImported[0], out int width)) throw new Exception("Parsing width failed");
            
            if (!int.TryParse(fileImported[1], out int height)) throw new Exception("Parsing width failed");

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

        public static void DrawTextureToScreen(int x, int y, List<List<Terminal.Symbol>> texture, List<List<Terminal.Symbol>> screen)
        {
            int? height = texture.Count;
            int? scrHeight = screen.Count;

            for (int i = 0; i < height && y+i < scrHeight; i++)
            {
                if (y+i < 0) {
                    i = -y;
                    --i;
                    continue;
                }
                int? width = texture[i].Count;
                int? scrWidth = screen[y+i].Count;
                for (int j = 0; j < width && x+j < scrWidth; j++) {
                    if (x+j < 0) {
                        j = -x;
                        --j;
                        continue;
                    }
                    try {
                        var elem = texture[i][j];
                        if (elem.character() != '\t') {
                            screen[y+i][x+j].character(elem.character());
                        }
                        if (elem.foreground() < 16) {
                            screen[y+i][x+j].foreground(elem.foreground());
                        }
                        if (elem.background() < 16) {
                            screen[y+i][x+j].background(elem.background());
                        }
                    } catch (Exception e) {
                        Environment.FailFast($"Unhandled exception: " + e.Message + "\nAt: " + e.StackTrace);
                    }
                }
            }
        }
        public static void PlaySound(string filepath, bool wait = false) {
            string fullpath = Path.GetFullPath(filepath);
            Player player = new Player();
            player.Play(filepath);
            TimeSpan shorttime = new TimeSpan(1);
            while (wait && player.Playing) Thread.Sleep(shorttime);
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
        {
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