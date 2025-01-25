using System.Security.AccessControl;
using System;
using Microsoft.VisualBasic.FileIO;
using NetCoreAudio;
using Cpp;
using System.Collections.Generic;
using Texture = System.Collections.Generic.List<System.Collections.Generic.List<Cpp.Terminal.Symbol>>;

namespace Cs
{
    public class TextureSystem { // Handling Texture I/O
        public static List<string> ImportText(string str_file) {
            string? line;
            List<String> result = new List<String>();
            try {
                StreamReader sr = new StreamReader(str_file);
                line = sr.ReadLine();
                while (line != null) {
                    result.Add(line);
                    line = sr.ReadLine();
                }
                sr.Close();
            }
            catch (Exception e) {
                Environment.FailFast($"Unhandled exception at Cs.TextureSystem.ImportText: " + e.Message);
            }
            
            return result;
        }
        
        public static void ExportText(string str_path, List<string> str_content) {
            try {
                if (File.Exists(str_path)) File.Delete(str_path);
                
                using (StreamWriter sw = File.CreateText(str_path)) {
                    for (int i = 0; i < str_content.Count(); ++i) {
                        sw.WriteLine(str_content[i]);
                    }
                    sw.Close();
                }
                
            }
            catch (Exception e) {
                Environment.FailFast($"Unhandled exception at Cs.TextureSystem.ExportText: " + e.Message);
            }
        }
        
        public static Texture TextureFromFile(string filepath) {

            var fileImported = ImportText(filepath); // Imported List<string>
            var file = new List<List<char>>(); // Every character of file
            var symbols = new Texture(); // Final symbol list

            if (!int.TryParse(fileImported[0], out int width)) throw new Exception("Parsing width failed");
            
            if (!int.TryParse(fileImported[1], out int height)) throw new Exception("Parsing height failed");

            for (int i = 2; i < fileImported.Count; i++) { // Importing file to List<List<char>>
                file.Add(ToCharList(fileImported[i]));
            }

            int remainingSymbols = width * 3 * height;
            for (int i = 0; i < height; i++) {
                
                var symbolLine = new List<Terminal.Symbol>();

                for (int j = 0; j < width * 3; j+=3) {
                    symbolLine.Add(new Terminal.Symbol(file[i][j], HexToByte(file[i][j+1]), HexToByte(file[i][j+2])));
                }
                symbols.Add(symbolLine);
            }
            

            return symbols;
        }

        public static void FileFromTexture(string filepath, Texture texture, bool recycle = false) {
            var file = new FileInfo(filepath);
            int width, height;
            
            try {
                using (StreamWriter sw = file.CreateText()) {
                    width = texture[0].Count;
                    sw.WriteLine(width);

                    height = texture.Count;
                    sw.WriteLine(height);

                    for (int i = 0; i < height; i++) {
                        for (int j = 0; j < texture[i].Count; j++) {
                            sw.Write(texture[i][j].character());
                            sw.Write(ByteToHex(texture[i][j].foreground()));
                            sw.Write(ByteToHex(texture[i][j].background()));
                        }
                        sw.Write('\n');
                    }
                    sw.Close();
                }
            }
            catch (Exception e) {
                Environment.FailFast($"Unhandled exception at Cs.TextureSystem.FileFromTexture: " + e.Message);
            }

        }

        public static void DrawTextureToScreen(int x, int y, Texture texture, Texture screen) {
            int? width = texture[0].Count;
            int? scrWidth = screen[0].Count;
            int? height = texture.Count;
            int? scrHeight = screen.Count;

            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    if (y+i >= 0 && y+i < scrHeight && x+j >= 0 && x+j < scrWidth) {
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
                    }
                }
            }
        }

        private static List<char> ToCharList(string input) {
            var array = input.ToCharArray();
            var list = new List<char>();
            for (int i  = 0; i < array.Length; i++) {
                if (array[i] == '\n') // DOESN'T ADD NEW LINE CHARACTERS (Correct if wrong)
                    continue;
                list.Add(array[i]);
            }
            return list;
        }

        private static byte HexToByte(char input) {
            byte num;
            if (byte.TryParse(input.ToString(), out num)) { // 0-9
                if (num >= 0 && num <= 9)
                    return num;
            }
            return input switch { // A-F
                'A' => 10,
                'B' => 11,
                'C' => 12,
                'D' => 13,
                'E' => 14,
                'F' => 15,
                _ => 16
            };
        }
        private static char ByteToHex(byte input) {
            if (input >= 0 && input <= 9) // 0-9
                return input.ToString().ToCharArray()[0];
            return input switch { // A-F
                10 => 'A',
                11 => 'B',
                12 => 'C',
                13 => 'D',
                14 => 'E',
                15 => 'F',
                _ => '-'
            };
        }
        public class Tile { // Texture struct
            public Texture texture;
            public string id;
            public Tile(string id, Texture texture) {
                this.id = id;
                this.texture = texture;
            }
        }
    }
}