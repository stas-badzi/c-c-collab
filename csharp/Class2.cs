using System;
using System.Runtime.InteropServices;

namespace CppCsComTest {

    public class Class2 {
        private const string DllSource =  @"" + DllHandle.Prefix + "cplusplus" + DllHandle.Suffix;
        //import
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "class2_hello")]
        public static extern void Hello();
        //import
        [DllImport(DllSource, CallingConvention = CallingConvention.Cdecl, EntryPoint = "class2_add")]
        public static extern int Add(int a, int b);

    }

}