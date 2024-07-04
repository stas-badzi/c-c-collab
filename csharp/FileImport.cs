using System.IO;
namespace Cs
{
    public class FileImport
    {
        // Impotowanie plików .txt
        public void ImportTxt()
        {
            string? sample;
            string line;
                    Console.Write(@"Przykład ścieżki pliku: C:\Users\Martin\Desktop\sample.txt
                    Podaj ścieżkę pliku: ");
                    sample = Console.ReadLine();
            try
            {
                StreamReader sr = new StreamReader(sample);
                line = sr.ReadLine();
                while (line != null)
                {
                    Console.WriteLine(line);
                    line = sr.ReadLine();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("FileImport wyjątek (exception): " + e.Message);
            }
            finally
            {
                Console.WriteLine("Zakończono importowanie pliku");
            }

        }
    }
}