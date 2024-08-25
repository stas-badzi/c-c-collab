using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Reflection.Metadata;
using Utility;

namespace Cpp
{
    public class Terminal
    {
        public static void Init() {
            CppImp.Terminal.Init();
        }

        public static void Fin() {
            CppImp.Terminal.Fin();
        }

        public static int HandleKeyboard() {
            return CppImp.Terminal.HandleKeyboard();
        }

        public static bool KeyDown(int key) {
            return CppImp.Terminal.KeyDown(key);
        }

        public static bool KeyToggled(int key) {
            return CppImp.Terminal.KeyToggled(key);
        }

        public static bool KeyHit(int key) {
            return CppImp.Terminal.KeyHit(key);
        }

        public static bool KeyReleased(int key) {
            return CppImp.Terminal.KeyReleased(key);
        }

        public static ulong[] FillScreen(List<List<Symbol>> symbols) {
            
            int ptrsize = IntPtr.Size;
            int ulngsize = sizeof(ulong);
            int height = symbols.Count();
            int width = symbols[0].Count();

            List<IntPtr> list_ptr = new List<IntPtr>(); // Convert 2xlist_Symbol to 2xlist_IntPtr
            for (int i = 0; i < height; i++)
            {
                list_ptr.Add(Marshal.AllocHGlobal(width * ptrsize));
                for (int j = 0; j < width; j++)
                {
                    Marshal.WriteIntPtr(list_ptr[i], j * ptrsize, symbols[i][j].Get());
                }
            }

            IntPtr ptr_smb = Marshal.AllocHGlobal(height * ptrsize);
            
            for (int i = 0; i < height; i++)
            {
                Marshal.WriteIntPtr(ptr_smb, i * ptrsize, list_ptr[i]);
            }

            // ptr_ptr_smb = (IntPtr) list_ptr_smb.to_array;

            IntPtr ulongints = CppImp.Terminal.FillScreen(ptr_smb, height, width); // Does the work itself

            Marshal.FreeHGlobal(ptr_smb);

            for (int i = 0; i < height; i++)
            {
                Marshal.FreeHGlobal(list_ptr[i]);
            }
            
            // ulongints -> ulongs
            ulong[] ulongs = new ulong[2];

            ulongs[0] = Convert.ToUInt64(Marshal.ReadInt32(ulongints, 0));
            ulongs[1] = Convert.ToUInt64(Marshal.ReadInt32(ulongints, ulngsize));

            // ???

            return ulongs;
        }

        public static short GetWindowWidth() {
            return CppImp.Terminal.GetWindowWidth();
        }

        public static short GetWindowHeight() {
            return CppImp.Terminal.GetWindowHeight();
        }

        public class Symbol
        {
            ~Symbol() {
                CppImp.Terminal.Symbol.Destruct(symbol);
            }

            public Symbol() {
                symbol = CppImp.Terminal.Symbol.Construct(' ');
            }

            public Symbol(Symbol cp) {
                symbol = CppImp.Terminal.Symbol.Construct(cp.symbol);
            }

            // Direct = do not copy vaues, copy ptr
            public Symbol(nint sym, bool direct = false) {
                if (direct) symbol = sym;
                else symbol = CppImp.Terminal.Symbol.Construct(sym);
            }
        #if _WIN32
            public Symbol(byte atr = 0x0000) {
                symbol = CppImp.Terminal.Symbol.Construct(atr);
            }
            public void GetAttribute(byte atr) {
                CppImp.Terminal.Symbol.SetAttribute(symbol, atr);
            }
            public byte GetAttribute() {
                return CppImp.Terminal.Symbol.GetAttribute(symbol);
            }
        #endif

            public Symbol(char character, byte foreground = 7, byte background = 0) {
                symbol = CppImp.Terminal.Symbol.Construct(TypeConvert.Utf8ToUnicode(character),foreground,background);
            }

            public char character() {
                return TypeConvert.UnicodeToUtf8(CppImp.Terminal.Symbol.character(symbol));
            }

            public void character(char val) {
                CppImp.Terminal.Symbol.character(symbol, TypeConvert.Utf8ToUnicode(val));
            }

            public byte foreground() {
                return CppImp.Terminal.Symbol.foreground(symbol);
            }

            public void foreground(byte val) {
                CppImp.Terminal.Symbol.foreground(symbol, val);
            }

            public byte background() {
                return CppImp.Terminal.Symbol.background(symbol);
            }

            public void background(byte val) {
                CppImp.Terminal.Symbol.background(symbol, val);
            }

            public void ReverseColors() {
                CsImp.Terminal.Symbol.ReverseColors(symbol);
            }

            public IntPtr Get() {
                return symbol;
            }
            
            private readonly IntPtr symbol;
        }
    }    
}