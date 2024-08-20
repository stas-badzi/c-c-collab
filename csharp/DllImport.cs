using System.Runtime.InteropServices;
using Utility;

namespace CppImp {

    public class Terminal
    {
        private const string DllSource = @"" + DllHandle.Prefix + "cplusplus" + DllHandle.Suffix;

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Init", CharSet = CharSet.Unicode)]
        public static extern void Init();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Fin", CharSet = CharSet.Unicode)]
        public static extern void Fin();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_HandleKeyBoard", CharSet = CharSet.Unicode)]
        public static extern int HandleKeyboard();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_KeyDown", CharSet = CharSet.Unicode)]
        public static extern bool KeyDown(int arg1);

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_KeyToggled", CharSet = CharSet.Unicode)]
        public static extern bool KeyToggled(int arg1);

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_KeyHit", CharSet = CharSet.Unicode)]
        public static extern bool KeyHit(int arg1);

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_KeyReleased", CharSet = CharSet.Unicode)]
        public static extern bool KeyReleased(int arg1);

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
            public static extern IntPtr Construct(Int32 character, byte foreground = 7, byte background = 0);

        #if _WIN32
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_Construct$atr", CharSet = CharSet.Unicode)]
            public static extern IntPtr Construct(byte attribute);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_SetAttribute", CharSet = CharSet.Unicode)]
            public static extern void SetAttribute(IntPtr smb, byte attribute);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_GetAttribute", CharSet = CharSet.Unicode)]
            public static extern byte GetAttribute(IntPtr smb);
        #endif

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_character$set", CharSet = CharSet.Unicode)]
            public static extern void character(IntPtr smb, Int32 character);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_character$get", CharSet = CharSet.Unicode)]
            public static extern int character(IntPtr smb);

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