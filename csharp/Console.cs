using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Reflection.Metadata;
using Utility;
using Cpp;

namespace Cs
{
    public class Console {
        public class Symbol {
            public static void ReverseColors(Terminal.Symbol sym) {
                byte back = sym.background();
                sym.background(sym.foreground());
                sym.foreground(back);
                return;
            }
        }
    }
}