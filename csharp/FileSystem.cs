using System;
using System.IO;
using System.Collections.Generic;
using CSCore;
using CSCore.Codecs;
using CSCore.SoundOut;
using Microsoft.VisualBasic.FileIO;
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

        public static Terminal.Symbol RevertColors(Terminal.Symbol sym) {
            Terminal.Symbol outsym = new Terminal.Symbol(sym);
            outsym.background(sym.foreground());
            outsym.foreground(sym.background());
            return outsym;
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
        
        public static List<List<Terminal.Symbol>> TextureFromFile(string filename)
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

        public static void DrawTextureToScreen(int x, int y, List<List<Terminal.Symbol>> texture, List<List<Terminal.Symbol>> screen)
        {
            int? width = texture[0].Count;
            int? scrWidth = screen[0].Count;
            int? height = texture.Count;
            int? scrHeight = screen.Count;

            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    if (y+i >= 0 && y+i < scrHeight && x+j >= 0 && x+j < scrWidth)
                    {
                        if (texture[i][j].character() != '\t') {
                            screen[i][j].character(texture[i][j].character());
                        }
                        if (texture[i][j].foreground() < 16) {
                            screen[i][j].foreground(texture[i][j].foreground());
                        }
                        if (texture[i][j].background() < 16) {
                            screen[i][j].background(texture[i][j].background());
                        }
                    }
                }
            }
        }

        public static void PlayMP3(string filePath)
        {
            var file = new FileInfo(filePath);
            if (!file.Exists)
            {
                Environment.FailFast("PlayMP3: Filepath not found");
                return;
            }
            using (var audioFile = CodecFactory.Instance.GetCodec(filePath))
            using (var soundOut = new WasapiOut())
            {
                var playbackFinished = new ManualResetEvent(false);

                soundOut.Initialize(audioFile);
                soundOut.Stopped += (s, e) =>
                {
                    playbackFinished.Set();
                };
                soundOut.Play();
                playbackFinished.WaitOne();
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