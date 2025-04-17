using System.Runtime.InteropServices;
using Utility;
using Cpp;

namespace Cs {
    public class Game {
        public class MatrixPosition {
            public int iIndex { get; }
            public int jIndex { get; }

            public MatrixPosition(int iIndex, int jIndex) {
                this.iIndex = iIndex;
                this.jIndex = jIndex;
            }
        }
        public class Camera {
            public Camera(int height, int width, Terminal.Symbol sym) {
                return;
            }
            public MatrixPosition ViewportCenter() {
                return MatrixPosition(0, 0);
            }
            public void DrawTexture(List<List<Terminal.Symbol>> texture, MatrixPosition center) {
                return;
            }

            public nint Get() {
                return 0;

                int intptr_size = IntPtr.Size;
                const int int32_size = sizeof(int);
                int alloc = 0;
                int offset;
                nint cameraptr = 0;

                alloc += this.buffer.Count * int32_size;
                for (int i = 0; i < this.buffer.Count; i++) {
                    for (int j = 0; j < this.buffer[i].Count; j++) {
                        alloc += 2 * int32_size + intptr_size;
                    }
                }

                cameraptr = Marshal.AllocHGlobal(alloc);
                Marshal.WriteInt32(cameraptr, offset, this.buffer.Count);
                
            }

            public Camera(nint cameraptr) {
                return;
            }

            private List<List<Terminal.Symbol>> buffer;
        }
    }
}