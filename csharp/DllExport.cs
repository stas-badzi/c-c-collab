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

    public class TextureSystem {
        [UnmanagedCallersOnly(EntryPoint = "TextureSystem_ImportText")]
        public static IntPtr ImportText(IntPtr file)
        {
            int intptr_size = IntPtr.Size;
            List<String> ret = Cs.TextureSystem.ImportText(TypeConvert.PtrToString(file));
            IntPtr output = Exec.AllocateMemory((nuint)( (ret.Count + 1) * intptr_size ));
            for (int i = 0; i < ret.Count; i++) {
                IntPtr elem = TypeConvert.StringToPtr(ret[i]);
                Exec.WritePointer<IntPtr>(output, intptr_size * i, elem);
            }
            IntPtr end = TypeConvert.StringToPtr("\u0000");
            Exec.WritePointer<IntPtr>(output, intptr_size * ret.Count(), end);

            return output;
        }
        [UnmanagedCallersOnly(EntryPoint = "TextureSystem_ExportText")]
        public static void ExportText(IntPtr path, IntPtr content) {
            if (path == IntPtr.Zero) {
                throw new Exception("Intptr $path Empty");
            }
            if (content == IntPtr.Zero) {
                throw new Exception("Intptr $content Empty");
            }
            
            int intptr_size = IntPtr.Size;
            List<String> text = new List<String>();

            IntPtr elem = Exec.ReadPointer<IntPtr>(content,0);
            String line = TypeConvert.PtrToString(elem);
            for (int i = 1; line.Length > 0; i++) {
                text.Add(line);
                elem = Exec.ReadPointer<IntPtr>(content, i * intptr_size);
                line = TypeConvert.PtrToString(elem);
            }

            Cs.TextureSystem.ExportText(TypeConvert.PtrToString(path), text);
        }
        [UnmanagedCallersOnly(EntryPoint = "TextureSystem_TextureFromFile")]
        public static nint TextureFromFile(nint filepathPtr)
        {
            if (filepathPtr == IntPtr.Zero)
                throw new Exception("Intptr $filepathPtr Empty");

            string filepath = TypeConvert.PtrToString(filepathPtr); // Convert filepath
            List<List<Terminal.Symbol>> func = Cs.TextureSystem.TextureFromFile(filepath); // Original function

            return TypeConvert.TextureToPtr(func);
        }
        [UnmanagedCallersOnly(EntryPoint = "TextureSystem_FileFromTexture")]
        public static void FileFromTexture(nint filepathPtr, nint texturePtr, bool recycle = false)
        {
            if (filepathPtr == IntPtr.Zero)
                throw new Exception("Intptr $filepathPtr Empty");
            if (texturePtr == IntPtr.Zero)
                throw new Exception("Intptr $texturePtr Empty");

            string filepath = TypeConvert.PtrToString(filepathPtr);

            var texture = TypeConvert.PtrToTexture(texturePtr);

            Cs.TextureSystem.FileFromTexture(filepath, texture, recycle);
        }
        [UnmanagedCallersOnly(EntryPoint = "TextureSystem_DrawTextureToScreen")]
        public static void DrawTextureToScreen(int x, int y, nint texturePtr, nint screenPtr)
        {
            if (texturePtr == IntPtr.Zero)
                throw new Exception("Intptr $texturePtr Empty");
            if (screenPtr == IntPtr.Zero)
                throw new Exception("Intptr $screenPtr Empty");
            
            var texture = TypeConvert.PtrToTexture(texturePtr);
            var screen = TypeConvert.PtrToTexture(screenPtr,true);

            Cs.TextureSystem.DrawTextureToScreen(x, y, texture, screen);
        }
    }

    public class Control {
        [UnmanagedCallersOnly(EntryPoint = "Control_CleanMemory")]
        public static void CleanMemory() {
            Cs.Control.CleanMemory();
        }
    }

    public class Console {
        public class Symbol {
            [UnmanagedCallersOnly(EntryPoint = "Console_Symbol_ReverseColors")]
            public static void ReverseColors(nint sym) {
                Console.Symbol.ReverseColors(new Terminal.Symbol(sym, true));
            }
        }
    }

    [UnmanagedCallersOnly(EntryPoint = "_PlaySound")]
    public static void _PlaySound(IntPtr filepathPtr, bool wait = false) {
        if (filepathPtr == IntPtr.Zero)
            throw new Exception("Intptr $filepathPtr Empty");

        Cs.PlaySound(TypeConvert.PtrToString(filepathPtr),wait);
    }
}