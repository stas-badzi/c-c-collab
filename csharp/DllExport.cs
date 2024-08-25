using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using Utility;
using Cs;
using System.Drawing;
using Cpp;
// UTF-8: int
// UNICODE: char

namespace CsExp {
    public class DllHandle {

    #if _WIN32
        public const string Prefix = "";
        public const string Suffix = ".dll";
    #elif __APPLE__
        public const string Prefix = "lib";
        public const string Suffix = ".dylib";
    #elif __linux__
        public const string Prefix = "lib";
        public const string Suffix = ".so";
    #elif __FreeBSD__
        public const string Prefix = "lib";
        public const string Suffix = ".so";
    #else
        public const string Prefix = "";
        public const string Suffix = "";
    #endif

        
    #if _WIN32
        public const string OS = "Windows";
    #elif __APPLE__
        public const string OS = "Apple";
    #elif __linux__
        public const string OS = "Linux";
    #elif __FreeBSD__
        public const string OS = "FreeBSD";
    #else
        public const string OS = "Unknown";
    #endif

    }

    public class FileSystem {
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_Add")]
        public static ulong Add(uint a, byte b) {
            return Cs.FileSystem.Add(a,b);
        }

        [UnmanagedCallersOnly(EntryPoint = "FileSystem_DoSomething")]
        public static int DoSomething(bool yes, char ch) {
            return UniConv.Utf8ToUnicode(Cs.FileSystem.DoSomething(yes, UniConv.UnicodeToUtf8(ch)));
            // return 
        }

        [UnmanagedCallersOnly(EntryPoint = "FileSystem_DoSomeThings")]
        public static nint DoSomeThings(nint yeses, nint str) {
            int int32_size = sizeof(Int32);
            int byte_size = sizeof(byte);

            List<bool> input = new List<bool>();
            Int32 size = Marshal.ReadInt32(yeses);

            for (int i = 0; i < size; i++) {
                byte by = Marshal.ReadByte(yeses, int32_size + (i*byte_size));
                bool b = false;
                if (by > 0) {b = true;}
                input.Add(b);
            }

            Marshal.FreeHGlobal(yeses);

            return UniConv.StringToPtr(Cs.FileSystem.DoSomeThings(input, UniConv.PtrToString(str)));
        }

        [UnmanagedCallersOnly(EntryPoint = "FileSystem_DoMoreThings")]
        public static nint DoMoreThings(nint list_str) {
            int nint_size = nint.Size;
            int int32_size = sizeof(Int32);
            int uint64_size = sizeof(UInt64);

            List<string> input = new List<string>();

            Int32 size = Marshal.ReadInt32(list_str);
            for (int i = 0; i < size; i++) {
                nint ptr = Marshal.ReadIntPtr(list_str, int32_size + (i*nint_size));
                input.Add(UniConv.PtrToString(ptr));
            }

            Marshal.FreeHGlobal(list_str);

            List<ulong> ret = Cs.FileSystem.DoMoreThings(input);

            nint output = Marshal.AllocHGlobal(int32_size + (ret.Count * uint64_size));

            Marshal.WriteInt32(output, ret.Count);

            for (int i = 0; i < ret.Count; i++) {
                Marshal.WriteInt64(output, int32_size + (i * uint64_size), Convert.ToInt64(ret[i]));
            }

            return output;
        }

        [UnmanagedCallersOnly(EntryPoint = "FileSystem_RevertColors")]
        public static nint RevertColors(nint sym) {
            return Cs.FileSystem.RevertColors(new Terminal.Symbol(sym)).Get();
        }

        [UnmanagedCallersOnly(EntryPoint = "FileSystem_ImportText")]
        public static IntPtr ImportText(IntPtr file)
        {
            int intptr_size = IntPtr.Size;
            List<String> ret = Cs.FileSystem.ImportText(UniConv.PtrToString(file));
            IntPtr output = Marshal.AllocHGlobal( (ret.Count + 1) * intptr_size);
            for (int i = 0; i < ret.Count; i++) {
                IntPtr elem = UniConv.StringToPtr(ret[i]);
                Marshal.WriteIntPtr(output, intptr_size * i, elem);
            }
            IntPtr end = UniConv.StringToPtr("\u0000");
            Marshal.WriteIntPtr(output, intptr_size * ret.Count(), end);

            return output;
        }

        

        // export
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_ExportText")]
        public static void ExportText(IntPtr path, IntPtr content) {
            if (path == IntPtr.Zero) {
                throw new Exception("Intptr $path Empty");
            }
            if (content == IntPtr.Zero) {
                throw new Exception("Intptr $content Empty");
            }
            
            int intptr_size = IntPtr.Size;
            List<String> text = new List<String>();

            IntPtr elem = Marshal.ReadIntPtr(content,0);
            String line = UniConv.PtrToString(elem);
            for (int i = 1; line.Length > 0; i++) {
                text.Add(line);
                elem = Marshal.ReadIntPtr(content, i * intptr_size);
                line = UniConv.PtrToString(elem);
            }

            Cs.FileSystem.ExportText(UniConv.PtrToString(path), text);
        }
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_TextureFromFile")]
        public static nint TextureFromFile(nint filepathPtr)
        {
            if (filepathPtr == IntPtr.Zero)
                throw new Exception("Intptr $filepathPtr Empty");
            
            const int int32_size = sizeof(int);
            int intptr_size = nint.Size;

            string filepath = UniConv.PtrToString(filepathPtr); // Convert filepath
            List<List<Terminal.Symbol>> func = Cs.FileSystem.TextureFromFile(filepath); // Original function

            int count = func.Count; // For better readability 
            int size = 0;
            for (int i = 0; i < func.Count; i++)
                for (int j = 0; j < func[i].Count; j++) // Count all symbols
                    size++;
            
            // Allocate memory for List<>.Count's and all symbol pointers
            nint texture = Marshal.AllocHGlobal(count * int32_size + size * intptr_size);

            size = 0; // Reset size
            Marshal.WriteInt32(texture, count);
            for (int i = 0; i < count; i++)
            {
                count = func[i].Count; // Assign adequate count
                Marshal.WriteInt32(texture, i * int32_size, count);
                for (int j = 0; j < count; j++)
                {
                    // Write symbol pointer in proper place 
                    Marshal.WriteIntPtr(texture, (i + 2) * int32_size + size * intptr_size, func[i][j].Get());
                    size++; // Move target place for next symbol
                }
            }
            return texture;
        }
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_FileFromTexture")]
        public static void FileFromTexture(nint filepathPtr, nint texturePtr, bool recycle = false)
        {
            if (filepathPtr == IntPtr.Zero)
                throw new Exception("Intptr $filepathPtr Empty");
            if (texturePtr == IntPtr.Zero)
                throw new Exception("Intptr $texturePtr Empty");

            const int int32_size = sizeof(int);
            int intptr_size = nint.Size;
            int size, count;

            string filepath = UniConv.PtrToString(filepathPtr);

            var texture = PtrToTexture(texturePtr);

            Cs.FileSystem.FileFromTexture(filepath, texture, recycle);
        }
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_DrawTextureToScreen")]
        public static void DrawTextureToScreen(int x, int y, nint texturePtr, nint screenPtr)
        {
            if (texturePtr == IntPtr.Zero)
                throw new Exception("Intptr $texturePtr Empty");
            if (screenPtr == IntPtr.Zero)
                throw new Exception("Intptr $screenPtr Empty");
            
            var texture = PtrToTexture(texturePtr);
            var screen = PtrToTexture(screenPtr);

            Cs.FileSystem.DrawTextureToScreen(x, y, texture, screen);
        }
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_PlayMP3")]
        public static void PlayMP3(IntPtr filepathPtr)
        {
            if (filepathPtr == IntPtr.Zero)
                throw new Exception("Intptr $filepathPtr Empty");
                
            Cs.FileSystem.PlayMP3(UniConv.PtrToString(filepathPtr));
        }
        private static List<List<Terminal.Symbol>> PtrToTexture(nint ptr) // For easier export
        {
            const int int32_size = sizeof(int);
            int intptr_size = nint.Size;
            int size, count;

            var texture = new List<List<Terminal.Symbol>>();

            size = Marshal.ReadInt32(ptr);
            for (int i = 0; i < size; i++)
                texture.Add(new List<Terminal.Symbol>());

            count = 0;
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < texture[i].Count; j++)
                {
                    nint ni = Marshal.ReadIntPtr(ptr, (i + 2) * int32_size + count * intptr_size);
                    texture[i].Add(new Terminal.Symbol(ptr));       
                    count++;
                }
            }
            return texture;
        }
    }
}