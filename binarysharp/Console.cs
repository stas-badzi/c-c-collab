using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Reflection.Metadata;

namespace Cpp
{
    class Console
    {
        public ulong[] FillScreen(List<List<Symbol>> symbols) {
            int height = symbols.Count();
            int width = symbols[0].Count();
            // List[0][0].Get() List[0][1].Get() ...
            // real functions
            List<List<IntPtr>> list_list_smb;

            // (IntPtr)(List[0].to_array) (IntPtr)(List[1].to_array) ...
            // not real functions
            List<IntPtr> list_ptr_smb;
            
            // (IntPtr)(List.to_array)
            // same as above
            IntPtr ptr_ptr_smb = new IntPtr();

            IntPtr ulongints = CppImp.Console.FillScreen(ptr_ptr_smb, symbols.Count(), symbols[0].Count());
            
            // (IntPtr to array, from index 0 to 1 {size 2})
            ulong[] ulongs = new ulong[2];

            return ulongs;
        }

        public short GetWindowWidth() {
            return CppImp.Console.GetWindowWidth();
        }

        public short GetWindowHeight() {
            return CppImp.Console.GetWindowHeight();
        }

        public class Symbol
        {
            public Symbol() {
                symbol = CppImp.Console.Symbol.Construct(' ');
            }

            public Symbol(char character, byte foreground = 7, byte background = 0) {
                symbol = CppImp.Console.Symbol.Construct(character,foreground,background);
            }

            public Symbol(Symbol cp) {
                symbol = CppImp.Console.Symbol.Construct(cp.symbol);
            }

            ~Symbol() {
                CppImp.Console.Symbol.Destruct(symbol);
            }

            public char character() {
                return CppImp.Console.Symbol.character(symbol);
            }

            public void character(char val) {
                CppImp.Console.Symbol.character(symbol, val);
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

            public IntPtr Get() {
                return symbol;
            }

        #if _WIN32
            public byte GetAttribute() {
                return CppImp.Console.Symbol.GetAttribute(symbol);
            }
            public void SetAttribute(byte val) {
                CppImp.Console.Symbol.SetAttribute(symbol, val);
            }
        #endif
            private readonly IntPtr symbol;
        }
    }    
}