using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace uVM.Compiler.Metadata
{
    interface IMetadataComponent
    {
        void Write(BinaryWriter writer);
    }
}
