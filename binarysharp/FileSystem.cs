using System.Runtime.InteropServices;
using Utility;
using Cpp;
using System.Text.Json.Serialization.Metadata;

namespace Cs {
    public class TextureSystem {
        public static List<String> ImportText(String filestr) {
            IntPtr ret = CsImp.TextureSystem.ImportText(TypeConvert.StringToPtr(filestr));
            int intptr_size = IntPtr.Size;
            List<String> text = new List<String>();

            IntPtr elem = Exec.ReadPointer<IntPtr>(ret, 0);
            String line = TypeConvert.PtrToString(elem);
            for (int i = 1; line.Length > 0; i++) {
                text.Add(line);
                elem = Exec.ReadPointer<IntPtr>(ret, i * intptr_size);
                line = TypeConvert.PtrToString(elem);
                Exec.FreeMemory(elem);
            }

            Exec.FreeMemory(ret);
            
            return text;
        }

        public static void ExportText(string pathstr, List<string> contentstr) {
            int intptr_size = IntPtr.Size;
            IntPtr output = Exec.AllocateMemory((nuint)((contentstr.Count + 1) * intptr_size));
            for (int i = 0; i < contentstr.Count; i++) {
                IntPtr elem = TypeConvert.StringToPtr(contentstr[i]);
                Exec.WritePointer<IntPtr>(output, intptr_size * i, elem);
            }
            IntPtr end = TypeConvert.StringToPtr("\u0000");
            Exec.WritePointer<IntPtr>(output, intptr_size * contentstr.Count, end);
            CsImp.TextureSystem.ExportText(TypeConvert.StringToPtr(pathstr), output);
        }

        public static List<List<Terminal.Symbol>> TextureFromFile(string filepath)
        {
            nint filepathPtr = TypeConvert.StringToPtr(filepath);  // Convert filepath back
            nint func = CsImp.TextureSystem.TextureFromFile(filepathPtr); // Get exported function

            var texture = TypeConvert.PtrToTexture(func);

            Exec.FreeMemory(func);
            return texture;
        }
        public static void FileFromTexture(string filepath, List<List<Terminal.Symbol>> texture, bool recycle = false)
        {
            nint filepathPtr = TypeConvert.StringToPtr(filepath);
            nint texturePtr = TypeConvert.TextureToPtr(texture);

            CsImp.TextureSystem.FileFromTexture(filepathPtr, texturePtr, recycle);
            Exec.FreeMemory(texturePtr);
        }
        public static void DrawTextureToScreen(int x, int y, List<List<Terminal.Symbol>> texture, List<List<Terminal.Symbol>> screen)
        {
            nint texturePtr = TypeConvert.TextureToPtr(texture);
            nint screenPtr = TypeConvert.TextureToPtr(screen);

            CsImp.TextureSystem.DrawTextureToScreen(x, y, texturePtr, screenPtr);
            Exec.FreeMemory(texturePtr);
            Exec.FreeMemory(screenPtr);
        }
    }
}