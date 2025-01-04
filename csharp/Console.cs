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
        public static void PlaySound(string filepath, bool wait = false) {
            string fullpath = Path.GetFullPath(filepath);
            Player player = new Player();
            player.Play(filepath);
            TimeSpan shorttime = new TimeSpan(1);
            while (wait && player.Playing) Thread.Sleep(shorttime);
        }   
    }
}