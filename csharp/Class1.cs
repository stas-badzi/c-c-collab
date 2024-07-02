using System;
using System.Runtime.InteropServices;

namespace CppCsComTest {

    public class Class1 {

        public static int Add(int a, int b, int c) {
            return Class2.Add( a, Class2.Add(b,c) );
        }

        public static void Hello() {
            Class2.Hello();
            Console.WriteLine("Hello from C# on " + DllHandle.OS + ".");
        }
        
    }

    public class ExportClass1 {

        [UnmanagedCallersOnly(EntryPoint = "class1_add")]
        public static int Add(int a, int b, int c) {
            return Class1.Add(a,b,c);
        }

        [UnmanagedCallersOnly(EntryPoint = "class1_hello")]
        public static void Hello() {
            Class1.Hello();
        }
        
    }

}