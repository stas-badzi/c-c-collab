using System.Runtime.InteropServices;
using Utility;

namespace Cs {
    public class FileSystem {
        public static List<String> ImportText(String filestr) {
            IntPtr ret = CsImp.FileSystem.ImportText(UniConv.StringToPtr(filestr));
            int intptr_size = IntPtr.Size;
            List<String> text = new List<String>();

            IntPtr elem = Marshal.ReadIntPtr(ret, 0);
            String line = UniConv.PtrToString(elem);
            for (int i = 1; line.Length > 0; i++) {
                text.Append(line);
                elem = Marshal.ReadIntPtr(ret, i * intptr_size);
                line = UniConv.PtrToString(elem);
            }
            return text;
        }
        public static void ExportText(string pathstr, List<string> contentstr) {
            int intptr_size = IntPtr.Size;
            IntPtr output = Marshal.AllocHGlobal( (contentstr.Count + 1) * intptr_size);
            for (int i = 0; i < contentstr.Count; i++) {
                IntPtr elem = UniConv.StringToPtr(contentstr[i]);
                Marshal.WriteIntPtr(output, intptr_size * i, elem);
            }
            IntPtr end = UniConv.StringToPtr("\u0000");
            CsImp.FileSystem.ExportText(UniConv.StringToPtr(pathstr), output);
        }
    }
}