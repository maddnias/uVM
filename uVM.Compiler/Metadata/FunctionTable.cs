using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace uVM.Compiler.Metadata
{
    class FunctionTable : IMetadataComponent
    {
        public int FunctionCount { get; set; }

        public FunctionTable(int functionCount)
        {
            FunctionCount = functionCount;
        }

        public void Write(BinaryWriter writer)
        {
            writer.Write(FunctionCount);
        }
    }
}
