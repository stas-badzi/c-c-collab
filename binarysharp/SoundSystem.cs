using System.Runtime.InteropServices;
using Utility;
using Cpp;

namespace Cs {
    public class SoundSystem {
        public static void PlaySound(string filepath, bool wait = false)
        {
            CsImp.SoundSystem.PlaySound(TypeConvert.StringToPtr(filepath),wait);
        }
    }
}