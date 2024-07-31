using System;
using System.IO;
using System.Collections.Generic;
using System.CommandLine;

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
        
        // !!
        // Comments soon to be deleted when finished function
        public static List<List<Symbol>> TextureFromFile(int width, int height, string filename)
        {
            // Tworzymy siatkę tekstury o wymiarach width x height
            List<List<Symbol>> texture = new List<List<Symbol>>();
            for (int i = 0; i < height; i++)
            {
                texture.Add(new List<Symbol>());
                for (int j = 0; j < width; j++)
                {
                    // Wypełniamy domyślnymi symbolami (spacja z białym kolorem na czarnym tle)
                    texture[i].Add(new Symbol(' ', ConsoleColor.Gray, ConsoleColor.Black));
                }
            }

            try
            {
                // Otwieramy plik do odczytu
                using (StreamReader sr = new StreamReader(filename))
                {
                    int currentRow = 0; // Aktualny wiersz w siatce
                    int currentCol = 0; // Aktualna kolumna w siatce
                    int totalCells = width * height; // Liczba komórek do wypełnienia

                    while (!sr.EndOfStream && totalCells > 0)
                    {
                        char c = (char)sr.Read(); // Odczytujemy jeden znak
                        if (c == '\n' || c == '\r') continue; // Pomijamy znaki nowej linii

                        // Konwertujemy znak na symbol z odpowiednimi kolorami
                        Symbol symbol = CharToSymbol(c);
                        texture[currentRow][currentCol] = symbol;

                        // Przechodzimy do następnej komórki
                        currentCol++;
                        if (currentCol >= width) // Jeśli dotarliśmy do końca wiersza
                        {
                            currentCol = 0; // Przechodzimy do pierwszej kolumny
                            currentRow++; // Przechodzimy do następnego wiersza
                        }
                        totalCells--; // Zmniejszamy liczbę komórek do wypełnienia
                    }
                }
            }
            catch (Exception e)
            {
                Environment.FailFast($"Unhandled exception at Cs.FileSystem.TextureFromFile: " + e.Message);
            }

            return texture;
        }

        // Funkcja do konwertowania znaków na symbole z kolorami
        public static Symbol CharToSymbol(char input)
        {
            // Mapujemy każdy znak na symbol z domyślnymi kolorami
            return new Symbol(input, ConsoleColor.White, ConsoleColor.Black); // Domyślne kolory
        }
    }

    // Klasa reprezentująca symbol z kolorem
    public class Symbol
    {
        public char Character { get; }
        public ConsoleColor ForegroundColor { get; }
        public ConsoleColor BackgroundColor { get; }

        public Symbol(char character, ConsoleColor foregroundColor, ConsoleColor backgroundColor)
        {
            Character = character;
            ForegroundColor = foregroundColor;
            BackgroundColor = backgroundColor;
        }

    }
}