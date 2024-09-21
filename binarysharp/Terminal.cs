using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Reflection.Metadata;
using Utility;

namespace Cpp
{
    public class Terminal
    {
        public static void Init() {
            CppImp.Console.Init();
        }

        public static void Fin() {
            CppImp.Console.Fin();
        }

        public static int HandleKeyboard() {
            return CppImp.Console.HandleKeyboard();
        }

        public static bool IsKeyDown(int key) {
            return CppImp.Console.IsKeyDown(key);
        }

        public static bool IsKeyToggled(int key) {
            return CppImp.Console.IsKeyToggled(key);
        }

        public static int KeyPressed() {
            return CppImp.Console.KeyPressed();
        }

        public static int KeyReleased() {
            return CppImp.Console.KeyReleased();
        }

        public static ulong[] FillScreen(List<List<Symbol>> symbols) {
            
            int ptrsize = IntPtr.Size;
            int ulngsize = sizeof(ulong);
            int height = symbols.Count();
            int width = symbols[0].Count();

            List<IntPtr> list_ptr = new List<IntPtr>(); // Convert 2xlist_Symbol to 2xlist_IntPtr
            for (int i = 0; i < height; i++)
            {
                list_ptr.Add(Exec.AllocateMemory((nuint)(width * ptrsize)));
                for (int j = 0; j < width; j++)
                {
                    Exec.WritePointer<IntPtr>(list_ptr[i], j * ptrsize, symbols[i][j].Get());
                }
            }

            IntPtr ptr_smb = Exec.AllocateMemory((nuint)(height * ptrsize));
            
            for (int i = 0; i < height; i++)
            {
                Exec.WritePointer<IntPtr>(ptr_smb, i * ptrsize, list_ptr[i]);
            }

            // ptr_ptr_smb = (IntPtr) list_ptr_smb.to_array;

            IntPtr ulongints = CppImp.Console.FillScreen(ptr_smb, height, width); // Does the work itself

            Exec.FreeMemory(ptr_smb);

            for (int i = 0; i < height; i++)
            {
                Exec.FreeMemory(list_ptr[i]);
            }
            
            // ulongints -> ulongs
            ulong[] ulongs = new ulong[2];

            ulongs[0] = Convert.ToUInt64(Exec.ReadPointer<Int32>(ulongints, 0));
            ulongs[1] = Convert.ToUInt64(Exec.ReadPointer<Int32>(ulongints, ulngsize));

            Exec.FreeMemory(ulongints);

            // ???

            return ulongs;
        }

        public static short GetWindowWidth() {
            return CppImp.Console.GetWindowWidth();
        }

        public static short GetWindowHeight() {
            return CppImp.Console.GetWindowHeight();
        }

        public class Symbol
        {
            ~Symbol() {
                CppImp.Console.Symbol.Destruct(symbol);
            }

            public Symbol() {
                symbol = CppImp.Console.Symbol.Construct(' ');
            }

            public Symbol(Symbol cp) {
                symbol = CppImp.Console.Symbol.Construct(cp.symbol);
            }

            // Direct = do not copy vaues, copy ptr
            public Symbol(nint sym, bool direct = false) {
                if (direct) symbol = sym;
                else symbol = CppImp.Console.Symbol.Construct(sym);
            }
        #if _WIN32
            public Symbol(byte atr = 0x0000) {
                symbol = CppImp.Console.Symbol.Construct(atr);
            }
            public void GetAttribute(byte atr) {
                CppImp.Console.Symbol.SetAttribute(symbol, atr);
            }
            public byte GetAttribute() {
                return CppImp.Console.Symbol.GetAttribute(symbol);
            }
        #endif

            public Symbol(char character, byte foreground = 7, byte background = 0) {
                symbol = CppImp.Console.Symbol.Construct(TypeConvert.Utf8ToUnicode(character),foreground,background);
            }

            public char character() {
                return TypeConvert.UnicodeToUtf8(CppImp.Console.Symbol.character(symbol));
            }

            public void character(char val) {
                CppImp.Console.Symbol.character(symbol, TypeConvert.Utf8ToUnicode(val));
            }

            public byte foreground() {
                return CppImp.Console.Symbol.foreground(symbol);
            }

            public void foreground(byte val) {
                CppImp.Console.Symbol.foreground(symbol, val);
            }

            public byte background() {
                return CppImp.Console.Symbol.background(symbol);
            }

            public void background(byte val) {
                CppImp.Console.Symbol.background(symbol, val);
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