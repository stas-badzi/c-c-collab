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
        public Tuple<int, int> viewportCenter { get; } // <height index, width index>

        public Camera(int width, int height, Terminal.Symbol symbol) { // EntryPoint = Camera_Construct
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
        public Camera(nint cameraPtr) { // For DllExport
            int offset = 0
            var int32_size = sizeof(int);
            var IntPtr_size = IntPtr.Size;

            Texture buffer;
            Tuple<int, int> viewportCenter;

            int height = Marshal.ReadInt32(cameraPtr, offset);
            offset += int32_size;

            buffer = new List<List<Terminal.Symbol>>();

            for (int i = 0; i < height; i++) {
                buffer.Add(new List<Terminal.Symbol>);
                int width = Marshal.ReadInt32(cameraPtr, offset);
                offset += int32_size;
                for (int j = 0; j < width; j++) {
                    buffer.Add(new Terminal.Symbol(Marshal.ReadIntPtr(cameraPtr, offset)));
                    offset += IntPtr_size;
                }
            }

            viewportCenter = new Tuple<int, int>(Marshal.ReadInt32(cameraPtr, offset), Marshal.ReadInt32(cameraPtr, offset + int32_size));

            this.buffer = buffer;
            this.viewportCenter = viewportCenter;

            Marshal.FreeHGlobal(cameraPtr);
        }
        private Camera(Texture buffer, Tuple<int, int> viewportCenter) { // For internal conversion
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
        public IntPtr Get() { // For DllImport
            int alloc = 0;
            var int32_size = sizeof(int);
            var IntPtr_size = IntPtr.Size;

            alloc += 3 * int32_size; // buffer.Count + viewportCenter
            foreach (var row in this.buffer) {
                alloc += int32_size + row.Count * IntPtr_size; // buffer[i].Count + buffer[i]
            }

            int offset = 0;
            IntPtr ret = Marshal.AllocHGlobal(alloc);

            Marshal.WriteInt32(ret, offset, this.buffer.Count);
            offset += int32_size;
            foreach (var row in this.buffer) {
                Marshal.WriteInt32(ret, offset, row.Count);
                offset += int32_size;
                foreach (var symbol in row) {
                    Marshal.WriteIntPtr(ret, offset, symbol.Get());
                    offset += IntPtr_size;
                }
            }

            Marshal.WriteInt32(ret, offset, this.viewportCenter.Item1);
            offset += int32_size;

            Marshal.WriteInt32(ret, offset, this.viewportCenter.Item2);
            offset += int32_size;
            // allocate memory for:
            // buffer height and widths
            // buffer symbols
            // both viewport values
            return ret;
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