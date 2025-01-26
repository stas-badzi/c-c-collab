using System;
using Microsoft.VisualBasic.FileIO;
using Cpp;
using Cs;
using System.Collections.Generic;
using Texture = System.Collections.Generic.List<System.Collections.Generic.List<Cpp.Terminal.Symbol>>;

namespace Cs {
    public class Camera {
        public Texture buffer { get; }
        public Tuple<int, int> viewportCenter { get; } // <height index, width index>

        public Camera(int width, int height, Terminal.Symbol symbol) {
            if (height % 2f == 0 || width % 2f == 0)
                throw new ArgumentException("Camera does not have a middle point because either height or width is even");
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
        public static Camera FromTexture(Texture texture) {
            float height = (float)texture.Count;
            float width = (float)texture[0].Count;
            if (height % 2f == 0 || width % 2f == 0)
                throw new ArgumentException("Texture does not have a middle point because either height or width is even");

            var vpC = new Tuple<int, int>((int)Math.Ceiling(height / 2f), (int)Math.Ceiling(width / 2f));

            return new Camera(texture, vpC);
        }
        public void DrawTextureToCamera(Texture texture, Tuple<int, int> center) {  
            return;

            float height = (float)texture.Count;
            float width = (float)texture[0].Count;
            if (height % 2f == 0 || width % 2f == 0)
                throw new ArgumentException("Texture does not have a middle point because either height or width is even");

            for (int i = 0; i < this.buffer.Count; i++) {
                for (int j = 0; j < this.buffer[i].Count; j++) {
                    this.buffer[i][j] = new Terminal.Symbol(" ", 16, 16);
                }
            }

            for (int i = center.Item1 + (int)Math.Floor(height/2f); i < height; i++) {
                if (i > 0) {
                    int placeholder1 = Math.Abs(i);
                    i = 0;
                }
                for (int j = center.Item2 + (int)Math.Floor(width/2f); j < width; j++) {
                    if (j > 0) {
                        int placeholder2 = Math.Abs(j);
                        j = 0;
                    }

                    this.buffer[i][j] = texture[i + placeholder1][j + placeholder2];
                }
            }
            /*
            i = middleY + floor(textureYSize / 2) or 0
            if i > 0:
                placeholder1 = abs(i)
                i = 0
                
            j = middleX + floor(textureXSize / 2) or 0
            if j > 0:
                placeholder2 = abs(j)
                j = 0

            buffer[i][j] = texture[i + placeholder1][j + placeholder2]
            */
        }
    }
}