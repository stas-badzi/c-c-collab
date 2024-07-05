using System;
using System.Runtime.InteropServices;

namespace CppCsComTest {
    public class FlImp {
        private const string DllSource =  @"" + DllHandle.Prefix + "csharp" + DllHandle.Suffix;
        //import
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "FileImport_ImportTxt")]
        public static extern IntPtr Imptxt(IntPtr file);
    }
    public class Program {

        static void Main(string[] args) {
            string file = "C:/Users/micha/Desktop/mswindows_script.txt";
            IntPtr strptr = Marshal.StringToHGlobalUni(file);
            Console.WriteLine(Marshal.PtrToStringAuto(FlImp.Imptxt(strptr)));
        }

    }

}