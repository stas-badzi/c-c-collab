using Cpp;

namespace Cs
{
    public class Control {
        public static void CleanMemory() {
            Terminal.Symbol.DestructQueued();
        }
    }
}