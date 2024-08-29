using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using Utility;
using Cs;
using System.Drawing;
using Cpp;
using Console = Cs.Console;
// UTF-8: int
// UNICODE: char

namespace CsExp {

    public class FileSystem {
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_Add")]
        public static ulong Add(uint a, byte b) {
            return Cs.FileSystem.Add(a,b);
        }

        [UnmanagedCallersOnly(EntryPoint = "FileSystem_DoSomething")]
        public static int DoSomething(bool yes, char ch) {
            return TypeConvert.Utf8ToUnicode(Cs.FileSystem.DoSomething(yes, TypeConvert.UnicodeToUtf8(ch)));
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

            return TypeConvert.StringToPtr(Cs.FileSystem.DoSomeThings(input, TypeConvert.PtrToString(str)));
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
                input.Add(TypeConvert.PtrToString(ptr));
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

        [UnmanagedCallersOnly(EntryPoint = "Console_Symbol_ReverseColors")]
        public static void ReverseColors(nint sym) {
            Console.Symbol.ReverseColors(new Terminal.Symbol(sym, true));
        }

        [UnmanagedCallersOnly(EntryPoint = "FileSystem_ImportText")]
        public static IntPtr ImportText(IntPtr file)
        {
            int intptr_size = IntPtr.Size;
            List<String> ret = Cs.FileSystem.ImportText(TypeConvert.PtrToString(file));
            IntPtr output = Marshal.AllocHGlobal( (ret.Count + 1) * intptr_size);
            for (int i = 0; i < ret.Count; i++) {
                IntPtr elem = TypeConvert.StringToPtr(ret[i]);
                Marshal.WriteIntPtr(output, intptr_size * i, elem);
            }
            IntPtr end = TypeConvert.StringToPtr("\u0000");
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
            String line = TypeConvert.PtrToString(elem);
            for (int i = 1; line.Length > 0; i++) {
                text.Add(line);
                elem = Marshal.ReadIntPtr(content, i * intptr_size);
                line = TypeConvert.PtrToString(elem);
            }

            Cs.FileSystem.ExportText(TypeConvert.PtrToString(path), text);
        }
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_TextureFromFile")]
        public static nint TextureFromFile(nint filepathPtr)
        {
            if (filepathPtr == IntPtr.Zero)
                throw new Exception("Intptr $filepathPtr Empty");
            
            const int int32_size = sizeof(int);
            int intptr_size = nint.Size;

            string filepath = TypeConvert.PtrToString(filepathPtr); // Convert filepath
            List<List<Terminal.Symbol>> func = Cs.FileSystem.TextureFromFile(filepath); // Original function

            return TypeConvert.TextureToPtr(func);
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

            string filepath = TypeConvert.PtrToString(filepathPtr);

            var texture = TypeConvert.PtrToTexture(texturePtr);

            Cs.FileSystem.FileFromTexture(filepath, texture, recycle);
        }
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_DrawTextureToScreen")]
        public static void DrawTextureToScreen(int x, int y, nint texturePtr, nint screenPtr)
        {
            if (texturePtr == IntPtr.Zero)
                throw new Exception("Intptr $texturePtr Empty");
            if (screenPtr == IntPtr.Zero)
                throw new Exception("Intptr $screenPtr Empty");
            
            var texture = TypeConvert.PtrToTexture(texturePtr);
            var screen = TypeConvert.PtrToTexture(screenPtr);

            Cs.FileSystem.DrawTextureToScreen(x, y, texture, screen);
        }
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_PlayWAV")]
        public static void PlayWAV(IntPtr filepathPtr, bool wait = false)
        {
            if (filepathPtr == IntPtr.Zero)
                throw new Exception("Intptr $filepathPtr Empty");

            Cs.FileSystem.PlayWAV(TypeConvert.PtrToString(filepathPtr),wait);
        }
    }
}