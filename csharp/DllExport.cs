using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

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
        public static [MarshalAs(UnmanagedType::LPWStr)]String^ ImportText([MarshalAs(UnmanagedType::LPWStr)]String^ file)
        {
            /* <-- '/'@here
            string str_file = new string();
            
            char c = '\t';
            for (int i = 0; c != '\0'; i++)
            {
                c = 'a';
            }
            return Marshal.StringToHGlobalUni(Cs.FileSystem.ImportText(str_file));//*/
            return gcnew String(Cs.FileSystem.ImportText(new string(file)));
        }
        // export
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_ExportText")]
        public static void ExportText(IntPtr path, IntPtr content)
        {
            Cs.FileSystem.ExportText(Marshal.PtrToStringUni(path),Marshal.PtrToStringUni(content));
        }
    }
}