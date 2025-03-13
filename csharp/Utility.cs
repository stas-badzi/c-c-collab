using System.Diagnostics.CodeAnalysis;
using System.Runtime.InteropServices;
using Cpp;
using _ = Cpp._;
using CppImp;

#pragma warning disable CS8500

namespace Utility
{
    [StructLayout(LayoutKind.Sequential)]  
    public struct Pair<T1,T2> {
        public T1 first;
        public T2 second;
        public Pair(T1 first, T2 second) {
            this.first = first;
            this.second = second;
        }
    }

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
    public class MarshalMore {
        public static unsafe void Write<T>(IntPtr ptr, Int32 ofs, T val) {
            T a = TypeConvert.BitConvert<byte,T>(0);
            if (a is nint || a is nuint) {
                Exec.WritePointer<IntPtr>(ptr, ofs, TypeConvert.BitConvert<T,IntPtr>(val));
                return; 
            }
            switch (sizeof(T)) {
                case sizeof(Byte):
                    Exec.WritePointer<Byte>(ptr, ofs, TypeConvert.BitConvert<T,Byte>(val));
                    break;
                case sizeof(Int16):
                    Exec.WritePointer<Int16>(ptr, ofs, TypeConvert.BitConvert<T,Int16>(val));
                    break;
                case sizeof(Int32):
                    Exec.WritePointer<Int32>(ptr, ofs, TypeConvert.BitConvert<T,Int32>(val));
                    break;
                case sizeof(Int64):
                    Exec.WritePointer<Int64>(ptr, ofs, TypeConvert.BitConvert<T,Int64>(val));
                    break;
                default:
                    throw new ArgumentException("The size of " + typeof(T) + "(" + sizeof(T) + "), doesn't match any Exec.WritePointer<T> Function");
            }
        }

        public static unsafe void Write<T>(IntPtr ptr, T val) {
            if (typeof(T) == typeof(nint) || typeof(T) == typeof(nuint)) {
                Exec.WritePointer<IntPtr>(ptr, TypeConvert.BitConvert<T,IntPtr>(val));
                return; 
            }
            switch (sizeof(T)) {
                case sizeof(Byte):
                    Exec.WritePointer<Byte>(ptr, TypeConvert.BitConvert<T,Byte>(val));
                    break;
                case sizeof(Int16):
                    Exec.WritePointer<Int16>(ptr, TypeConvert.BitConvert<T,Int16>(val));
                    break;
                case sizeof(Int32):
                    Exec.WritePointer<Int32>(ptr, TypeConvert.BitConvert<T,Int32>(val));
                    break;
                case sizeof(Int64):
                    Exec.WritePointer<Int64>(ptr, TypeConvert.BitConvert<T,Int64>(val));
                    break;
                default:
                    throw new ArgumentException("The size of " + typeof(T) + "(" + sizeof(T) + "), doesn't match any Exec.WritePointer<T> Function");
            }
        }

        public static unsafe T Read<T>(IntPtr ptr, Int32 ofs) {
            if (typeof(T) == typeof(nint) || typeof(T) == typeof(nuint)) {
                return TypeConvert.BitConvert<IntPtr,T>(Exec.ReadPointer<IntPtr>(ptr, ofs));
            }
            return sizeof(T) switch
            {
                sizeof(Byte) => TypeConvert.BitConvert<Byte, T>(Exec.ReadPointer<Byte>(ptr, ofs)),
                sizeof(Int16) => TypeConvert.BitConvert<Int16, T>(Exec.ReadPointer<Int16>(ptr, ofs)),
                sizeof(Int32) => TypeConvert.BitConvert<Int32, T>(Exec.ReadPointer<Int32>(ptr, ofs)),
                sizeof(Int64) => TypeConvert.BitConvert<Int64, T>(Exec.ReadPointer<Int64>(ptr, ofs)),
                _ => throw new ArgumentException("The size of " + typeof(T) + "(" + sizeof(T) + "), doesn't match any Exec.ReadPointer<T> Function"),
            };
        }

        public static unsafe T Read<T>(IntPtr ptr) {
            if (typeof(T) == typeof(nint) || typeof(T) == typeof(nuint)) {
                return TypeConvert.BitConvert<IntPtr,T>(Exec.ReadPointer<IntPtr>(ptr));
            }
            switch (sizeof(T)) {
                case sizeof(Byte):
                    return TypeConvert.BitConvert<Byte,T>(Exec.ReadPointer<Byte>(ptr));
                case sizeof(Int16):
                    return TypeConvert.BitConvert<Int16,T>(Exec.ReadPointer<Int16>(ptr));
                case sizeof(Int32):
                    return TypeConvert.BitConvert<Int32,T>(Exec.ReadPointer<Int32>(ptr));
                case sizeof(Int64):
                    return TypeConvert.BitConvert<Int64,T>(Exec.ReadPointer<Int64>(ptr));
                default:
                    throw new ArgumentException("The size of " + typeof(T) + "(" + sizeof(T) + "), doesn't match any Exec.ReadPointer<T> Function");
            }
        }

        public static void WriteBoolean(IntPtr ptr, Int32 ofs, Boolean val) {
            Exec.WritePointer<Byte>(ptr, ofs, TypeConvert.BitConvert<Boolean,Byte>(val));
        }

        public static void WriteBoolean(IntPtr ptr, Boolean val) {
            Exec.WritePointer<Byte>(ptr, TypeConvert.BitConvert<Boolean,Byte>(val));
        }

        public static Boolean ReadBoolean(IntPtr ptr, Int32 ofs) {
            return TypeConvert.BitConvert<Byte,Boolean>(Exec.ReadPointer<Byte>(ptr, ofs));
        }

        public static Boolean ReadBoolean(IntPtr ptr) {
            return TypeConvert.BitConvert<Byte,Boolean>(Exec.ReadPointer<Byte>(ptr));
        }


        public static void WriteSByte(IntPtr ptr, Int32 ofs, SByte val) {
            Exec.WritePointer<Byte>(ptr, ofs, TypeConvert.BitConvert<SByte,Byte>(val));
        }

        public static void WriteSByte(IntPtr ptr, SByte val) {
            Exec.WritePointer<Byte>(ptr, TypeConvert.BitConvert<SByte,Byte>(val));
        }

        public static SByte ReadSByte(IntPtr ptr, Int32 ofs) {
            return TypeConvert.BitConvert<Byte,SByte>(Exec.ReadPointer<Byte>(ptr, ofs));
        }

        public static SByte ReadSByte(IntPtr ptr) {
            return TypeConvert.BitConvert<Byte,SByte>(Exec.ReadPointer<Byte>(ptr));
        }


        public static void WriteUInt16(IntPtr ptr, Int32 ofs, UInt16 val) {
            Exec.WritePointer<Int16>(ptr, ofs, TypeConvert.BitConvert<UInt16,Int16>(val));
        }

        public static void WriteUInt16(IntPtr ptr, UInt16 val) {
            Exec.WritePointer<Int16>(ptr, TypeConvert.BitConvert<UInt16,Int16>(val));
        }

        public static UInt16 ReadUInt16(IntPtr ptr, Int32 ofs) {
            return TypeConvert.BitConvert<Int16,UInt16>(Exec.ReadPointer<Int16>(ptr, ofs));
        }

        public static UInt16 ReadUInt16(IntPtr ptr) {
            return TypeConvert.BitConvert<Int16,UInt16>(Exec.ReadPointer<Int16>(ptr));
        }



        public static void WriteUInt32(IntPtr ptr, Int32 ofs, UInt32 val) {
            Exec.WritePointer<Int32>(ptr, ofs, TypeConvert.BitConvert<UInt32,Int32>(val));
        }

        public static void WriteUInt32(IntPtr ptr, UInt32 val) {
            Exec.WritePointer<Int32>(ptr, TypeConvert.BitConvert<UInt32,Int32>(val));
        }

        public static UInt32 ReadUInt32(IntPtr ptr, Int32 ofs) {
            return TypeConvert.BitConvert<Int32,UInt32>(Exec.ReadPointer<Int32>(ptr, ofs));
        }

        public static UInt32 ReadUInt32(IntPtr ptr) {
            return TypeConvert.BitConvert<Int32,UInt32>(Exec.ReadPointer<Int32>(ptr));
        }



        public static void WriteUInt64(IntPtr ptr, Int32 ofs, UInt64 val) {
            Exec.WritePointer<Int64>(ptr, ofs, TypeConvert.BitConvert<UInt64,Int64>(val));
        }

        public static void WriteUInt64(IntPtr ptr, UInt64 val) {
            Exec.WritePointer<Int64>(ptr, TypeConvert.BitConvert<UInt64,Int64>(val));
        }

        public static UInt64 ReadUInt64(IntPtr ptr, Int32 ofs) {
            return TypeConvert.BitConvert<Int64,UInt64>(Exec.ReadPointer<Int64>(ptr, ofs));
        }

        public static UInt64 ReadUInt64(IntPtr ptr) {
            return TypeConvert.BitConvert<Int64,UInt64>(Exec.ReadPointer<Int64>(ptr));
        }



        public static void WriteUIntPtr(IntPtr ptr, Int32 ofs, UIntPtr val) {
            Exec.WritePointer<IntPtr>(ptr, ofs, TypeConvert.BitConvert<UIntPtr,IntPtr>(val));
        }

        public static void WriteUIntPtr(IntPtr ptr, UIntPtr val) {
            Exec.WritePointer<IntPtr>(ptr, TypeConvert.BitConvert<UIntPtr,IntPtr>(val));
        }

        public static UIntPtr ReadUIntPtr(IntPtr ptr, Int32 ofs) {
            return TypeConvert.BitConvert<IntPtr,UIntPtr>(Exec.ReadPointer<IntPtr>(ptr, ofs));
        }

        public static UIntPtr ReadUIntPtr(IntPtr ptr) {
            return TypeConvert.BitConvert<IntPtr,UIntPtr>(Exec.ReadPointer<IntPtr>(ptr));
        }


        public static void WriteFloat(IntPtr ptr, Int32 ofs, float val) {
            Exec.WritePointer<Int32>(ptr, ofs, TypeConvert.BitConvert<float,Int32>(val));
        }

        public static void WriteFloat(IntPtr ptr, float val) {
            Exec.WritePointer<Int32>(ptr, TypeConvert.BitConvert<float,Int32>(val));
        }

        public static float ReadFloat(IntPtr ptr, Int32 ofs) {
            return TypeConvert.BitConvert<Int32,float>(Exec.ReadPointer<Int32>(ptr, ofs));
        }

        public static float ReadFloat(IntPtr ptr) {
            return TypeConvert.BitConvert<Int32,float>(Exec.ReadPointer<Int32>(ptr));
        }


        public static void WriteDouble(IntPtr ptr, Int32 ofs, Double val) {
            Exec.WritePointer<Int64>(ptr, ofs, TypeConvert.BitConvert<Double,Int64>(val));
        }

        public static void WriteDouble(IntPtr ptr, Double val) {
            Exec.WritePointer<Int64>(ptr, TypeConvert.BitConvert<Double,Int64>(val));
        }

        public static Double ReadDouble(IntPtr ptr, Int32 ofs) {
            return TypeConvert.BitConvert<Int64,Double>(Exec.ReadPointer<Int64>(ptr, ofs));
        }

        public static Double ReadDouble(IntPtr ptr) {
            return TypeConvert.BitConvert<Int64,Double>(Exec.ReadPointer<Int32>(ptr));
        }
    }

    public class TypeConvert
    {
        public static unsafe T2 BitConvert<T1,T2>(T1 val) {
            int T1_size = (typeof(T1) == typeof(nint) || typeof(T1) == typeof(nuint)) ? IntPtr.Size : sizeof(T1);
            int T2_size = (typeof(T2) == typeof(nint) || typeof(T2) == typeof(nuint)) ? IntPtr.Size : sizeof(T2);
            if (T1_size != T2_size) {
                throw new InvalidOperationException(typeof(T1) + " if of size " + T1_size + " which doesn't match " + typeof(T2) + " size (" + T2_size + ") so bitwise conversion cannot be mede");
            }
            void* ptr1 = &val;
            T2* ptr2 = (T2*)ptr1;
            T2 ret = *ptr2;
            return ret;
        }

        public static char UnicodeToUtf8(UInt32 uni) {
            return Convert.ToChar(uni);
        }

        public static UInt32 Utf8ToUnicode(char utf) {
            return Convert.ToUInt32(utf);
        }
        public static String PtrToString(IntPtr ptr)
        {
        #if WIN32 // it's suppose to not be used even on windows
            string? output = Marshal.PtrToStringUni(ptr);
            Exec.FreeMemory(ptr);
            if (output == null) { _.ThrowMsg("PtrToString Null"); return ""; }
            return output;
        #else
            int int32_size = sizeof(Int32);

            String str = "";
            UInt32 intg = new UInt32();
            for (int i = 0; true; i++) {
                intg = Exec.ReadPointer<UInt32>(ptr,i*int32_size);
                if (intg == 0) {
                    break;
                }
                str += Convert.ToChar(intg);
            }
            Exec.FreeMemory(ptr);
            return str;
        #endif
        }

        public static IntPtr StringToPtr(String str)
        {
            #if WIN32
            return Marshal.StringToHGlobalUni(str);
            #else
            int int32_size = sizeof(Int32);

            IntPtr ptr = Exec.AllocateMemory((nuint)((str.Length + 1) * int32_size));
            for (int i = 0; i < str.Length; i++) {
                UInt32 textint = Convert.ToUInt32(str[i]);
                Exec.WritePointer<UInt32>(ptr, i*int32_size, textint);
            }
            Exec.WritePointer<UInt32>(ptr, str.Length * int32_size, 0);

            return ptr;
            #endif

        }

        public static unsafe nint ListToPtr<T>(List<T> list) {
            int T_size = (typeof(T) == typeof(nint) || typeof(T) == typeof(nuint)) ? IntPtr.Size : sizeof(T);
            const int int_size = sizeof(int);

            int size = list.Count;
            nint ptr = Exec.AllocateMemory((nuint)(int_size + T_size * size));

            Exec.WritePointer<Int32>(ptr, size);

            for (int i = 0; i < size; i++) {
                MarshalMore.Write<T>(ptr, int_size + T_size * i, list[i]);
            }

            return ptr;
        }

        public static unsafe List<T> PtrToList<T>(nint ptr) {
            int T_size = (typeof(T) == typeof(nint) || typeof(T) == typeof(nuint)) ? IntPtr.Size : sizeof(T);
            const int int_size = sizeof(int);

            int size = Exec.ReadPointer<Int32>(ptr);
            List<T> list = new List<T>();

            for (int i = 0; i < size; i++) {
                T elem = MarshalMore.Read<T>(ptr, int_size + T_size * i);
                list.Add(elem);
            }

            Exec.FreeMemory(ptr);

            return list;
        }
        public static List<List<Terminal.Symbol>> PtrToTexture(nint ptr, bool direct = false)
        {
            const int int32_size = sizeof(int);
            int intptr_size = nint.Size;
            int height, count;

            var texture = new List<List<Terminal.Symbol>>();

            height = Exec.ReadPointer<Int32>(ptr);
            
            count = 0;
            for (int i = 0; i < height; i++)
            {
                texture.Add(new List<Terminal.Symbol>());
                //***************** here ****************************************
                int width = Exec.ReadPointer<Int32>(ptr, (i + 1) * int32_size + count * intptr_size);
                //***************** end *****************************************
                for (int j = 0; j < width; j++)
                {
                    nint ni = Exec.ReadPointer<IntPtr>(ptr, (i + 2) * int32_size + count * intptr_size);
                    // bro what the fuvk -----------under here----------- (I knew it from the begginig but I couldn't find it)
                    CppImp.Console.Symbol.Inspect(ni);
                    texture[i].Add(new Terminal.Symbol(ni,direct));
                    count++;
                }
            }

            // This is where you do the freeHGlobal
            Exec.FreeMemory(ptr);
            //(other than that congrats in doing all that in one intptr, I always did it by first List<Symbol> -> List<IntPtr> and than List<IntPtr> -> IntPtr , whitch was a small problem later with all the Marshal.FreeHglobal)
            // well... now you have a function PtrToLIst<T>() and ListToPtr<T>() so that's not that needed anyway
            // {you could've encoded the and decoded the texture[i].Count only once as texture[0].Count since it's a rectangle so all rows have the same length}

            return texture;
        }

        public static nint TextureToPtr(List<List<Terminal.Symbol>> texture, bool direct = false)
        {
            const int int32_size = sizeof(int);
            int intptr_size = nint.Size;
            int size, count;

            size = texture.Count;
            count = 0;
            for (int i = 0; i < size; i++)
            {
                count += texture[i].Count;
            }
            nint texturePtr = Exec.AllocateMemory((nuint)( (size + 1) * int32_size + count * intptr_size ));

            count = 0;
            Exec.WritePointer<Int32>(texturePtr, size);
            for (int i = 0; i < size; i++)
            {
                Exec.WritePointer<Int32>(texturePtr, (i + 1) * int32_size + count * intptr_size, texture[i].Count);
                for (int j = 0; j < texture[i].Count; j++)
                {
                    if (direct) texture[i][j].UnarmPointer();
                    Exec.WritePointer<IntPtr>(texturePtr, (i + 2) * int32_size + count * intptr_size, texture[i][j].Get());
                    count++;
                }
            }
            return texturePtr;
            // ALWAYS use FreeHGlobal after using this pointer
        }
    }

}