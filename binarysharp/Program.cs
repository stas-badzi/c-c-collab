using System;
using System.Runtime;
using System.Runtime.InteropServices;

namespace CsImp {
    public class FileSystem {
        private const string DllSource =  @"" + DllHandle.Prefix + "csharp" + DllHandle.Suffix;
        //import
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "FileSystem_ImportText")]
        public static extern IntPtr ImportText(IntPtr fileptr);
    }
}

namespace Cs {
    public class FileSystem {
        public static string ImportText(string filestr) {
            return Marshal.PtrToStringAuto(CsImp.FileSystem.ImportText(Marshal.StringToHGlobalUni(filestr)));
        }
    }

    public class Program {

        static void Main(string[] args) {
            string filename = Directory.GetCurrentDirectory() + @"/../../../csbin";
            Console.WriteLine("Acessing file: " + filename);
            Console.WriteLine(FileSystem.ImportText(filename));
        }

    }

}