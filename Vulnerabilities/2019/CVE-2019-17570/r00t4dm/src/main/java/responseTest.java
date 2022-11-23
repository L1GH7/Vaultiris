
import org.apache.xmlrpc.XmlRpcException;
import org.apache.xmlrpc.client.XmlRpcClient;
import org.apache.xmlrpc.client.XmlRpcClientConfig;
import org.apache.xmlrpc.client.XmlRpcClientConfigImpl;
import org.apache.xmlrpc.common.TypeFactoryImpl;
import org.apache.xmlrpc.common.XmlRpcHttpRequestConfigImpl;
import org.apache.xmlrpc.common.XmlRpcStreamConfig;
import org.apache.xmlrpc.common.XmlRpcStreamRequestConfig;
import org.apache.xmlrpc.parser.XmlRpcResponseParser;
import org.apache.xmlrpc.util.SAXParsers;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;

import java.io.*;
import java.net.URL;
import java.util.HashMap;
import java.util.List;
import java.util.Vector;

/**
 * 受害客户端
 */
public class responseTest {

    public static void main (String []args) throws IOException, SAXException, org.xml.sax.SAXException, XmlRpcException {

        XmlRpcClient client = new XmlRpcClient();
        XmlRpcClientConfigImpl config = new XmlRpcClientConfigImpl();
        config.setServerURL(new URL("http://localhost:8899/xmlrpc"));
//        config.setEnabledForExceptions(true);
//        config.setEnabledForExtensions(true);
        client.setConfig(config);
        Vector params = new Vector();
        params.addElement("r00t4dm");
        Object result2 = client.execute("Calc.ca1lc", params);
        System.out.println(result2);

    }

    private HashMap getResponseObject(InputStream file) throws XmlRpcException, IOException, SAXException, org.xml.sax.SAXException {
        final XMLReader xr = SAXParsers.newXMLReader();
        XmlRpcResponseParser xp = new XmlRpcResponseParser(new XmlRpcHttpRequestConfigImpl(), new TypeFactoryImpl(new XmlRpcClient()));
        xr.setContentHandler(xp);
        xr.parse(String.valueOf(new InputSource(file)));
        return (HashMap) xp.getResult();
    }

}
