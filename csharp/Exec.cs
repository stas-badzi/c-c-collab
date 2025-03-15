using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Reflection.Metadata;
using Utility;
using Cpp;

namespace Cpp
{
    public class _ {
        public static void ThrowMsg(string str) { CppImp._.ThrowMsg(TypeConvert.StringToPtr(str)); }
    }

    public class Exec {
        public static string GetRootDir() { return TypeConvert.PtrToString(CppImp.System.GetRootDir()); }

        public static string ToNativePath(string path) { return TypeConvert.PtrToString(CppImp.System.ToNativePath(TypeConvert.StringToPtr(path))); }

        public static IntPtr AllocateMemory(nuint bytes) { return CppImp.System.AllocateMemory(bytes); }

        public static void FreeMemory(IntPtr pointer) { CppImp.System.FreeMemory(pointer); }

        public static IntPtr MovePointer(IntPtr pointer, int bytes) { return CppImp.System.MovePointer(pointer, bytes);}

        public static T ReadPointer<T>(IntPtr pointer) {
            return typeof(T).Name switch
            {
                nameof(IntPtr) or nameof(UIntPtr) => TypeConvert.BitConvert<IntPtr, T>(CppImp.System.ReadPointer_nint(pointer)),
                nameof(SByte) => TypeConvert.BitConvert<sbyte, T>(CppImp.System.ReadPointer_int8(pointer)),
                nameof(Byte) => TypeConvert.BitConvert<byte, T>(CppImp.System.ReadPointer_uint8(pointer)),
                nameof(Int16) => TypeConvert.BitConvert<short, T>(CppImp.System.ReadPointer_int16(pointer)),
                nameof(UInt16) => TypeConvert.BitConvert<ushort, T>(CppImp.System.ReadPointer_uint16(pointer)),
                nameof(Int32) => TypeConvert.BitConvert<int, T>(CppImp.System.ReadPointer_int32(pointer)),
                nameof(UInt32) => TypeConvert.BitConvert<uint, T>(CppImp.System.ReadPointer_uint32(pointer)),
                nameof(Int64) => TypeConvert.BitConvert<long, T>(CppImp.System.ReadPointer_int64(pointer)),
                nameof(UInt64) => TypeConvert.BitConvert<ulong, T>(CppImp.System.ReadPointer_uint64(pointer)),
                _ => throw new ArgumentException("Exporting this type has not been implemented"),
            };
        }
        public static T ReadPointer<T>(IntPtr pointer, int offset) {
            return typeof(T).Name switch
            {
                nameof(IntPtr) or nameof(UIntPtr) => TypeConvert.BitConvert<IntPtr, T>(CppImp.System.ReadPointer_nint(pointer, offset)),
                nameof(SByte) => TypeConvert.BitConvert<sbyte, T>(CppImp.System.ReadPointer_int8(pointer, offset)),
                nameof(Byte) => TypeConvert.BitConvert<byte, T>(CppImp.System.ReadPointer_uint8(pointer, offset)),
                nameof(Int16) => TypeConvert.BitConvert<short, T>(CppImp.System.ReadPointer_int16(pointer, offset)),
                nameof(UInt16) => TypeConvert.BitConvert<ushort, T>(CppImp.System.ReadPointer_uint16(pointer, offset)),
                nameof(Int32) => TypeConvert.BitConvert<int, T>(CppImp.System.ReadPointer_int32(pointer, offset)),
                nameof(UInt32) => TypeConvert.BitConvert<uint, T>(CppImp.System.ReadPointer_uint32(pointer, offset)),
                nameof(Int64) => TypeConvert.BitConvert<long, T>(CppImp.System.ReadPointer_int64(pointer, offset)),
                nameof(UInt64) => TypeConvert.BitConvert<ulong, T>(CppImp.System.ReadPointer_uint64(pointer, offset)),
                _ => throw new ArgumentException("Exporting this type has not been implemented"),
            };
        }
        public static void WritePointer<T>(IntPtr pointer, T value) {
            switch (typeof(T).Name) {
                case nameof(IntPtr):
                case nameof(UIntPtr):
                    CppImp.System.WritePointer_nint(pointer,TypeConvert.BitConvert<T,IntPtr>(value));
                    return;
                case nameof(SByte):
                    CppImp.System.WritePointer_int8(pointer,TypeConvert.BitConvert<T,sbyte>(value));
                    return;
                case nameof(Byte):
                    CppImp.System.WritePointer_uint8(pointer,TypeConvert.BitConvert<T,byte>(value));
                    return;
                case nameof(Int16):
                    CppImp.System.WritePointer_int16(pointer,TypeConvert.BitConvert<T,short>(value));
                    return;
                case nameof(UInt16):
                    CppImp.System.WritePointer_uint16(pointer,TypeConvert.BitConvert<T,ushort>(value));
                    return;
                case nameof(Int32):
                    CppImp.System.WritePointer_int32(pointer,TypeConvert.BitConvert<T,int>(value));
                    return;
                case nameof(UInt32):
                    CppImp.System.WritePointer_uint32(pointer,TypeConvert.BitConvert<T,uint>(value));
                    return;
                case nameof(Int64):
                    CppImp.System.WritePointer_int64(pointer,TypeConvert.BitConvert<T,long>(value));
                    return;
                case nameof(UInt64):
                    CppImp.System.WritePointer_uint64(pointer,TypeConvert.BitConvert<T,ulong>(value));
                    return;
                default:
                    throw new ArgumentException("Importing this type has not been implemented");
            }
        }
        public static void WritePointer<T>(IntPtr pointer, int offset, T value) {
            switch (typeof(T).Name) {
                case nameof(IntPtr):
                case nameof(UIntPtr):
                    CppImp.System.WritePointer_nint(pointer, offset, TypeConvert.BitConvert<T,IntPtr>(value));
                    return;
                case nameof(SByte):
                    CppImp.System.WritePointer_int8(pointer, offset, TypeConvert.BitConvert<T,sbyte>(value));
                    return;
                case nameof(Byte):
                    CppImp.System.WritePointer_uint8(pointer, offset, TypeConvert.BitConvert<T,byte>(value));
                    return;
                case nameof(Int16):
                    CppImp.System.WritePointer_int16(pointer, offset, TypeConvert.BitConvert<T,short>(value));
                    return;
                case nameof(UInt16):
                    CppImp.System.WritePointer_uint16(pointer, offset, TypeConvert.BitConvert<T,ushort>(value));
                    return;
                case nameof(Int32):
                    CppImp.System.WritePointer_int32(pointer, offset, TypeConvert.BitConvert<T,int>(value));
                    return;
                case nameof(UInt32):
                    CppImp.System.WritePointer_uint32(pointer, offset, TypeConvert.BitConvert<T,uint>(value));
                    return;
                case nameof(Int64):
                    CppImp.System.WritePointer_int64(pointer, offset, TypeConvert.BitConvert<T,long>(value));
                    return;
                case nameof(UInt64):
                    CppImp.System.WritePointer_uint64(pointer, offset, TypeConvert.BitConvert<T,ulong>(value));
                    return;
                default:
                    throw new ArgumentException("Importing this type has not been implemented");
            }
        }
    
    }
} 