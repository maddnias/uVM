using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using uVM.Compiler.Metadata;

namespace uVM.Compiler
{
    public class TestCompiler
    {
        public static void CompileTest1()
        {
            
            /*
             * void mainfunc() {
             *    return func2(500);
             * }
             * 
             * int func2(int param) {
             *    return param * 2;
             * }
             * 
             */

            var filehdr = new FileHeader(0);
            var functionTbl = new FunctionTable(2);

            var mainfunc = new FunctionHeader(8, 3, null, 0, 0);
            var func2 = new FunctionHeader(1, 4, null, 1, 0);

            using (var ms = new MemoryStream())
            using (var bw = new BinaryWriter(ms))
            {
                bw.Write(0); // PUSH 500
                bw.Write(DoubleInt2Long(1, 1, 500));
                bw.Write(3); // LCALL 1
                bw.Write(DoubleInt2Long(1, 1, 1));
                bw.Write(1); // RET

                mainfunc.Code = ms.ToArray();
            }

            using (var ms = new MemoryStream())
            using (var bw = new BinaryWriter(ms))
            {
                bw.Write(9); // RARG 0
                bw.Write(DoubleInt2Long(1, 1, 0));
                bw.Write(0); // PUSH 2
                bw.Write(DoubleInt2Long(1, 1, 2));
                bw.Write(5); // MUL
                bw.Write(1); // RET

                func2.Code = ms.ToArray();
            }

            using (var ms = new MemoryStream())
            using (var bw = new BinaryWriter(ms))
            {
                filehdr.Write(bw);
                functionTbl.Write(bw);
                mainfunc.Write(bw);
                func2.Write(bw);

                File.WriteAllBytes("C:\\Users\\Mattias\\Documents\\testfile.uc", ms.ToArray());
            }
        }

        public static void CompileTest2()
        {

            /*
             * void mainfunc() {
             *    array<int> arr = mkarr(array<int>[1]);
             *    arr[0] = 1337;
             *    return arr[0];
             * }
             * 
             */

            var filehdr = new FileHeader(0);
            var functionTbl = new FunctionTable(1);

            var mainfunc = new FunctionHeader(8, 11, null, 0, 1);

            using (var ms = new MemoryStream())
            using (var bw = new BinaryWriter(ms))
            {
                bw.Write(0); // PUSH 1
                bw.Write(DoubleInt2Long(1, 1, 1));
                bw.Write(11); // MKARR <DT_INTEGER>
                bw.Write(DoubleInt2Long(1, 9, 0));
                bw.Write(14); // SETVAR <0>
                bw.Write(DoubleInt2Long(1, 9, 0));
                bw.Write(15); // GETVAR <0>
                bw.Write(DoubleInt2Long(1, 1, 0));
                bw.Write(0); // PUSH 0
                bw.Write(DoubleInt2Long(1, 1, 0));
                bw.Write(0); // PUSH 1000
                bw.Write(DoubleInt2Long(1, 1, 1337));
                bw.Write(12); // SETELEM <0>
                bw.Write(DoubleInt2Long(0, 0, 0));
                bw.Write(15); // GETVAR <0>
                bw.Write(DoubleInt2Long(1, 1, 0));
                bw.Write(0); // PUSH 0
                bw.Write(DoubleInt2Long(1, 1, 0));
                bw.Write(13); // GETELEM <0>
                bw.Write(1); // RET

                mainfunc.Code = ms.ToArray();
            }

            using (var ms = new MemoryStream())
            using (var bw = new BinaryWriter(ms))
            {
                filehdr.Write(bw);
                functionTbl.Write(bw);
                mainfunc.Write(bw);

                File.WriteAllBytes("C:\\Users\\Mattias\\Documents\\testfile.uc", ms.ToArray());
            }
        }
        public static void CompileTest3()
        {

            /*
             * void mainfunc() {
             *    array<int> arr = mkarr(array<int>[1]);
             *    arr[0] = 1000;
             *    return arr[0];
             * }
             * 
             */

            var filehdr = new FileHeader(0);
            var functionTbl = new FunctionTable(1);

            var mainfunc = new FunctionHeader(8, 37, null, 0, 2);

            using (var ms = new MemoryStream())
            using (var bw = new BinaryWriter(ms))
            {
                bw.Write(0); // PUSH 5
                bw.Write(DoubleInt2Long(1, 1, 5));
                bw.Write(11); // MKARR <DT_INTEGER>
                bw.Write(DoubleInt2Long(1, 9, 0));
                bw.Write(14); // SETVAR <0>
                bw.Write(DoubleInt2Long(1, 9, 0));
                bw.Write(0); // PUSH 0
                bw.Write(DoubleInt2Long(1, 1, 0));
                bw.Write(14); // SETVAR <1>
                bw.Write(DoubleInt2Long(1, 1, 1));

                bw.Write(15); // GETVAR <0>
                bw.Write(DoubleInt2Long(1, 9, 0));
                bw.Write(0); // PUSH 0
                bw.Write(DoubleInt2Long(1, 1, 0));
                bw.Write(0); // PUSH 1
                bw.Write(DoubleInt2Long(1, 1, 1));
                bw.Write(12); // SETELEM <0>
                bw.Write(DoubleInt2Long(0, 0, 0));

                bw.Write(15); // GETVAR <0>
                bw.Write(DoubleInt2Long(1, 9, 0));
                bw.Write(0); // PUSH 1
                bw.Write(DoubleInt2Long(1, 1, 1));
                bw.Write(0); // PUSH 2
                bw.Write(DoubleInt2Long(1, 1, 2));
                bw.Write(12); // SETELEM <0>
                bw.Write(DoubleInt2Long(0, 0, 0));

                bw.Write(15); // GETVAR <0>
                bw.Write(DoubleInt2Long(1, 9, 0));
                bw.Write(0); // PUSH 2
                bw.Write(DoubleInt2Long(1, 1, 2));
                bw.Write(0); // PUSH 3
                bw.Write(DoubleInt2Long(1, 1, 3));
                bw.Write(12); // SETELEM <0>
                bw.Write(DoubleInt2Long(0, 0, 0));

                bw.Write(15); // GETVAR <0>
                bw.Write(DoubleInt2Long(1, 9, 0));
                bw.Write(0); // PUSH 3
                bw.Write(DoubleInt2Long(1, 1, 3));
                bw.Write(0); // PUSH 4
                bw.Write(DoubleInt2Long(1, 1, 4));
                bw.Write(12); // SETELEM <0>
                bw.Write(DoubleInt2Long(0, 0, 0));

                bw.Write(15); // GETVAR <0>
                bw.Write(DoubleInt2Long(1, 9, 0));
                bw.Write(0); // PUSH 4
                bw.Write(DoubleInt2Long(1, 1, 4));
                bw.Write(0); // PUSH 5
                bw.Write(DoubleInt2Long(1, 1, 5));
                bw.Write(12); // SETELEM <0>
                bw.Write(DoubleInt2Long(0, 0, 0));

                bw.Write(15); // GETVAR <0>
                bw.Write(DoubleInt2Long(1, 9, 0));
                bw.Write(0); // PUSH 0
                bw.Write(DoubleInt2Long(1, 1, 0));
                bw.Write(0); // PUSH 1
                bw.Write(DoubleInt2Long(1, 1, 1));
                bw.Write(12); // SETELEM <0>
                bw.Write(DoubleInt2Long(0, 0, 0));
                //loop start
                bw.Write(0xF); // GETVAR <0>
                bw.Write(DoubleInt2Long(1, 9, 0));
                bw.Write(0xF); // GETVAR <1>
                bw.Write(DoubleInt2Long(1, 1, 1));
                bw.Write(0xD); // GETELEM
                bw.Write(0); // PUSH 5
                bw.Write(DoubleInt2Long(1, 1, 5));
                bw.Write(2); // ADD
                bw.Write(0xF); // GETVAR <0>
                bw.Write(DoubleInt2Long(1, 9, 0));
                bw.Write(12); // SETELEM <0>
                bw.Write(DoubleInt2Long(0, 0, 0));
                bw.Write(1); // RET

                mainfunc.Code = ms.ToArray();
            }

            using (var ms = new MemoryStream())
            using (var bw = new BinaryWriter(ms))
            {
                filehdr.Write(bw);
                functionTbl.Write(bw);
                mainfunc.Write(bw);

                File.WriteAllBytes("C:\\Users\\Mattias\\Documents\\testfile.uc", ms.ToArray());
            }
        }

        public static long DoubleInt2Long(int mainType, int subType, int value)
        {
            int type = DoubleShort2Int(mainType, subType);
            long b = type;
            b = b << 32;
            b = b | (uint)value;

            return b;
        }

        public static int DoubleShort2Int(int hi, int lo)
        {
            int b = hi;
            b = b << 16;
            b = b | (ushort)lo;

            return b;
        }
    }
}
