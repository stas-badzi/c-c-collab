﻿using System.Collections.Generic;
using System;
using Cpp;
using Cs;
using Utility;

namespace BinarySharp {
    public class Program {
        static int Main(string[] args) {
            Terminal.Init();
            var sym = new Terminal.Symbol('▒',16,16);
            bool running = true;
            while (running) {
                var width = Terminal.GetWindowWidth();
                var height = Terminal.GetWindowHeight();

                var screen = new List<List<Terminal.Symbol>>();

                for (int h = 0; h < height; h++) {
                    screen.Add(new List<Terminal.Symbol>());
                    for (int w = 0; w < width; w++) {
                        screen[h].Add(sym);
                    }
                }
                
                Terminal.HandleMouseAndFocus();
                var mouse = Terminal.GetMouseStatus();
                Terminal.FillScreen(screen);

                if (Terminal.IsFocused()) Terminal.HandleKeyboard();

                //if (Terminal.KeyPressed() == Key.Enum.ESC) return 0;
                switch (Terminal.KeyPressed()) {
                    case Key.Enum.ESC:
                        running = false;
                        break;
                    case Key.Enum.a:
                        sym = new Terminal.Symbol('A',15,15);
                        break;
                    case Key.Enum.b:
                        sym = new Terminal.Symbol('▒',16,16);
                        break;
                    case Key.Enum.s:
                        SoundSystem.PlaySound(GetSoundPath("shoot.wav"), false);
                        break;
                }
            }
            return 0;
        }
        private static string GetSoundPath(string filename) {
            return Path.Combine([AppContext.BaseDirectory,"..","sounds",filename]);
        }
    }
}