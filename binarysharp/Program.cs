using System.Collections.ObjectModel;
using System.ComponentModel.DataAnnotations;
using System.Collections.Generic;
using System;
using Cpp;
using CppImp;
using Cs;
using Utility;

namespace BinarySharp {
    public class Program {
        static int Main(string[] args) {
            Terminal.Init();
            while (true) {
                var width = Terminal.GetWindowWidth();
                var height = Terminal.GetWindowHeight();

                var screen = new List<List<Terminal.Symbol>>();

                for (int h = 0; h < height; h++) {
                    screen.Add(new List<Terminal.Symbol>());
                    for (int w = 0; w < width; w++) {
                        var sym = new Terminal.Symbol('X',16,16);
                        screen[h].Add(sym);
                    }
                }

                Terminal.HandleMouseAndFocus();
                var mouse = Terminal.GetMouseStatus();

                Terminal.FillScreen(screen);

                if (Terminal.IsFocused()) Terminal.HandleKeyboard();

                if (Terminal.KeyPressed() == Key.Enum.ESC) return 0;
                //if (Terminal.IsKeyDown(Key.Enum.ESC))
                    //break;
            }
            return 0;
        }
    }
}