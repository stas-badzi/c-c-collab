using System.Runtime.InteropServices;

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
            return Marshal.StringToHGlobalUni(Cs.FileSystem.ImportText(Marshal.PtrToStringUni(file)));
        }
        // export
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_ExportText")]
        public static void ExportText(IntPtr path, IntPtr content)
        {
            Cs.FileSystem.ExportText(Marshal.PtrToStringUni(path),Marshal.PtrToStringUni(content));
        }
    }

}