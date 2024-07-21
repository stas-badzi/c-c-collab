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
        public static unsafe IntPtr ImportText(char* file)
        {
            String filestr = "";
            int a = 0;
            for (int i = 0; a < 10; i++) {
                filestr += *(file+i);
            }

            char[] text = Cs.FileSystem.ImportText(filestr).ToCharArray();

            IntPtr textptr = Marshal.AllocHGlobal(text.Length * sizeof(char));
            Marshal.Copy(text, 0, textptr, text.Length * sizeof(char));
            return textptr;
        }
        // export
        [UnmanagedCallersOnly(EntryPoint = "FileSystem_ExportText")]
        public static unsafe void ExportText(char* path, char* content) {
            String pathstr = "";
            for (int i = 0; *(path+i) != '\0'; i++) {
                pathstr += *(path+i);
            }

            String contentstr ="";
            for (int i = 0; *(content+i) != '\0'; i++) {
                contentstr += *(content+i);
            }

            Cs.FileSystem.ExportText(pathstr,contentstr);
        }
    }
}