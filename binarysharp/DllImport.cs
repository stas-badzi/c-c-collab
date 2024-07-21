using System.Runtime.InteropServices;

namespace CsImp {
    
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
        private const string DllSource =  @"" + DllHandle.Prefix + "csharp" + DllHandle.Suffix;

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "FileSystem_ImportText", CharSet = CharSet.Unicode)]
        public static extern unsafe char* ImportText(char* fileptr);

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "FileSystem_ExportText", CharSet = CharSet.Unicode)]
        public static extern unsafe void ExportText(char* pathptr, char* contentptr);
    }
}

namespace CppImp {

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

    public class Console
    {
        private const string DllSource = @"" + DllHandle.Prefix + "cplusplus" + DllHandle.Suffix;

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Init", CharSet = CharSet.Unicode)]
        public static extern void Init();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_FillScreen$ret2", CharSet = CharSet.Unicode)]
        public static extern IntPtr FillScreen(IntPtr symbols, int height, int width);

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_GetWindowWidth", CharSet = CharSet.Unicode)]
        public static extern short GetWindowWidth();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_GetWindowHeight", CharSet = CharSet.Unicode)]
        public static extern short GetWindowHeight();

        public struct Symbol {
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_Construct$smb", CharSet = CharSet.Unicode)]
            public static extern IntPtr Construct(IntPtr smb);
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_Construct$cfb", CharSet = CharSet.Unicode)]
            public static extern IntPtr Construct(char character, byte foreground = 7, byte background = 0);

        #if _WIN32
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_Construct$atr", CharSet = CharSet.Unicode)]
            public static extern IntPtr Construct(byte attribute);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_SetAttribute", CharSet = CharSet.Unicode)]
            public static extern void SetAttribute(IntPtr smb, byte attribute);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_GetAttribute", CharSet = CharSet.Unicode)]
            public static extern byte GetAttribute(IntPtr smb);
        #endif

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_character$set", CharSet = CharSet.Unicode)]
            public static extern void character(IntPtr smb, char character);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_character$get", CharSet = CharSet.Unicode)]
            public static extern char character(IntPtr smb);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_foreground$set", CharSet = CharSet.Unicode)]
            public static extern void foreground(IntPtr smb, byte foreground);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_foreground$get", CharSet = CharSet.Unicode)]
            public static extern byte foreground(IntPtr smb);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_background$get", CharSet = CharSet.Unicode)]
            public static extern void background(IntPtr smb, byte background);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_background$get", CharSet = CharSet.Unicode)]
            public static extern byte background(IntPtr smb);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_Destruct", CharSet = CharSet.Unicode)]
            public static extern void Destruct(IntPtr smb);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_operator$eq", CharSet = CharSet.Unicode)]
            public static extern IntPtr operator_eq(IntPtr cp, IntPtr src);

        }
    }
}