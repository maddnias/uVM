using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace uVM.Compiler.Metadata
{
    class FunctionHeader : IMetadataComponent
    {
        public byte ReturnType { get; set; }
        public int OpCount { get; set; }
        public uint ParameterCount { get; set; }
        public uint VariableCount { get; set; }
        public int CodeSize { get; set; }
        public byte[] Code { get; set; }

        public FunctionHeader(byte returnType, int opCount, byte[] code, uint parameterCount, uint variableCount)
        {
            ReturnType = returnType;
            OpCount = opCount;
            Code = code;
            ParameterCount = parameterCount;
            VariableCount = variableCount;
        }

        public void Write(BinaryWriter writer)
        {
            writer.Write(ReturnType);
            writer.Write(ParameterCount);
            for (var i = 0; i < ParameterCount; i++)
                writer.Write(TestCompiler.DoubleShort2Int(1, 1));
            writer.Write(VariableCount);
            for (var i = 0; i < VariableCount; i++)
                writer.Write(TestCompiler.DoubleShort2Int(1, 1));
            writer.Write(OpCount);
            writer.Write((int)Code.Length);
            writer.Write(Code);
        }
    }
}
