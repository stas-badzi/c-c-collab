using System;
using System.Runtime.InteropServices;

namespace CppCsComTest {
    public class Class3 {
        private const string DllSource =  @"" + DllHandle.Prefix + "csharp" + DllHandle.Suffix;
        //import
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "class3_hello")]
        public static extern void Hello();
        //import
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "class3_add")]
        public static extern int Add(int a, int b);
    }
    public class Program {

        static void Main(string[] args) {
            Class3.Hello();
            Console.WriteLine("Hello from binary.");
        }

    }

}