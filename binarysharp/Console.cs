using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Reflection.Metadata;

namespace Cpp
{
    class Console
    {
        class Symbol
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

            private IntPtr symbol;
        }
    }    
}