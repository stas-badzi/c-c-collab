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

            nint input = Marshal.AllocHGlobal(int32_size + (byte_size * taki.Count));

            Marshal.WriteInt32(input, taki.Count);

            for (int i = 0; i < taki.Count; i++) {
                byte b = 0;
                if (taki[i]) {b = 1;}
                Marshal.WriteByte(input, int32_size + (i * byte_size), b);
            }

            return TypeConvert.PtrToString(CsImp.FileSystem.DoSomeThings(input, TypeConvert.StringToPtr(str)));
        }

        public static List<ulong> DoMoreThings(List<string> ls) {
            int nint_size = nint.Size;
            int int_size = sizeof(int);
            int long_size = sizeof(long);

            nint input = Marshal.AllocHGlobal(int_size + (ls.Count * nint_size));
            Marshal.WriteInt32(input, ls.Count);

            for (int i = 0; i < ls.Count; i++) {
                nint ptr = TypeConvert.StringToPtr(ls[i]);
                Marshal.WriteIntPtr(input, int_size + (i * nint_size), ptr);
            }

            nint ret = CsImp.FileSystem.DoMoreThings(input);

            List<ulong> output = new List<ulong>();

            Int32 size = Marshal.ReadInt32(ret);
            for (int i = 0; i < size; i++) {
                Int64 signed = Marshal.ReadInt64(ret, int_size + (i*long_size));
                output.Add(Convert.ToUInt64(signed));
            }

            Marshal.FreeHGlobal(ret);

            return output;
        }

        public static List<String> ImportText(String filestr) {
            IntPtr ret = CsImp.FileSystem.ImportText(TypeConvert.StringToPtr(filestr));
            int intptr_size = IntPtr.Size;
            List<String> text = new List<String>();

            IntPtr elem = Marshal.ReadIntPtr(ret, 0);
            String line = TypeConvert.PtrToString(elem);
            for (int i = 1; line.Length > 0; i++) {
                text.Add(line);
                elem = Marshal.ReadIntPtr(ret, i * intptr_size);
                line = TypeConvert.PtrToString(elem);
                Marshal.FreeHGlobal(elem);
            }

            Marshal.FreeHGlobal(ret);
            
            return text;
        }

        public static void ExportText(string pathstr, List<string> contentstr) {
            int intptr_size = IntPtr.Size;
            IntPtr output = Marshal.AllocHGlobal( (contentstr.Count + 1) * intptr_size);
            for (int i = 0; i < contentstr.Count; i++) {
                IntPtr elem = TypeConvert.StringToPtr(contentstr[i]);
                Marshal.WriteIntPtr(output, intptr_size * i, elem);
            }
            IntPtr end = TypeConvert.StringToPtr("\u0000");
            Marshal.WriteIntPtr(output, intptr_size * contentstr.Count, end);
            CsImp.FileSystem.ExportText(TypeConvert.StringToPtr(pathstr), output);
        }

        public static List<List<Terminal.Symbol>> TextureFromFile(string filepath)
        {
            const int int32_size = sizeof(int);
            int intptr_size = nint.Size;
            int size, count;
            
            nint filepathPtr = TypeConvert.StringToPtr(filepath);  // Convert filepath back
            nint func = CsImp.FileSystem.TextureFromFile(filepathPtr); // Get exported function

            var texture = TypeConvert.PtrToTexture(func);

            Marshal.FreeHGlobal(func);
            return texture;
        }
        public static void FileFromTexture(string filepath, List<List<Terminal.Symbol>> texture, bool recycle = false)
        {
            const int int32_size = sizeof(int);
            int intptr_size = nint.Size;
            int size, count;

            nint filepathPtr = TypeConvert.StringToPtr(filepath);
            nint texturePtr = TypeConvert.TextureToPtr(texture);

            CsImp.FileSystem.FileFromTexture(filepathPtr, texturePtr, recycle);
            Marshal.FreeHGlobal(texturePtr);
        }
        public static void DrawTextureToScreen(int x, int y, List<List<Terminal.Symbol>> texture, List<List<Terminal.Symbol>> screen)
        {
            nint texturePtr = TypeConvert.TextureToPtr(texture);
            nint screenPtr = TypeConvert.TextureToPtr(screen);

            CsImp.FileSystem.DrawTextureToScreen(x, y, texturePtr, screenPtr);
            Marshal.FreeHGlobal(texturePtr);
            Marshal.FreeHGlobal(screenPtr);
        }
        public static void PlayMP3(string filepath)
        {
            CsImp.FileSystem.PlayMP3(TypeConvert.StringToPtr(filepath));
        }
    }
}