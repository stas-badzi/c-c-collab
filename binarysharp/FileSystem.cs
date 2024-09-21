using System.Runtime.InteropServices;
using Utility;
using Cpp;
using System.Text.Json.Serialization.Metadata;

namespace Cs {
    public class FileSystem {
        public static ulong Add(uint a, byte b) {
            return CsImp.FileSystem.Add(a, b);
        }

        public static char DoSomething(bool tak, char c) {
            return TypeConvert.UnicodeToUtf8(CsImp.FileSystem.DoSomething(tak, TypeConvert.Utf8ToUnicode(c)));
        }

        public static string DoSomeThings(List<bool> taki, string str) {
            int byte_size = sizeof(byte);
            int int32_size = sizeof(Int32);

            nint input = Exec.AllocateMemory((nuint)(int32_size + (byte_size * taki.Count)));

            Exec.WritePointer<Int32>(input, taki.Count);

            for (int i = 0; i < taki.Count; i++) {
                byte b = 0;
                if (taki[i]) {b = 1;}
                Exec.WritePointer<Byte>(input, int32_size + (i * byte_size), b);
            }

            return TypeConvert.PtrToString(CsImp.FileSystem.DoSomeThings(input, TypeConvert.StringToPtr(str)));
        }

        public static List<ulong> DoMoreThings(List<string> ls) {
            int nint_size = nint.Size;
            int int_size = sizeof(int);
            int long_size = sizeof(long);

            nint input = Exec.AllocateMemory((nuint)(int_size + (ls.Count * nint_size)));
            Exec.WritePointer<Int32>(input, ls.Count);

            for (int i = 0; i < ls.Count; i++) {
                nint ptr = TypeConvert.StringToPtr(ls[i]);
                Exec.WritePointer<IntPtr>(input, int_size + (i * nint_size), ptr);
            }

            nint ret = CsImp.FileSystem.DoMoreThings(input);

            List<ulong> output = new List<ulong>();

            Int32 size = Exec.ReadPointer<Int32>(ret);
            for (int i = 0; i < size; i++) {
                Int64 signed = Exec.ReadPointer<Int64>(ret, int_size + (i*long_size));
                output.Add(Convert.ToUInt64(signed));
            }

            Exec.FreeMemory(ret);

            return output;
        }

        public static List<String> ImportText(String filestr) {
            IntPtr ret = CsImp.FileSystem.ImportText(TypeConvert.StringToPtr(filestr));
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
            CsImp.FileSystem.ExportText(TypeConvert.StringToPtr(pathstr), output);
        }

        public static List<List<Terminal.Symbol>> TextureFromFile(string filepath)
        {
            nint filepathPtr = TypeConvert.StringToPtr(filepath);  // Convert filepath back
            nint func = CsImp.FileSystem.TextureFromFile(filepathPtr); // Get exported function

            var texture = TypeConvert.PtrToTexture(func);

            Exec.FreeMemory(func);
            return texture;
        }
        public static void FileFromTexture(string filepath, List<List<Terminal.Symbol>> texture, bool recycle = false)
        {
            nint filepathPtr = TypeConvert.StringToPtr(filepath);
            nint texturePtr = TypeConvert.TextureToPtr(texture);

            CsImp.FileSystem.FileFromTexture(filepathPtr, texturePtr, recycle);
            Exec.FreeMemory(texturePtr);
        }
        public static void DrawTextureToScreen(int x, int y, List<List<Terminal.Symbol>> texture, List<List<Terminal.Symbol>> screen)
        {
            nint texturePtr = TypeConvert.TextureToPtr(texture);
            nint screenPtr = TypeConvert.TextureToPtr(screen);

            CsImp.FileSystem.DrawTextureToScreen(x, y, texturePtr, screenPtr);
            Exec.FreeMemory(texturePtr);
            Exec.FreeMemory(screenPtr);
        }
        public static void PlaySound(string filepath, bool wait = false)
        {
            CsImp.FileSystem.PlaySound(TypeConvert.StringToPtr(filepath),wait);
        }
    }
}