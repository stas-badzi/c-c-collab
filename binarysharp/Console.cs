using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Reflection.Metadata;

namespace Cpp
{
    class Console
    {
        public ulong[] FillScreen(List<List<Symbol>> symbols) {
            int ptrsize = Marshal.SizeOf(typeof(IntPtr));
            int ulngsize = Marshal.SizeOf(typeof(ulong));
            int height = symbols.Count();
            int width = symbols[0].Count();

            List<IntPtr> list_ptr = new List<List<IntPtr>>(); // Convert 2xlist_Symbol to 2xlist_IntPtr
            for (int i = 0; i < height; i++)
            {
                list_ptr.Add(Marshal.AllocHGlobal(width));
                for (int j = 0; j < width; j++)
                {
                    IntPtr elem = Add(list,j);
                    Marshal.ReadIntPtr()
                }
                list_ptr.Add(list);
            }

            
            IntPtr ptr_ptr_smb = new IntPtr();

            // ptr_ptr_smb = (IntPtr) list_ptr_smb.to_array;

            IntPtr ulongints = CppImp.Console.FillScreen(ptr_ptr_smb, height, width); // Does the work itself

            Marshal.FreeHGlobal(ptr_ptr_smb);
            
            // ulongints -> ulongs
            ulong[] ulongs = new ulong[2];

            ulongs[0] = Marshal.ReadIntPtr(ulongints, 0);
            ulongs[1] = Marshal.ReadIntPtr(ulongints, ulngsize);

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