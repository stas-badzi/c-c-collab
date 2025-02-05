using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using Microsoft.VisualBasic.FileIO;
using Cs;
using Cpp;
using Texture = System.Collections.Generic.List<System.Collections.Generic.List<Cpp.Terminal.Symbol>>;

namespace Cs {
    public class Camera {
        public Texture buffer { get; }
        public Tuple<int, int> viewportCenter { get; }

        public void DrawTextureToCamera(Texture texture, Tuple<int, int> center) {
            
        }
    }
}