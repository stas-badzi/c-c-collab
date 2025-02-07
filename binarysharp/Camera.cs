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
    }
}