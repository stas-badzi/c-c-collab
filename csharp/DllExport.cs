using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using Utility;

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
    }
}