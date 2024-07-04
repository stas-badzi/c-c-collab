using System;
using System.Runtime.InteropServices;

namespace CppCsComTest {
    public class FlImp {
        private const string DllSource =  @"" + DllHandle.Prefix + "csharp" + DllHandle.Suffix;
        //import
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "FileImport_ImportTxt")]
        public static extern void Imptxt();
    }
    public class Program {

        static void Main(string[] args) {
            /*Class3.Hello();
            Console.WriteLine("Hello from binary.");*/
            Console.WriteLine("Hello from binarysharp.");
            FlImp.Imptxt();
        }

    }

}