using System;
using System.Runtime.InteropServices;

namespace CsImp {
    public class FileImport {
        private const string DllSource =  @"" + DllHandle.Prefix + "csharp" + DllHandle.Suffix;
        //import
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "FileImport_ImportTxt")]
        public static extern IntPtr ImportText(IntPtr file);
    }
}

namespace Cs {
    public class FileImport {
        public static string ImportText(string file) {
            return Marshal.PtrToStringAuto(CsImp.FileImport.ImportText(Marshal.StringToHGlobalUni(file)))
        }
    }

    public class Program {

        static void Main(string[] args) {
            string filename = "C:/Users/micha/Desktop/mswindows_script.txt";
            Console.WriteLine(FileImport.ImportText(filename));
        }

    }

}