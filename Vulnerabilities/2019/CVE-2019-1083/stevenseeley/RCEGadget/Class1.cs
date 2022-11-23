using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RCEGadget
{
    public class GadgetOSCommand
    {
        public GadgetOSCommand()
        {
            System.Diagnostics.Process.Start("calc");
        }
    }
}
