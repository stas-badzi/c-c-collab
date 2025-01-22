using System;
using Microsoft.VisualBasic.FileIO;
using Cpp;
using System.Collections.Generic;
using Texture = System.Collections.Generic.List<System.Collections.Generic.List<Cpp.Terminal.Symbol>>;

namespace Cs {
    public class Tile { // Texture struct
        public Texture buffer;
        public string id;
        public Tile(string id, Texture buffer) {
            this.id = id;
            this.buffer = buffer;
        }
    }
}