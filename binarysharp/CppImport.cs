using System;
using System.Runtime.InteropServices;

namespace CppImport
{
    public static class ConsoleInterop
    {
        private const string DllName = "dynamic_library";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Console_Init();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Console_FillScreen_ret2(ref IntPtr symbols, int height, int width);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Console_GetWindowWidth();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Console_GetWindowHeight();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Console_Symbol_Construct_smb(IntPtr sym);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Console_Symbol_Construct_cfb(char character, byte foreground = 7, byte background = 0);

        #if _WIN32
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Console_Symbol_Construct_atr(byte attribute);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Console_Symbol_SetAttribute(IntPtr smb, byte attribute);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte Console_Symbol_GetAttribute(IntPtr smb);
        #endif

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Console_Symbol_character_set(IntPtr smb, char character);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern char Console_Symbol_character_get(IntPtr smb);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Console_Symbol_foreground_set(IntPtr smb, byte foreground);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte Console_Symbol_foreground_get(IntPtr smb);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Console_Symbol_background_set(IntPtr smb, byte background);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte Console_Symbol_background_get(IntPtr smb);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Console_Symbol_Destruct(IntPtr smb);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Console_Symbol_operator_eq(IntPtr cp, IntPtr src);

        public static void Test() // Only for experiment purposes
        {
            Console.WriteLine("test suceeded");
        }
    }
}
