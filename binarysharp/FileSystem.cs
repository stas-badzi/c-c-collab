using System.Runtime.InteropServices;

namespace Cs {
    public class FileSystem {
        public static unsafe string ImportText(string filestr) {
            char[] unfilestr = filestr.ToCharArray();
            fixed (char* fileptr = unfilestr) {
                return new String(CsImp.FileSystem.ImportText(fileptr));
            }
        }
        public static unsafe void ExportText(string pathstr, string contentstr) {
            char[] unpathstr = pathstr.ToCharArray();
            char[] uncontentstr = contentstr.ToCharArray();
            fixed (char* pathptr = unpathstr) {
                fixed (char* contentptr = uncontentstr) {
                    CsImp.FileSystem.ExportText(pathptr, contentptr);
                }
            }
        }
    }
}