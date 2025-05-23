using System.Runtime.InteropServices;
using Utility;

namespace CsImp {

    public class Terminal {
        private const string DllSource =  @"" + DllHandle.Prefix + "factoryrushsharp" + DllHandle.Suffix;
        public class Symbol {
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_ReverseColors", CharSet = CharSet.Unicode)]
            public static extern nint ReverseColors(nint sym);
        }
    }

    public class TextureSystem {
        private const string DllSource =  @"" + DllHandle.Prefix + "factoryrushsharp" + DllHandle.Suffix;

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "TextureSystem_ImportText", CharSet = CharSet.Unicode)]
        public static extern IntPtr ImportText(IntPtr fileptr);

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "TextureSystem_ExportText", CharSet = CharSet.Unicode)]
        public static extern void ExportText(IntPtr pathptr, IntPtr contentptr);
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "TextureSystem_TextureFromFile", CharSet = CharSet.Unicode)]
        public static extern nint TextureFromFile(nint filepathPtr);
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "TextureSystem_FileFromTexture", CharSet = CharSet.Unicode)]
        public static extern void FileFromTexture(nint filepathPtr, nint texturePtr, bool recycle = false);
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "TextureSystem_DrawTextureToScreen", CharSet = CharSet.Unicode)]
        public static extern void DrawTextureToScreen(int x, int y, nint texturePtr, nint screenPtr);
    }

    public class SoundSystem {
        private const string DllSource =  @"" + DllHandle.Prefix + "factoryrushsharp" + DllHandle.Suffix;
        
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "SoundSystem_PlaySound", CharSet = CharSet.Unicode)]
        public static extern void PlaySound(nint filepathPtr, bool wait = false);
    }
}

namespace CppImp {

    public class System {
        private const string DllSource = @"" + DllHandle.Prefix + "factoryrushplus" + DllHandle.Suffix;
        

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_GetRootDir", CharSet = CharSet.Unicode)]
        public static extern IntPtr GetRootDir();
        
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ToNativePath", CharSet = CharSet.Unicode)]
        public static extern IntPtr ToNativePath(IntPtr arg1);
        
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_AllocateMemory", CharSet = CharSet.Unicode)]
        public static extern IntPtr AllocateMemory(nuint arg1);
        
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_FreeMemory", CharSet = CharSet.Unicode)]
        public static extern void FreeMemory(IntPtr arg1);
        
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_MovePointer", CharSet = CharSet.Unicode)]
        public static extern nint MovePointer(IntPtr arg1, Int32 arg2);

        // ReadPointer<T>
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$bool", CharSet = CharSet.Unicode)]
            public static extern bool ReadPointer_bool(nint arg1);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$nint", CharSet = CharSet.Unicode)]
            public static extern nint ReadPointer_nint(nint arg1);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$int8", CharSet = CharSet.Unicode)]
            public static extern sbyte ReadPointer_int8(nint arg1);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$int16", CharSet = CharSet.Unicode)]
            public static extern short ReadPointer_int16(nint arg1);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$int32", CharSet = CharSet.Unicode)]
            public static extern int ReadPointer_int32(nint arg1);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$int64", CharSet = CharSet.Unicode)]
            public static extern long ReadPointer_int64(nint arg1);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$uint8", CharSet = CharSet.Unicode)]
            public static extern byte ReadPointer_uint8(nint arg1);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$uint16", CharSet = CharSet.Unicode)]
            public static extern ushort ReadPointer_uint16(nint arg1);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$uint32", CharSet = CharSet.Unicode)]
            public static extern uint ReadPointer_uint32(nint arg1);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$uint64", CharSet = CharSet.Unicode)]
            public static extern ulong ReadPointer_uint64(nint arg1);

            // offset = true

            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$bool$ofs", CharSet = CharSet.Unicode)]
            public static extern bool ReadPointer_bool(nint arg1, int arg2);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$nint$ofs", CharSet = CharSet.Unicode)]
            public static extern nint ReadPointer_nint(nint arg1, int arg2);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$int8$ofs", CharSet = CharSet.Unicode)]
            public static extern sbyte ReadPointer_int8(nint arg1, int arg2);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$int16$ofs", CharSet = CharSet.Unicode)]
            public static extern short ReadPointer_int16(nint arg1, int arg2);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$int32$ofs", CharSet = CharSet.Unicode)]
            public static extern int ReadPointer_int32(nint arg1, int arg2);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$int64$ofs", CharSet = CharSet.Unicode)]
            public static extern long ReadPointer_int64(nint arg1, int arg2);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$uint8$ofs", CharSet = CharSet.Unicode)]
            public static extern byte ReadPointer_uint8(nint arg1, int arg2);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$uint16$ofs", CharSet = CharSet.Unicode)]
            public static extern ushort ReadPointer_uint16(nint arg1, int arg2);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$uint32$ofs", CharSet = CharSet.Unicode)]
            public static extern uint ReadPointer_uint32(nint arg1, int arg2);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_ReadPointer$uint64$ofs", CharSet = CharSet.Unicode)]
            public static extern ulong ReadPointer_uint64(nint arg1, int arg2);
        // ~ReadPointer<T>

        // WritePointer<T>
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$bool", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_bool(nint arg1, bool arg2);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$nint", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_nint(nint arg1, nint arg2);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$int8", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_int8(nint arg1, sbyte arg2);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$int16", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_int16(nint arg1, short arg2);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$int32", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_int32(nint arg1, int arg2);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$int64", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_int64(nint arg1, long arg2);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$uint8", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_uint8(nint arg1, byte arg2);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$uint16", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_uint16(nint arg1, ushort arg2);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$uint32", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_uint32(nint arg1, uint arg2);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$uint64", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_uint64(nint arg1, ulong arg2);

            // offset = true

            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$bool$ofs", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_bool(nint arg1, int arg2, bool arg3);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$nint$ofs", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_nint(nint arg1, int arg2, nint arg3);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$int8$ofs", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_int8(nint arg1, int arg2, sbyte arg3);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$int16$ofs", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_int16(nint arg1, int arg2, short arg3);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$int32$ofs", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_int32(nint arg1, int arg2, int arg3);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$int64$ofs", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_int64(nint arg1, int arg2, long arg3);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$uint8$ofs", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_uint8(nint arg1, int arg2, byte arg3);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$uint16$ofs", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_uint16(nint arg1, int arg2, ushort arg3);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$uint32$ofs", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_uint32(nint arg1, int arg2, uint arg3);
            
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "System_WritePointer$uint64$ofs", CharSet = CharSet.Unicode)]
            public static extern void WritePointer_uint64(nint arg1, int arg2, ulong arg3);
        // ~WritePointer<T>

    }

    public class Console
    {
        private const string DllSource = @"" + DllHandle.Prefix + "factoryrushplus" + DllHandle.Suffix;

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Init", CharSet = CharSet.Unicode)]
        public static extern void Init();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Fin", CharSet = CharSet.Unicode)]
        public static extern void Fin();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_HandleKeyboard", CharSet = CharSet.Unicode)]
        public static extern void HandleKeyboard();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_IsKeyDown", CharSet = CharSet.Unicode)]
        public static extern bool IsKeyDown(ushort arg1);

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_KeysToggled", CharSet = CharSet.Unicode)]
        public static extern byte KeysToggled();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_KeyPressed", CharSet = CharSet.Unicode)]
        public static extern ushort KeyPressed();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_KeyReleased", CharSet = CharSet.Unicode)]
        public static extern ushort KeyReleased();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_HandleMouseAndFocus", CharSet = CharSet.Unicode)]
        public static extern void HandleMouseAndFocus();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_GetMouseStatus", CharSet = CharSet.Unicode)]
        public static extern IntPtr GetMouseStatus();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_MouseButtonClicked$ret2", CharSet = CharSet.Unicode)]
        public static extern IntPtr MouseButtonClicked();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_MouseButtonReleased", CharSet = CharSet.Unicode)]
        public static extern byte MouseButtonReleased();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_IsFocused", CharSet = CharSet.Unicode)]
        public static extern bool IsFocused();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_FillScreen", CharSet = CharSet.Unicode)]
        public static extern void FillScreen(IntPtr arg1);

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_GetWindowWidth", CharSet = CharSet.Unicode)]
        public static extern short GetWindowWidth();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_GetWindowHeight", CharSet = CharSet.Unicode)]
        public static extern short GetWindowHeight();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_GetArgC", CharSet = CharSet.Unicode)]
        public static extern int GetArgC();

        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_GetArgV", CharSet = CharSet.Unicode)]
        public static extern IntPtr GetArgV();

        public struct Symbol {
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_Construct$smb", CharSet = CharSet.Unicode)]
            public static extern IntPtr Construct(IntPtr smb);
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_Construct$cfb", CharSet = CharSet.Unicode)]
            public static extern IntPtr Construct(UInt32 character, byte foreground = 7, byte background = 0);

        #if _WIN32
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_Construct$atr", CharSet = CharSet.Unicode)]
            public static extern IntPtr Construct(byte attribute);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_SetAttribute", CharSet = CharSet.Unicode)]
            public static extern void SetAttribute(IntPtr smb, byte attribute);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_GetAttribute", CharSet = CharSet.Unicode)]
            public static extern byte GetAttribute(IntPtr smb);
        #endif

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_character$set", CharSet = CharSet.Unicode)]
            public static extern void character(IntPtr smb, UInt32 character);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_character$get", CharSet = CharSet.Unicode)]
            public static extern UInt32 character(IntPtr smb);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_foreground$set", CharSet = CharSet.Unicode)]
            public static extern void foreground(IntPtr smb, byte foreground);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_foreground$get", CharSet = CharSet.Unicode)]
            public static extern byte foreground(IntPtr smb);
//                                                                                                                    $get(fu$ed)
            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_background$set", CharSet = CharSet.Unicode)]
            public static extern void background(IntPtr smb, byte background);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_background$get", CharSet = CharSet.Unicode)]
            public static extern byte background(IntPtr smb);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_Destruct", CharSet = CharSet.Unicode)]
            public static extern void Destruct(IntPtr smb);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_operator$eq", CharSet = CharSet.Unicode)]
            public static extern IntPtr operator_eq(IntPtr cp, IntPtr src);

            [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Console_Symbol_Inspect", CharSet = CharSet.Unicode)]
            public static extern void Inspect(IntPtr sym);
        }
    }
}