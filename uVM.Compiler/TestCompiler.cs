﻿using System;
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
        public static void Compile()
        {
            var filehdr = new FileHeader(0);
            var functionTbl = new FunctionTable(2);

            var mainfunc = new FunctionHeader(8, 3, null, 0);
            var func2 = new FunctionHeader(1, 4, null, 1);

            using (var ms = new MemoryStream())
            using (var bw = new BinaryWriter(ms))
            {
                bw.Write(0); // PUSH 500
                bw.Write(DoubleInt2Long(1, 7, -1000));
                bw.Write(3); // LCall 1
                bw.Write(DoubleInt2Long(5, -1, 1));
                bw.Write(1); // RET

                mainfunc.Code = ms.ToArray();
            }

            using (var ms = new MemoryStream())
            using (var bw = new BinaryWriter(ms))
            {
                bw.Write(9);
                bw.Write(DoubleInt2Long(1, -1, 0));
                bw.Write(0); // PUSH 500
                bw.Write(DoubleInt2Long(2, -1, 0));
                bw.Write((long) 2);
                bw.Write(5);
                bw.Write(1);

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
        static long DoubleInt2Long(int mainType, int subType, int value)
        {
            int type = DoubleShort2Int(mainType, subType);
            long b = type;
            b = b << 32;
            b = b | (uint)value;

            return b;
        }

        static int DoubleShort2Int(int hi, int lo)
        {
            int b = hi;
            b = b << 16;
            b = b | (ushort)lo;

            return b;
        }
    }
}