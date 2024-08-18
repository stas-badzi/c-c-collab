using System.Runtime.InteropServices;

namespace Utility
{
    public class DllHandle
    {

    #if _WIN32
        public const string Prefix = "";
        public const string Suffix = ".dll";
    #elif __APPLE__
        public const string Prefix = "lib";
        public const string Suffix = ".dylib";
    #elif __linux__
        public const string Prefix = "lib";
        public const string Suffix = ".so";
    #elif __FreeBSD__
        public const string Prefix = "lib";
        public const string Suffix = ".so";
    #else
        public const string Prefix = "";
        public const string Suffix = "";
    #endif


    #if _WIN32
        public const string OS = "Windows";
    #elif __APPLE__
        public const string OS = "Apple";
    #elif __linux__
        public const string OS = "Linux";
    #elif __FreeBSD__
        public const string OS = "FreeBSD";
    #else
        public const string OS = "Unknown";
    #endif

    }

    class UniConv
    {
        public static char UnicodeToUtf8(Int32 uni) {
            return Convert.ToChar(uni);
        }

        public static Int32 Utf8ToUnicode(char utf) {
            return Convert.ToInt32(utf);
        }
        public static String PtrToString(IntPtr ptr)
        {
        #if _WIN32
            string output = Marshal.PtrToStringUni(ptr);
            Marshal.FreeHGlobal(ptr);
            return output;
        #else
            int int32_size = sizeof(Int32);

            String str = "";
            Int32 intg = Marshal.ReadInt32(ptr,0);
            for (int i = 0; true; i++) {
                intg = Marshal.ReadInt32(ptr,i*int32_size);
                if (intg == 0) {
                    break;
                }
                str += Convert.ToChar(intg);
            }
            Marshal.FreeHGlobal(ptr);
            return str;
        #endif
        }

        public static IntPtr StringToPtr(String str)
        {
    #if _WIN32
            return Marshal.StringToHGlobalUni(str);
    #else
            int int32_size = sizeof(Int32);

            IntPtr ptr = Marshal.AllocHGlobal( (str.Length + 1) * int32_size);
            for (int i = 0; i < str.Length; i++) {
                Int32 textint = Convert.ToInt32(str[i]);
                Marshal.WriteInt32(ptr, i*int32_size, textint);
            }
            Marshal.WriteInt32(ptr, str.Length * int32_size, 0);

            return ptr;
    #endif
        }
    }

}