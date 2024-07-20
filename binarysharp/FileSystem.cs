using System.Runtime.InteropServices;

namespace Cs {
    public class FileSystem {
        public static string ImportText(string filestr) {
            String^ unfilestr = gcnew String(filestr);
            return new String(CsImp.FileSystem.ImportText(unfilestr));
        }
        public static void ExportText(string pathstr, string contentstr) {
            String^ unpathstr = gcnew String(pathstr);
            String^ uncontentstr = gcnew String(contentstr);
            CsImp.FileSystem.ExportText(unpathstr, uncontentstr);
        }
    }
}