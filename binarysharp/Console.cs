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

            List<List<IntPtr>> list_list_smb = new(); // Convert 2xlist_Symbol to 2xlist_IntPtr
            for (int i = 0; i < symbols.Count(); i++)
            {
                for (int j = 0; j < symbols[i].Count(); j++)
                {
                    list_list_smb[i].Add(symbols[i][j].Get()); // list_list_smb[i] is null -> potential OutOfRangeException
                }
            }

            List<IntPtr> list_ptr_smb;
            for (int i = 0; i < list_list_smb.Count(); i++)
            {
                IntPtr[] buffer = list_list_smb[i].ToArray(); // jeszcze nie działa
                unsafe
                {
                fixed (IntPtr* p = buffer)
                {
                    IntPtr ptr = (IntPtr)p;
                    // wtf jakas funkcja turecka
                }
                }
                // |
                // V
                // list_ptr_smb.Add((IntPtr) list_list_smb[i].to_array);
            }
            
            IntPtr ptr_ptr_smb = new IntPtr();

            // ptr_ptr_smb = (IntPtr) list_ptr_smb.to_array;

            IntPtr ulongints = CppImp.Console.FillScreen(ptr_ptr_smb, symbols.Count(), symbols[0].Count()); // Does the work itself
            
            // ulongints -> ulongs
            ulong[] ulongs = new ulong[2];

            // ???

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