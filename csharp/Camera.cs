using System;
using Microsoft.VisualBasic.FileIO;
using Cpp;
using System.Collections.Generic;
using Texture = System.Collections.Generic.List<System.Collections.Generic.List<Cpp.Terminal.Symbol>>;

namespace Cs {
    public class Tile { // Texture struct
        public Texture texture;
        public string id;
        public Tile(string id, Texture texture) {
            this.id = id;
            this.texture = texture;
        }
    }
    public class Camera {
        public Texture buffer { get; }
        public Tuple<int, int> viewportCenter { get; } // <height index, width index>

        private Camera(Texture buffer, Tuple<int, int> viewportCenter) {
            this.buffer = buffer;
            this.viewportCenter = viewportCenter;
        }
        public Camera FromTexture(Texture texture) {
            height = float.Parse(texture.Count);
            width = float.Parse(texture[0].Count);
            if (height % 2f == 0 || width % 2f == 0)
                throw new ArgumentException("Texture does not have a middle point")

            var vpC = new Tuple<int, int>(Math.Ceiling(int.Parse(height/2f)), Math.Ceiling(int.Parse(width/2f)));

            return new Camera(texture, vpC)
        }
    }
}