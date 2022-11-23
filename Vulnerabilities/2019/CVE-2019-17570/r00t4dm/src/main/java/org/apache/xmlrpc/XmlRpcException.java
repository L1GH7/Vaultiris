//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//

package org.apache.xmlrpc;

import java.io.PrintStream;
import java.io.PrintWriter;

public class XmlRpcException extends Exception {
    private static final long serialVersionUID = 3258693217049325618L;
    public final int code;
    public final Throwable linkedException;

    public XmlRpcException(int pCode, String pMessage) {
        this(pCode, pMessage, (Throwable)null);
    }

    public XmlRpcException(String pMessage, Throwable pLinkedException) {
        this(0, pMessage, pLinkedException);
    }

    public XmlRpcException(String pMessage) {
        this(0, pMessage, (Throwable)null);
    }

    public XmlRpcException(int pCode, String pMessage, Throwable pLinkedException) {
        super(pMessage);
        this.code = pCode;
//        this.linkedException = pLinkedException;
        this.linkedException = pLinkedException;
    }

    public void printStackTrace(PrintStream pStream) {
        super.printStackTrace(pStream);
        if (this.linkedException != null) {
            pStream.println("Caused by111111:");
            this.linkedException.printStackTrace(pStream);
        }

    }

    public void printStackTrace(PrintWriter pWriter) {
        super.printStackTrace(pWriter);
        if (this.linkedException != null) {
            pWriter.println("Caused by1111111:");
            this.linkedException.printStackTrace(pWriter);
        }

    }

    public Throwable getCause() {
        return this.linkedException;
    }
}
