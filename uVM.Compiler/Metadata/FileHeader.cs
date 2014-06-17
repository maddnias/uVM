using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace uVM.Compiler.Metadata
{
    public class FileHeader : IMetadataComponent
    {
        public const int Magic = 0xDEAD;
        public int EntryPoint;

        public FileHeader(int entryPoint)
        {
            EntryPoint = entryPoint;
        }

        public void Write(BinaryWriter writer)
        {
            writer.Write(Magic);
            writer.Write(EntryPoint);
        }
    }
}
