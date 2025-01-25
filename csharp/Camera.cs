using System;
using Microsoft.VisualBasic.FileIO;
using Cpp;
using Cs;
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

        public Camera(int width, int height, Terminal.Symbol symbol) {
            if (height % 2f == 0 || width % 2f == 0)
                throw new ArgumentException("Change does not have a middle point because either height or width is even");
            var buffer = new Texture();
            for (int i = 0; i < height; i++) {
                buffer.Add(new List<Terminal.Symbol>());
                for (int j = 0; j < width; j++) {
                    buffer[i].Add(symbol);
                }
            }
            var vpC = new Tuple<int, int>((int)Math.Ceiling(height / 2f), (int)Math.Ceiling(width / 2f));

            this.buffer = buffer;
            this.viewportCenter = vpC;
        }
        private Camera(Texture buffer, Tuple<int, int> viewportCenter) {
            this.buffer = buffer;
            this.viewportCenter = viewportCenter;
        }
        public Camera FromTexture(Texture texture) {
            float height = (float)texture.Count;
            float width = (float)texture[0].Count;
            if (height % 2f == 0 || width % 2f == 0)
                throw new ArgumentException("Texture does not have a middle point");

            var vpC = new Tuple<int, int>((int)Math.Ceiling(height / 2f), (int)Math.Ceiling(width / 2f));

            return new Camera(texture, vpC);
        }
    }
}