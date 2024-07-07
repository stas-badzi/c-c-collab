using System.Runtime.InteropServices;

namespace Cs {
    public class FileSystem {
        public static string ImportText(string filestr) {
            return Marshal.PtrToStringUni(CsImp.FileSystem.ImportText(Marshal.StringToHGlobalUni(filestr)));
        }
        public static void ExportText(string pathstr, string contentstr) {
            CsImp.FileSystem.ExportText(Marshal.StringToHGlobalUni(pathstr), Marshal.StringToHGlobalUni(contentstr));
        }
    }
}