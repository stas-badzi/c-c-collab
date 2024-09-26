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

        public static void FillScreen(List<List<Symbol>> symbols) {
            nint sym = TypeConvert.TextureToPtr(symbols);
            CppImp.Console.FillScreen(sym);
        }

        public static short GetWindowWidth() {
            return CppImp.Console.GetWindowWidth();
        }

        public static short GetWindowHeight() {
            return CppImp.Console.GetWindowHeight();
        }

        public class Symbol
        {
            private static List<IntPtr> deleteQueue = new List<IntPtr>();

            public static void DestructQueued() {
                for (var i = 0; i < deleteQueue.Count; i++) {
                    CppImp.Console.Symbol.Destruct(deleteQueue[i]);
                }
                deleteQueue.Clear();
            }

            ~Symbol() {
                if (this.Armed)
                    Symbol.deleteQueue.Add(this.symbol);
            }

            // be carefull not to use the struct after it hes been destructed
            public void Destruct() {
                if (this.Armed)
                    CppImp.Console.Symbol.Destruct(symbol);
                this.Armed = false;
            }

            public void ArmPointer() {
                this.Armed = true;
            }

            public void UnarmPointer() {
                this.Armed = false;
            }

            public unsafe void Set(Symbol cp) {
                this.Destruct();
                this.symbol = CppImp.Console.Symbol.Construct(symbol);
                this.ArmPointer();
            }

            public void Set(nint sym, bool direct = false) {
                this.Destruct();
                this.symbol = direct ? sym : CppImp.Console.Symbol.Construct(sym);
                this.Armed = !direct;
            }

            public void Set(char character, byte foreground = 7, byte background = 0) {
                this.Destruct();
                this.symbol = CppImp.Console.Symbol.Construct(TypeConvert.Utf8ToUnicode(character),foreground,background);
                this.ArmPointer();
            }
        #if _WIN32
            public void Set(byte atr = 0x0000) {
                this.Destruct();
                this.symbol = CppImp.Console.Symbol.Construct(atr);
                this.ArmPointer();
            }
        #endif

            public Symbol() {
                symbol = CppImp.Console.Symbol.Construct(' ');
                this.ArmPointer();
            }

            public Symbol(Symbol cp) {
                symbol = CppImp.Console.Symbol.Construct(cp.symbol);
                this.ArmPointer();
            }

            // Direct = do not copy vaues, copy ptr
            public Symbol(nint sym, bool direct = false) {
                symbol = direct ? sym : CppImp.Console.Symbol.Construct(sym);
                Armed = !direct;
            }
        #if _WIN32
            public Symbol(byte atr = 0x0000) {
                symbol = CppImp.Console.Symbol.Construct(atr);
                this.ArmPointer();
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
                this.ArmPointer();
            }

            // to edit a Symbol (Console::Symbol too, from;)
                // too.Destruct(); too = new Symbol(from);
           
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
                Cs.Console.Symbol.ReverseColors(new Symbol(symbol, true));
            }

            public nint Get() {
                return symbol;
            }
            
            private IntPtr symbol;
            private Boolean Armed;
        }
    }    
}