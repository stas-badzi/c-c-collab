using System;
using System.Runtime.InteropServices;

namespace CppCsComTest {

    public class Class3 {
        //export
        [UnmanagedCallersOnly(EntryPoint = "class3_add")]
        public static int Add(int a, int b, int c, int d) {
            return Class2.Add( a, Class1.Add(b,c,d) );
        }
        //export
        [UnmanagedCallersOnly(EntryPoint = "class3_hello")]
        public static void Hello() {
            Class1.Hello();
            Console.WriteLine("Hello again. [C#]");
            return;
        }
    }

}